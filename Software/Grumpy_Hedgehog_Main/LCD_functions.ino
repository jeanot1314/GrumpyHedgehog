
  //HH_eyes_blink();
  //HH_sad();
  //HH_happy();

void HH_TFT_init(){
  // Use this initializer if using a 1.8" TFT screen:
  //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  // OR use this initializer if using a 1.8" TFT screen with offset such as WaveShare:
  //SENDtft.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab

  // OR use this initializer (uncomment) if using a 1.44" TFT:
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // OR use this initializer (uncomment) if using a 0.96" 160x80 TFT:
  //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display

  // OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  //tft.init(240, 240);           // Init ST7789 240x240

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
  //eyes_picture(false,0,0);
  //nose_picture(true,0,0);
  //eyes_picture(true,0,0);
  
  //HH_eyes(true, 0,0);
  //nose_picture(true);
  //HH_open_mouth();
  //delay(20000);
  HH_wake_up();
}

void HH_speak_mode(String val){ // write on the entire bottom line of the screen
  tft.setCursor(5, 7*tft.height()/8-2);
  tft.fillRect(0, 7*tft.height()/8-2, tft.width(), (tft.height()/8)-2, ST77XX_BLACK);
  tft.setTextColor(SPEECH_COLOR);  
  tft.setTextSize(2);
  tft.println(val);
  flag_text_timeout=1;
}

void HH_speak_counter(int cpt){ // write on the entire bottom line of the screen
  String val;
  switch(cpt){
    case 0:val = "PERSON  0";break;
    case 1:val = "PERSON  1";break;
    case 2:val = "PERSON  2";break;
    case 3:val = "PERSON  3";break;
    case 4:val = "PERSON  4";break;
    case 5:val = "PERSON  5";break;
    case 6:val = "PERSON  6";break;
    case 7:val = "PERSON  7";break;
    case 8:val = "PERSON  8";break;
    case 9:val = "PERSON  9";break;
    default:val = "PERSON  +";
  }

  tft.setCursor(5, 7*tft.height()/8-2);
  tft.fillRect(0, 7*tft.height()/8-2, tft.width(), (tft.height()/8)-2, ST77XX_BLACK);
  tft.setTextColor(SPEECH_COLOR);  
  tft.setTextSize(2);
  tft.println(val);
  flag_text_timeout=1;
}

void HH_speak(String val){ // write large in the middle of the screen
  String secret = "SECRET";
  tft.setCursor(10, 1*tft.height()/8);
  tft.setTextColor(SPEECH_COLOR);  
  tft.setTextSize(3);
  tft.println(secret);
  String patern = "PATERN";
  tft.setCursor(10, 3*tft.height()/8);
  tft.setTextColor(SPEECH_COLOR);  
  tft.setTextSize(3);
  tft.println(patern);
  tft.setCursor((tft.width()/2)-10, 5*tft.height()/8);
  tft.setTextColor(SPEECH_COLOR);  
  tft.setTextSize(5);
  tft.println(val);
}

void HH_wake_up(){
  
  HH_eyes_close(true);
  HH_nose(true,0,0);
  delay(2000);
  HH_eyes(true, 0,0);
  flag_text_timeout=1; 
}

void HH_original_state(){
  HH_nose(true,0,0);
  HH_eyes(true, 0,0);
}

void HH_look(int horizontal, int vertical){
  int speed_delay=800;
  for(int i=0;i<1;i++){
    HH_eyes(false, 0,0);
    HH_nose(false, 0,0);
    HH_eyes(true, horizontal/2, vertical/2);
    HH_nose(true, horizontal/2, vertical/2);
    delay(50);
    HH_eyes(false, horizontal/2, vertical/2);
    HH_nose(false, horizontal/2, vertical/2);
    HH_eyes(true, horizontal, vertical );
    HH_nose(true, horizontal, vertical);
    delay(speed_delay); 
    HH_eyes(false,  horizontal, vertical);
    HH_nose(false, horizontal, vertical);
    HH_eyes(true, horizontal/2, vertical/2);
    HH_nose(true, horizontal/2, vertical/2);
    delay(50);
    HH_eyes(false,  horizontal/2, vertical/2);
    HH_nose(false, horizontal/2, vertical/2);
    HH_eyes(true, 0,0);
    HH_nose(true, 0,0);
  }
}

void HH_look_pictures(int horizontal, int vertical){
  int speed_delay=1000;
  int inter_speed_delay=10;
  
  eyes_picture(false, 0,0);
  HH_nose(false, 0,0);
  eyes_picture(true, horizontal/3, vertical/3);
  HH_nose(true, horizontal/3, vertical/3);
  delay(inter_speed_delay);
  eyes_picture(false, horizontal/3, vertical/3);
  HH_nose(false, horizontal/3, vertical/3);
  eyes_picture(true, 2*horizontal/3, 2*vertical/3);
  HH_nose(true, 2*horizontal/3, 2*vertical/3);
  delay(inter_speed_delay);
  eyes_picture(false, 2*horizontal/3, 2*vertical/3);
  HH_nose(false, 2*horizontal/3, 2*vertical/3);
  eyes_picture(true, horizontal, vertical );
  HH_nose(true, horizontal, vertical);
  delay(speed_delay); 
  eyes_picture(false,  horizontal, vertical);
  HH_nose(false, horizontal, vertical);
  eyes_picture(true, 2*horizontal/3, 2*vertical/3);
  HH_nose(true, 2*horizontal/3, 2*vertical/3);
  delay(inter_speed_delay);
  eyes_picture(false, 2*horizontal/3, 2*vertical/3);
  HH_nose(false, 2*horizontal/3, 2*vertical/3);
  eyes_picture(true, horizontal/3, vertical/3);
  HH_nose(true, horizontal/3, vertical/3);
  delay(inter_speed_delay);
  eyes_picture(false, horizontal/3, vertical/3);
  HH_nose(false, horizontal/3, vertical/3);
  eyes_picture(true, 0, 0);
  HH_nose(true, 0, 0);
}

void HH_lookFromTo(int horizontal_from, int vertical_from, int horizontal_to, int vertical_to){
  int inter_speed_delay=10;
  HH_eyes(false, horizontal_from,vertical_from);
  HH_nose(false, horizontal_from,vertical_from);
  HH_eyes(true, 2*horizontal_from/3+1*horizontal_to/3, 2*vertical_from/3+1*vertical_to/3);
  HH_nose(true, 2*horizontal_from/3+1*horizontal_to/3, 2*vertical_from/3+1*vertical_to/3);
  delay(inter_speed_delay);
  HH_eyes(false, 2*horizontal_from/3+1*horizontal_to/3, 2*vertical_from/3+1*vertical_to/3);
  HH_nose(false, 2*horizontal_from/3+1*horizontal_to/3, 2*vertical_from/3+1*vertical_to/3);
  HH_eyes(true, 1*horizontal_from/3+2*horizontal_to/3, 1*vertical_from/3+2*vertical_to/3);
  HH_nose(true, 1*horizontal_from/3+2*horizontal_to/3, 1*vertical_from/3+2*vertical_to/3);
  delay(inter_speed_delay);
  HH_eyes(false, 1*horizontal_from/3+2*horizontal_to/3, 1*vertical_from/3+2*vertical_to/3);
  HH_nose(false, 1*horizontal_from/3+2*horizontal_to/3, 1*vertical_from/3+2*vertical_to/3);
  HH_eyes(true, horizontal_to, vertical_to);
  HH_nose(true, horizontal_to, vertical_to);
}

void HH_eyes_blink(){
  int speed_delay=200;
  HH_eyes(false,0,0);
  HH_eyes_close(true);
  delay(speed_delay); 
  HH_eyes_close(false);
  HH_eyes(true,0,0);
}

void HH_open_mouth(){
  int speed_delay=200;
  for(int i=0;i<6;i++){
    HH_nose_mouth(0, 0, (2*i)-1);
    HH_nose_mouth(1, 0, (2*i));
    delay(speed_delay);
  }
  delay(500);
  for(int i=5;i>=0;i--){
    HH_nose_mouth(0, 0, (2*i)+1);
    HH_nose_mouth(1, 0, (2*i));
    delay(speed_delay);
  }
}

void HH_happy(){
  int speed_delay=2000;
    HH_eyes_happy();
}

void HH_sad(){
  int speed_delay=4000;
  for(int i=0;i<1;i++){
    HH_eyes(true,0,0);
    HH_eyes_sad(14);
    delay(100);
    HH_eyes_sad(12);
    delay(speed_delay); 
    HH_eyes(true,0,0);
  }
}

void HH_eyes(bool present, int horizontal, int vertical){
  if(present==1){

    //tft.fillTriangle(EYES_1_HORIZONTAL_POSITION+horizontal-20, EYES_VERTICAL_POSITION+vertical-15,   EYES_1_HORIZONTAL_POSITION+horizontal+17, EYES_VERTICAL_POSITION+vertical-10,  EYES_1_HORIZONTAL_POSITION+horizontal+5, EYES_VERTICAL_POSITION+vertical-20, NOSE_COLOR);
    //tft.fillTriangle(EYES_2_HORIZONTAL_POSITION+horizontal+20, EYES_VERTICAL_POSITION+vertical-15,   EYES_2_HORIZONTAL_POSITION+horizontal-17, EYES_VERTICAL_POSITION+vertical-10,  EYES_2_HORIZONTAL_POSITION+horizontal-5, EYES_VERTICAL_POSITION+vertical-20, NOSE_COLOR);
    
    //tft.fillCircle(EYES_1_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE+4, NOSE_COLOR);
    //tft.fillCircle(EYES_2_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE+4, NOSE_COLOR);
    tft.fillCircle(EYES_1_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE, EYES_COLOR);
    tft.fillCircle(EYES_2_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE, EYES_COLOR);

    //tft.fillCircle(EYES_1_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, 3, ST77XX_BLACK);
    //tft.fillCircle(EYES_2_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, 3, ST77XX_BLACK);
    //if(horizontal || vertical){
    //  tft.fillCircle(EYES_1_HORIZONTAL_POSITION+horizontal/5, EYES_VERTICAL_POSITION+vertical/5, EYES_SIZE, ST77XX_BLACK);
    //  tft.fillCircle(EYES_2_HORIZONTAL_POSITION+horizontal/5, EYES_VERTICAL_POSITION+vertical/5, EYES_SIZE, ST77XX_BLACK);
    //}
  }
  else{
    tft.fillCircle(EYES_1_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE, ST77XX_BLACK);
    tft.fillCircle(EYES_2_HORIZONTAL_POSITION+horizontal, EYES_VERTICAL_POSITION+vertical, EYES_SIZE, ST77XX_BLACK);    
  }  
}

void HH_eyes_close(bool present){
  if(present==1){
    tft.fillRoundRect(EYES_1_HORIZONTAL_POSITION-EYES_SIZE, EYES_VERTICAL_POSITION-EYES_SIZE/4, EYES_SIZE*2, EYES_SIZE/2,1, EYES_COLOR);
    tft.fillRoundRect(EYES_2_HORIZONTAL_POSITION-EYES_SIZE, EYES_VERTICAL_POSITION-EYES_SIZE/4, EYES_SIZE*2, EYES_SIZE/2,1, EYES_COLOR);
  }
  else{
    tft.fillRoundRect(EYES_1_HORIZONTAL_POSITION-EYES_SIZE, EYES_VERTICAL_POSITION-EYES_SIZE/4, EYES_SIZE*2, EYES_SIZE/2,1, ST77XX_BLACK);
    tft.fillRoundRect(EYES_2_HORIZONTAL_POSITION-EYES_SIZE, EYES_VERTICAL_POSITION-EYES_SIZE/4, EYES_SIZE*2, EYES_SIZE/2,1, ST77XX_BLACK); 
  }
}

void HH_eyes_happy(){
  HH_eyes(true,0,0);
  tft.fillCircle(EYES_1_HORIZONTAL_POSITION+3, EYES_VERTICAL_POSITION+7, EYES_SIZE, ST77XX_BLACK);
  tft.fillCircle(EYES_2_HORIZONTAL_POSITION-3, EYES_VERTICAL_POSITION+7, EYES_SIZE, ST77XX_BLACK);
}

void HH_eyes_sad(int degree){

  tft.fillCircle(EYES_1_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION-degree, EYES_SIZE+2, ST77XX_BLACK);
  tft.fillCircle(EYES_2_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION-degree, EYES_SIZE+2, ST77XX_BLACK);
  tft.fillCircle(EYES_1_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION+degree+4, EYES_SIZE+2, ST77XX_BLACK);
  tft.fillCircle(EYES_2_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION+degree+4, EYES_SIZE+2, ST77XX_BLACK);
}

void HH_eyes_sleep(){
  HH_eyes(true,0,0);
  tft.fillCircle(EYES_1_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION-4, EYES_SIZE, ST77XX_BLACK);
  tft.fillCircle(EYES_2_HORIZONTAL_POSITION, EYES_VERTICAL_POSITION-4, EYES_SIZE, ST77XX_BLACK);
}

void HH_nose(bool present, int horizontal, int vertical){

  if(present==1){
    //tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION+24+3*vertical/2, 9, NOSE_COLOR);
    //tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION+23+3*vertical/2, 6, 0xC000);
    
    tft.fillCircle(tft.width()/2+horizontal/2,NOSE_VERTICAL_POSITION+vertical/4 , NOSE_SIZE+3, NOSE_COLOR);

    tft.fillCircle(tft.width()/2+horizontal-5, NOSE_VERTICAL_POSITION-5+3*vertical/2, 4, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION-5+3*vertical/2, 5, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2+horizontal+5, NOSE_VERTICAL_POSITION-5+3*vertical/2, 4, ST77XX_BLACK);
  }
  else{
    //tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION+24+3*vertical/2, 9, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2+horizontal/2, NOSE_VERTICAL_POSITION+vertical/4, NOSE_SIZE+3, ST77XX_BLACK);
  }  
}

void HH_nose_mouth(bool present, int horizontal, int vertical){

  if(present==1){
    tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION+14+vertical, 15, NOSE_COLOR);
    tft.fillCircle(tft.width()/2+horizontal, NOSE_VERTICAL_POSITION+13+vertical, 13, 0xC000);
    
    tft.fillCircle(tft.width()/2,NOSE_VERTICAL_POSITION , NOSE_SIZE+3, NOSE_COLOR);

    tft.fillCircle(tft.width()/2-5, NOSE_VERTICAL_POSITION-5, 4, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2, NOSE_VERTICAL_POSITION-5, 5, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2+5, NOSE_VERTICAL_POSITION-5, 4, ST77XX_BLACK);
  }
  else{
    tft.fillCircle(tft.width()/2, NOSE_VERTICAL_POSITION+24, 15, ST77XX_BLACK);
    tft.fillCircle(tft.width()/2, NOSE_VERTICAL_POSITION, NOSE_SIZE+3, ST77XX_BLACK);
  }  
}

void nose_picture_bear(bool visible){
  if(visible){
    tft.fillRect(3*(tft.width())/10+49,40,20,61, ST77XX_BLACK);
    tft.drawRGBBitmap(3*(tft.width())/10,40,nose,49,63);
  }
  else{tft.fillRect(3*(tft.width())/10,40,49,63, ST77XX_BLACK);}
}

void eyes_picture(bool present, int horizontal, int vertical){
  if(present){
    tft.drawRGBBitmap(tft.width()/4-7+horizontal,tft.height()/4+vertical,eyes_new,15,15);
    tft.drawRGBBitmap(3*tft.width()/4-7+horizontal,tft.height()/4+vertical,eyes_new,15,15);
  }
  else{
    tft.fillCircle(1*(tft.width())/4+horizontal,tft.height()/4+vertical+7,8, ST77XX_BLACK);
    tft.fillCircle(3*(tft.width())/4+horizontal,tft.height()/4+vertical+7,8, ST77XX_BLACK);
  }
}

void nose_picture(bool present, int horizontal, int vertical){
  if(present){
    tft.drawRGBBitmap(tft.width()/2+horizontal-26,tft.height()/2-24+vertical,nose_new,56,66);
  }
  else{
    tft.fillRect(tft.width()/2+horizontal+22,tft.height()/2+vertical-22,56,66, ST77XX_BLACK);
  }
}
