#include <Wire.h>
#include <SPI.h>

#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>//Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

#define VL53L1X_1 17 //24
#define VL53L1X_2 15 //25
#define VL53L1X_3 16 //26
#define VL53L1X_4 14 //27

#define NB_MESURE 10

SFEVL53L1X distanceSensor_1, distanceSensor_2, distanceSensor_3, distanceSensor_4;

#define MAX_DISTANCE_SHORT_DETECTION 1280
#define MAX_DISTANCE_LONG_DETECTION 3000


void setup() {

  Serial.begin(9600);
  delay(100);
  Serial.println("Starting Program");
  delay(100);

  pinMode(VL53L1X_1, OUTPUT);
  pinMode(VL53L1X_2, OUTPUT);
  pinMode(VL53L1X_3, OUTPUT);
  pinMode(VL53L1X_4, OUTPUT);
  digitalWrite(VL53L1X_1, LOW);
  digitalWrite(VL53L1X_2, LOW);
  digitalWrite(VL53L1X_3, LOW);
  digitalWrite(VL53L1X_4, LOW);

  Wire.setSDA(18);//34
  Wire.setSCL(19);//33
  Wire.begin();



  digitalWrite(VL53L1X_1, HIGH);
  delay(150);
  if (distanceSensor_1.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 1 failed to begin. Please check wiring. Freezing...");
    while (1);
  }

  Serial.print("Sensor 1 address : 0x");
  Serial.println(distanceSensor_1.getI2CAddress(), HEX);
  delay(100);
  distanceSensor_1.setI2CAddress(0x11);
  delay(100);
  Serial.print("Sensor 1 address : 0x");
  Serial.println(distanceSensor_1.getI2CAddress(), HEX);


  digitalWrite(VL53L1X_2, HIGH);
  delay(150);
  if (distanceSensor_2.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 2 failed to begin. Please check wiring. Freezing...");
    while (1);
  }
  Serial.print("Sensor 2 address : 0x");
  Serial.println(distanceSensor_2.getI2CAddress(), HEX);
  delay(100);
  distanceSensor_2.setI2CAddress(0x22);
  delay(100);
  Serial.print("Sensor 2 address : 0x");
  Serial.println(distanceSensor_2.getI2CAddress(), HEX);


  digitalWrite(VL53L1X_3, HIGH);
  delay(150);
  if (distanceSensor_3.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor 3 failed to begin. Please check wiring. Freezing...");
    while (1);
  }
  Serial.print("Sensor 3 address : 0x");
  Serial.println(distanceSensor_3.getI2CAddress(), HEX);
  delay(100);
  distanceSensor_3.setI2CAddress(0x44);
  //digitalWrite(VL53L1X_3, LOW);
  delay(100);
  Serial.print("Sensor 3 address : 0x");
  Serial.println(distanceSensor_3.getI2CAddress(), HEX);

  /*
    digitalWrite(VL53L1X_4, HIGH);
    delay(150);
    if (distanceSensor_4.begin() != 0) //Begin returns 0 on a good init
    {
      Serial.println("Sensor 4 failed to begin. Please check wiring. Freezing...");
      while (1);
    }
    Serial.print("Sensor 4 address : 0x");
    Serial.println(distanceSensor_4.getI2CAddress(), HEX);
    delay(100);
    distanceSensor_4.setI2CAddress(0x88);
    delay(100);
    Serial.print("Sensor 4 address : 0x");
    Serial.println(distanceSensor_4.getI2CAddress(), HEX);
  */
  VL53L1X_config(distanceSensor_1); // if we don't start with Person Counter we set a short distance configuration for TOF
  VL53L1X_config(distanceSensor_2); // if we don't start with Person Counter we set a short distance configuration for TOF
  VL53L1X_config(distanceSensor_3); // if we don't start with Person Counter we set a short distance configuration for TOF
  // VL53L1X_config(distanceSensor_4); // if we don't start with Person Counter we set a short distance configuration for TOF
  Serial.println("Start program");
}

void loop() {
  // 1 sensor : around 50Hz
  // 2 sensor : around 90Hz
  // 4 sensors : around 160Hz

  int cpt_while = 0;
  int tab[4][NB_MESURE];
  while (cpt_while < NB_MESURE) {
    detect_4_sensors(tab, cpt_while); // General detection for TOF on the entire grid at short range
    cpt_while++;
  }
  Serial.println("---------------------");
  int moy[4];
  for (int i = 0; i < 4; i++) {
    moy[i] = 0;
    for (int j = 0; j < NB_MESURE; j++) {
      moy[i] += tab[i][j];
    }
  }
  Serial.print("   "); Serial.print(moy[2] / NB_MESURE);
  Serial.print("   "); Serial.println(moy[3] / NB_MESURE);
  Serial.print("   "); Serial.print(moy[0] / NB_MESURE);
  Serial.print("   "); Serial.println(moy[1] / NB_MESURE);
}


/*
   Check the entire grid of the sensor. If it detect a value inside the defined range
   Return 1 if something is detected
*/
int detect_4_sensors(int tab[4][NB_MESURE], int cpt) {
  int range = 250;
  int distance_1 = MAX_DISTANCE_SHORT_DETECTION;
  int distance_2 = MAX_DISTANCE_SHORT_DETECTION;
  int distance_3 = MAX_DISTANCE_SHORT_DETECTION;
  int distance_4 = MAX_DISTANCE_SHORT_DETECTION;

  distanceSensor_1.setROI(16, 16, 198);
  distanceSensor_2.setROI(16, 16, 198);
  distanceSensor_3.setROI(16, 16, 198);
  //distanceSensor_4.setROI(16, 16, 198);

  distanceSensor_1.startRanging(); //Write configuration bytes to initiate measurement
  distanceSensor_2.startRanging(); //Write configuration bytes to initiate measurement
  distanceSensor_3.startRanging(); //Write configuration bytes to initiate measurement
  //distanceSensor_4.startRanging(); //Write configuration bytes to initiate measurement

  while (!distanceSensor_1.checkForDataReady()) {
    delay(1);
  }
  while (!distanceSensor_2.checkForDataReady()) {
    delay(1);
  }
  while (!distanceSensor_3.checkForDataReady()) {
    delay(1);
  }
  /* while (!distanceSensor_4.checkForDataReady()) {
     delay(1);
    }*/

  tab[0][cpt] = distanceSensor_1.getDistance() / 10; //Get the result of the measurement from the sensor
  tab[1][cpt] = distanceSensor_2.getDistance() / 10; //Get the result of the measurement from the sensor
  tab[2][cpt] = distanceSensor_3.getDistance() / 10; //Get the result of the measurement from the sensor
  //tab[3][cpt] = distanceSensor_4.getDistance() / 10; //Get the result of the measurement from the sensor

  if (distance_1 >= MAX_DISTANCE_SHORT_DETECTION) {
    distance_1 = MAX_DISTANCE_SHORT_DETECTION;
  }
  distanceSensor_1.clearInterrupt();
  distanceSensor_1.stopRanging();
  distanceSensor_2.clearInterrupt();
  distanceSensor_2.stopRanging();
  distanceSensor_3.clearInterrupt();
  distanceSensor_3.stopRanging();
  //distanceSensor_4.clearInterrupt();
  //distanceSensor_4.stopRanging();

  if (distance_1 < range) {
    return 1;
  }
  else {
    return 0;
  }
}

void VL53L1X_config(SFEVL53L1X distanceSensor) {
  int read_delay = 20; //ms
  //distanceSensor.setDistanceThreshold(300, 400,3);
  //distanceSensor.setI2CAddress(0x52); // default 0xEACC
  // For fast ranging VL53L1_SetPresetMode (Dev, VL53L1_PRESETMODE_LITE_RANGING)
  //document https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html#resource
  distanceSensor.setTimingBudgetInMs(read_delay); // test 10 ?
  distanceSensor.setDistanceModeShort(); // should be short to be faster?
  distanceSensor.setIntermeasurementPeriod(read_delay);
}
