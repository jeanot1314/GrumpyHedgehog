
/* Project : Don't touch the GrumpyHedgehog
   Project description : https://hackaday.io/project/180123-gesturepattern-recognition-without-camera-tof
   Based on VL53L1X sensors from ST
   Author : Jean Perardel
*/

#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include "Keyboard.h"
#include "bitmap.h"

#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#endif

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

// Librairies for VL53L1X
#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>//Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>


// *********************** LCD TFT variables *************************
#define TFT_CS  8
#define TFT_RST 10 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC  9

#ifndef ARDUINO_SAMD_MKRWIFI1010
#define TFT_MOSI 11
#define TFT_SCK 13
#endif
// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// For 1.14", 1.3", 1.54", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#define TFT_DRAW_ALGO_1

// *********************** TOF variables *************************
#define NUMBER_OF_TOF_SENSORS 3
#define SERIAL_DEBUG 0
#define MATRICE_DIMENSION 16
//#define SHUTDOWN_PIN 2
//#define INTERRUPT_PIN 3

#if defined(TEENSYDUINO)
#define TOF_SDA 18
#define TOF_SCL 19
#define VL53L1X_1 17
#define VL53L1X_2 16
#define VL53L1X_3 15
#define VL53L1X_4 14

#elif defined(ARDUINO_SAMD_MKRWIFI1010)
#define TOF_SDA 8
#define TOF_SCL 9
#define VL53L1X_1 7
#define VL53L1X_2 6
#define VL53L1X_3 5
#define VL53L1X_4 4

#else // #if defined(__AVR_ATmega8__)
#define TOF_SDA A4
#define TOF_SCL A5
#define VL53L1X_1 A0
#define VL53L1X_2 A1
#define VL53L1X_3 A2
#define VL53L1X_4 A3
#endif

//SFEVL53L1X distanceSensor_1, distanceSensor_2, distanceSensor_3, distanceSensor_4;
SFEVL53L1X distanceSensor[4];

// *********************** IR variables *************************
#define DS18B20_PIN 0
#define IR_LED_PIN 1
#define IR_TSOP2236_PIN 3
byte ledPin = LED_BUILTIN;

// *********************** RELAY/SERVO variables *************************
#define RELAY_PIN 2
#define SERVO_PIN 4
#define SERVO_SPEED 10
Servo myservo;  // create servo object to control a servo
int pos_servo = 0;    // variable to store the servo position

// *********************** Person Counter BLYNK variables *************************
int number_person = 0;
#define NB_PERSON_MAX 2 // Person Counter

#ifdef ARDUINO_SAMD_MKRWIFI1010
volatile bool event = false;
char auth[] = "";
char ssid[] = "";
char pass[] = "";

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  //Serial.print("a");
  // process received value
}
BLYNK_READ(V0)
{
  Blynk.virtualWrite(V0, number_person);
}
BlynkTimer timer;

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, number_person);
}
#endif

// *********************** PATERN Definition *************************
boolean patern_1[4][4] = {
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
  0, 1, 1, 0,
};

boolean patern_2[4][4] = {
  0, 0, 1, 1,
  0, 0, 1, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
};

boolean patern_3[4][4] = {
  1, 0, 0, 1,
  1, 0, 0, 1,
  1, 1, 1, 1,
  1, 1, 1, 1,
};

// *********************** Hedgehog LCD design definition *************************

#define CODE_RIGHT 100
#define CODE_LEFT 101
#define CODE_UP 102
#define CODE_STATIC 103

// Definition of LCD design size and colors
// RGB 565 : F8 7E 1F
#define EYES_SIZE tft.height()/16
#define EYES_COLOR 0xFD00
#define EYES_VERTICAL_POSITION 3*tft.height()/10
#define EYES_1_HORIZONTAL_POSITION tft.width()/4
#define EYES_2_HORIZONTAL_POSITION 3*tft.width()/4

#define NOSE_SIZE tft.height()/4//22
#define NOSE_DOT_SIZE tft.height()/20
#define NOSE_COLOR 0x8200
#define NOSE_VERTICAL_POSITION 12*tft.height()/20
#define NOSE_HORIZONTAL_POSITION tft.height()/2

#define MOUTH_COLOR 0x9100
#define SPEECH_COLOR 0xFD00

#define PATERN_ON 0x001F
#define PATERN_OFF 0x00E0

// *********************** GENERAL variables *************************
// Reset the main counter at :
#define MAX_COUNTER 0xFFFF

// Definition of pseudo real time. Frequency will define the number of time the program enter the function on MAX_COUNTER number
#define Blink_FREQUENCY 256
#define Blink_FREQUENCY_READ_PER_CYCLE global_cpt%((MAX_COUNTER/Blink_FREQUENCY)+1)

#define MAX_DISTANCE_SHORT_DETECTION 1280
#define MAX_DISTANCE_LONG_DETECTION 3000


#define SERVO_STOP_POSITION 180
#define SERVO_ARROW_POSITION 0

#define MODE_USB 0
#define MODE_RELAY 1
#define MODE_SERVOMOTOR 2
#define MODE_IR 3
#define MODE_PERSON_COUNTER 4

int device_MODE = 0; // Set here the starting mode
int nb_person = 0; // Actual number of person in the room for Person Counter
int global_cpt = 0; // Main Counter of the program
boolean flag_text_timeout = 0; // This flag is used to timeout the text on the screen
boolean flag_unlock = 0; // This flag is used to timeout the unlock sign on the screen

void setup(void) {

  // SETUP TIME OF FLIGHT PIN
#ifdef TEENSYDUINO
  Wire.setSDA(TOF_SDA);
  Wire.setSCL(TOF_SCL);
#endif
  Wire.begin();
  VL53L1X_init(NUMBER_OF_TOF_SENSORS);

  Serial.begin(9600);

#ifdef USB_HID
  Keyboard.begin();
#endif
#ifdef ARDUINO_SAMD_MKRWIFI1010
  Blynk.begin(auth, ssid, pass);
#endif

  pinMode(IR_LED_PIN, OUTPUT);
  pinMode(IR_TSOP2236_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
  myservo.write(SERVO_STOP_POSITION);

  HH_TFT_init(); // Initialize Grumpy Hedgehog LCD Face

  Serial.println("-----START PROGRAM GRUMPY HEDGEHOG-----");
  HH_wake_up();
}

void loop() {
  int detection_trigger = 0;


  if (device_MODE != MODE_PERSON_COUNTER) {
    detection_trigger = detect_All(); // General detection for TOF on the entire grid at short range
  }
  else {
    detection_trigger = detect_All(); // General detection for TOF on the entire grid at short range
    //detection_trigger = detect_All_longDistance(); // General detection for TOF on the entire grid at long range for person Counter
  }

  if (detection_trigger) {
    int Direction = 0;
    if (device_MODE != MODE_PERSON_COUNTER) {
      // ***Direction = detect_LeftRightUpStatic(); // This function detect one of the 4 mouvements : Left / Right / Up / Static
    }
    else {
      // ***Direction = detect_LeftRightUpStatic(); // This function detect one of the 4 mouvements : Left / Right / Up / Static
      //Direction = detect_LeftRightStatic_longDistance(); // This function detect one of the 3 mouvements : Left / Right / Static for Person Counter (long distance)
    }
    if (Direction) {
      global_cpt = 0; // Reinitialize global counter when a movement is detected
      delay(500);
      if (Direction == CODE_RIGHT) { // Right movement detected
        //Serial.println("RIGHT");
#ifdef TFT_DRAW_ALGO_1
        HH_V1_look(+13, 0);
#elif TFT_DRAW_ALGO_2
        HH_V2_look_left_right(44);
#endif
        switch (device_MODE) {
          case MODE_USB: // USB
            HH_speak_mode("USB   PLAY");
#ifdef USB_HID
            Keyboard.print(" "); // Set here the command you want to send
#endif
            break;
          case MODE_PERSON_COUNTER: // Person Counter, +1
            nb_person++;
            HH_speak_counter(nb_person);
            if (nb_person >= NB_PERSON_MAX) {
              myservo.write(SERVO_STOP_POSITION);  // tell servo to go to position STOP
            }
            break;
          case MODE_IR:
            HH_speak_mode("IR    VOL+");
            digitalWrite(IR_LED_PIN, HIGH); // TODO
            break;
          case MODE_RELAY:
            HH_speak_mode("RELAY  ON");
            digitalWrite(RELAY_PIN, HIGH);
            delay(200);
            myservo.write(SERVO_STOP_POSITION); // tell servo to go to position STOP
            break;
        }
      }
      else if (Direction == CODE_LEFT) { // Left movement detected
        //Serial.println("LEFT");
#ifdef TFT_DRAW_ALGO_1
        HH_V1_look(-13, 0);
#elif TFT_DRAW_ALGO_2
        HH_V2_look_left_right(-44);
#endif
        switch (device_MODE) {
          case MODE_USB:
            HH_speak_mode("USB   STOP   ");
#ifdef USB_HID
            Keyboard.print("q");
#endif
            break;
          case MODE_PERSON_COUNTER:
            if (nb_person > 0) {
              nb_person--;
            }
            HH_speak_counter(nb_person);
            if (nb_person < NB_PERSON_MAX) {
              myservo.write(SERVO_ARROW_POSITION); // tell servo to go to position ARROW
            }
            break;
          case MODE_IR:
            HH_speak_mode("IR    VOL-");
            digitalWrite(IR_LED_PIN, LOW); // TODO Finish connecting IR
            break;
          case MODE_RELAY:
            HH_speak_mode("RELAY  OFF");
            digitalWrite(RELAY_PIN, LOW);
            delay(200);
            //myservo.write(SERVO_ARROW_POSITION); // tell servo to go to position ARROW
            break;
        }
      }
      else if (Direction == CODE_UP) { // up movement detected
        //Serial.println("UP");
#ifdef TFT_DRAW_ALGO_1
        HH_V1_look(0, -10);
#elif TFT_DRAW_ALGO_2
        HH_V2_look_up_down(44);
#endif
        delay(200);
        HH_happy();
        //HH_speak_mode("  UNLOCK  ");
        //flag_unlock = 1;
        delay(500);
      }
      else if (Direction == CODE_STATIC) { // static hand movement detected
        //Serial.println("STATIC");
#ifdef TFT_DRAW_ALGO_1
        HH_V1_look(0, 10);
#elif TFT_DRAW_ALGO_2
        HH_V2_look_up_down(-44);
#endif
        delay(200);
        HH_happy();
        if (device_MODE >= 3) {
          device_MODE = 0;
        }
        else {
          //device_MODE++;
        }
        switch (device_MODE) {
          case MODE_USB:
            HH_speak_mode("USB   MODE");
            break;
          case MODE_PERSON_COUNTER:
            nb_person = 0;
            HH_speak_counter(nb_person);
            //VL53L1X_config_long(); // Set TOF configuration for long range
            myservo.write(SERVO_ARROW_POSITION); // tell servo to go to position ARROW
            break;
          case MODE_IR:
            HH_speak_mode("IR    MODE");
            //VL53L1X_config(); // Set TOF configuration for short range
            myservo.write(SERVO_STOP_POSITION); // tell servo to go to position ARROW
            break;
          case MODE_RELAY:
            HH_speak_mode("RELAY MODE");
            break;
        }
        delay(500);
      }
    }
  }

  if ((Blink_FREQUENCY_READ_PER_CYCLE == 0 || Blink_FREQUENCY_READ_PER_CYCLE == 12) && flag_text_timeout == 0) {
    HH_eyes_blink();
    //Serial.print("EYES BLINK :");
    //Serial.print(global_cpt);
    //Serial.print(" --- ");
    //Serial.println(Blink_FREQUENCY_READ_PER_CYCLE);
  }

  if (global_cpt % MAX_COUNTER == MAX_COUNTER / 0x2FF && flag_text_timeout == 1) { // remove text after timeout
    HH_speak_mode("");
    //HH_original_state();
    flag_text_timeout = 0;
  }

  if (flag_unlock == 1) { // start the detection patern mode
    int patern_return = detect_patern(distanceSensor[0], 4, 4);
    tft.fillScreen(ST77XX_BLACK);
    flag_unlock = 0;
    //  HH_original_state();
  }

  if (device_MODE == MODE_PERSON_COUNTER) {
#ifdef ARDUINO_SAMD_MKRWIFI1010
    Serial.print("BLYNK CONNECTED STATUS : ");
    Serial.println(Blynk.connected());
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
#endif
    number_person = random(0, 5);
  }

  if (global_cpt >= MAX_COUNTER) {
    global_cpt = 0;
  }
  else {
    global_cpt++;
  }
}


void patern_1_secret_action() {
#ifdef USB_SERIAL
  Serial.println("PASSWORD");
#endif
#ifdef USB_HID
  Keyboard.print("PASSWORD");
#endif
}

void patern_2_secret_action() {
#ifdef USB_SERIAL
  Serial.println("START_MUSIC");
#endif
#ifdef USB_HID
  Keyboard.print("START_MUSIC");
#endif
}

void patern_3_secret_action() {
#ifdef USB_SERIAL
  Serial.println("HELLO_WORLD");
#endif
#ifdef USB_HID
  Keyboard.print("HELLO_WORLD");
#endif
}
