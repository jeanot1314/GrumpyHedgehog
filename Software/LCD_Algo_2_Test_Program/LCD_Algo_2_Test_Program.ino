
#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define TFT_CS  8
#define TFT_RST 10 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC  9

// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
// For 1.14", 1.3", 1.54", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);


void setup() {

  Serial.begin(9600);
  delay(100);
  Serial.println("Starting Program");
  delay(100);

  // Use this initializer if using a 1.8" TFT screen:
  //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  // OR use this initializer if using a 1.8" TFT screen with offset such as WaveShare:
  //SENDtft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab

  // OR use this initializer (uncomment) if using a 1.44" TFT:
  //  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // OR use this initializer (uncomment) if using a 0.96" 160x80 TFT:
  //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display

  // OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  SPI.setMOSI(11);
  SPI.setSCK(13);
  tft.init(240, 240, SPI_MODE2);           // Init ST7789 240x240

  // OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
  //tft.init(240, 320);           // Init ST7789 320x240

  // OR use this initializer (uncomment) if using a 1.14" 240x135 TFT:
  //tft.init(135, 240);           // Init ST7789 240x135

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(40000000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setRotation(2);

}

void loop() {
  //tft.drawRGBBitmap(0, 0, hackaday, 240, 240);
  Serial.println("Loop");
  int sp = 15;
  int ball_size = 20;
  int ball_color = 0x8200;
  int slowing = 50;
  for (int i = ball_size; i <= tft.width() - ball_size; i = i + sp) {
    for (int k = 0; k < sp; k++) {
      tft.drawCircleHelper(i + k, ball_size, ball_size, 1,  ST77XX_BLACK);
      tft.drawCircleHelper(i + k, ball_size, ball_size, 8,  ST77XX_BLACK);
      tft.drawPixel(i + k - ball_size, ball_size, ST77XX_BLACK);
    }
    tft.fillCircle(i + sp, ball_size, ball_size, ball_color);
    delay(slowing);
  }

  for (int j = ball_size; j <= tft.width() - ball_size; j = j + sp) {
    for (int k = 0; k < sp; k++) {
      tft.drawCircleHelper(tft.width() - ball_size, j + k, ball_size, 1,  ST77XX_BLACK);
      tft.drawCircleHelper(tft.width() - ball_size, j + k, ball_size, 2,  ST77XX_BLACK);
      tft.drawPixel(tft.width() - ball_size, j + k - ball_size, ST77XX_BLACK);
    }
    tft.fillCircle(tft.width() - ball_size, j + sp, ball_size, ball_color);
    delay(slowing);
  }

  for (int i = tft.width() - ball_size; i >= ball_size; i = i - sp) {
    for (int k = 0; k < sp; k++) {
      tft.drawCircleHelper(i - k, tft.width() - ball_size, ball_size, 2,  ST77XX_BLACK);
      tft.drawCircleHelper(i - k, tft.width() - ball_size, ball_size, 4,  ST77XX_BLACK);
      tft.drawPixel(i - k + ball_size, tft.width() - ball_size, ST77XX_BLACK);
    }
    tft.fillCircle(i - sp, tft.width() - ball_size, ball_size, ball_color);
    delay(slowing);
  }
  for (int j = tft.width() - ball_size; j >= ball_size; j = j - sp) {
    for (int k = 0; k < sp; k++) {
      tft.drawCircleHelper(ball_size, j - k, ball_size, 4,  ST77XX_BLACK);
      tft.drawCircleHelper(ball_size, j - k, ball_size, 8,  ST77XX_BLACK);
      tft.drawPixel(ball_size, j - k + ball_size, ST77XX_BLACK);
    }
    tft.fillCircle(ball_size, j - sp, ball_size, ball_color);
    delay(slowing);
  }
  //tft.fillScreen(ST77XX_BLACK);
}
