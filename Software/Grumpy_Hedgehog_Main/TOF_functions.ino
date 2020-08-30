

void VL53L1X_info() {
  VL53L1X_Version_t Version = distanceSensor.getSoftwareVersion();
  Serial.print("Software Version : 0x");
  Serial.print((Version.major), HEX); Serial.print(" 0x");
  Serial.print((Version.minor), HEX); Serial.print(" 0x");
  Serial.print((Version.build), HEX); Serial.print(" 0x");
  Serial.println(Version.revision, HEX);
  Serial.print("Sensor ID : 0x"); Serial.println(distanceSensor.checkID(), HEX);
  Serial.print("I2C address : 0x"); Serial.println(distanceSensor.getI2CAddress(), HEX);
  Serial.print("Timing Budget mode : "); Serial.println(distanceSensor.getTimingBudgetInMs());
  Serial.print("Interrupt polarity : "); Serial.println(distanceSensor.getInterruptPolarity());
  //Serial.print("Distance mode : ");Serial.println(distanceSensor.getDistanceMode());
  //Serial.print("Intermeasurement mode : ");Serial.println(distanceSensor.getIntermeasurementPeriod());
  //Serial.print("getOffset mode : ");Serial.println(distanceSensor.getOffset());
  //Serial.print("getXTalk mode : ");Serial.println(distanceSensor.getXTalk());
  //Serial.print("ThresholdWindow mode : ");Serial.println(distanceSensor.getDistanceThresholdWindow()); //--- Returns distance threshold window option.
  //Serial.print("ThresholdLow : ");Serial.println(distanceSensor.getDistanceThresholdLow()); //--- Returns lower bound in mm.
  //Serial.print("ThresholdHigh : ");Serial.println(distanceSensor.getDistanceThresholdHigh());
}

void VL53L1X_config() {
  int read_delay = 20; //ms
  //distanceSensor.setDistanceThreshold(300, 400,3);
  //distanceSensor.setI2CAddress(0x52); // default 0xEACC
  // For fast ranging VL53L1_SetPresetMode (Dev, VL53L1_PRESETMODE_LITE_RANGING)
  //document https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html#resource
  distanceSensor.setTimingBudgetInMs(read_delay); // test 10 ?
  distanceSensor.setDistanceModeShort(); // should be short to be faster?
  distanceSensor.setIntermeasurementPeriod(read_delay);
}

void VL53L1X_config_long() {
  int read_delay = 100; //ms
  //distanceSensor.setDistanceThreshold(300, 400,3);
  //distanceSensor.setI2CAddress(0x52); // default 0xEACC
  // For fast ranging VL53L1_SetPresetMode (Dev, VL53L1_PRESETMODE_LITE_RANGING)
  //document https://www.st.com/en/imaging-and-photonics-solutions/vl53l1x.html#resource
  distanceSensor.setTimingBudgetInMs(read_delay); // test 10 ?
  distanceSensor.setDistanceModeLong(); // should be short to be faster?
  distanceSensor.setIntermeasurementPeriod(read_delay);
}

/*
   Read sensor and print patern on the LCD
   If the sensor is saturated with an object for few seconds, it leaves the function
*/
int detect_patern(int nb_lines, int nb_colones) {
  int cpt_match_patern_1 = 0;
  int cpt_match_patern_2 = 0;
  int cpt_match_patern_3 = 0;

  int exit_flag_detection_range = 80;
  int exit_flag_detection = 0;

  while (exit_flag_detection < 3) {
    int exit_flag_detection_cpt = 0;
    int range = 0xFF;
    //int distanceAll=512;

    if (nb_lines <= 0) {
      nb_lines = 1;
    }
    if (nb_colones <= 0) {
      nb_colones = 1;
    }

    int nb_ROI = nb_colones * nb_lines;
    int data[nb_lines][nb_colones];

    for (int i = 0; i < nb_lines; i++) {
      for (int j = 0; j < nb_colones; j++) {
        data[i][j] = 0;
        distanceSensor.setROI(MATRICE_DIMENSION / (nb_colones), MATRICE_DIMENSION / (nb_lines), opticalCalcul(i, j, nb_lines, nb_colones));
        distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
        while (!distanceSensor.checkForDataReady()) {
          delay(1);
        }
        int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
        //Serial.print("< ");Serial.print(distance);Serial.print(" >");
        if (distance >= range) {
          distance = range;
        }
        else if (distance <= exit_flag_detection_range) {
          exit_flag_detection_cpt++;
        }

        int tmp = map(distance, 0, range, 0x1F, 0); // Encoded on 5 bit
        data[i][j] |= (tmp << 5) + tmp; // Color encoded in 565, with green and blue
        //data_4[i*nb_colones+j] = distance;
        distanceSensor.clearInterrupt();
        distanceSensor.stopRanging();
      }
    }

    int match_patern_1 = 0;
    int match_patern_2 = 0;
    int match_patern_3 = 0;

    for (int i = 0; i < nb_lines; i++) {
      for (int j = 0; j < nb_colones; j++) {
        if (data[i][j] != 0 && patern_1[i][j] == 1) {
          match_patern_1++;
        }
        else if (data[i][j] == 0 && patern_1[i][j] == 0) {
          match_patern_1++;
        }

        if (data[i][j] != 0 && patern_2[i][j] == 1) {
          match_patern_2++;
        }
        else if (data[i][j] == 0 && patern_2[i][j] == 0) {
          match_patern_2++;
        }

        if (data[i][j] != 0 && patern_3[i][j] == 1) {
          match_patern_3++;
        }
        else if (data[i][j] == 0 && patern_3[i][j] == 0) {
          match_patern_3++;
        }
      }
    }
    //Serial.print(match_patern_1);Serial.print("---");Serial.println(cpt_match_patern_1);
    if (match_patern_1 == 16) {
      cpt_match_patern_1++;
    }
    else if (match_patern_2 == 16) {
      cpt_match_patern_2++;
    }
    else if (match_patern_3 == 16) {
      cpt_match_patern_3++;
    }
    else {
      cpt_match_patern_1 = 0;
      cpt_match_patern_2 = 0;
    }

    for (int i = 0; i < nb_lines; i++) {
      for (int j = 0; j < nb_colones; j++) {
        //Serial.write(data[i]);//Serial.print("-");
        if (cpt_match_patern_1 < 4 && cpt_match_patern_2 < 4 && cpt_match_patern_3 < 4) {
          tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, data[i][j]);
        }
        else {
          if (cpt_match_patern_1 >= 4) {
            if (patern_1[i][j] > 0) {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_ON);
            }
            else {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_OFF);
            }
          }
          if (cpt_match_patern_2 >= 4) {
            if (patern_2[i][j] > 0) {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_ON);
            }
            else {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_OFF);
            }
          }
          if (cpt_match_patern_3 >= 4) {
            if (patern_3[i][j] > 0) {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_ON);
            }
            else {
              tft.fillRect(j * tft.width() / nb_colones, i * (tft.height()) / nb_lines, tft.width() / nb_colones, (tft.height()) / nb_lines, PATERN_OFF);
            }
          }
        }
      }
    }
    if (cpt_match_patern_1 >= 4) {
      cpt_match_patern_1 = 0;
      HH_speak("1");
      patern_1_secret_action();
      delay(4000);
      if (detect_All() == 0) {
        return 1;
      }
    }
    else if (cpt_match_patern_2 >= 4) {
      cpt_match_patern_2 = 0;
      HH_speak("2");
      patern_2_secret_action();
      delay(4000);
      if (detect_All() == 0) {
        return 2;
      }
    }
    else if (cpt_match_patern_3 >= 4) {
      cpt_match_patern_3 = 0;
      HH_speak("3");
      patern_3_secret_action();
      delay(4000);
      if (detect_All() == 0) {
        return 3;
      }
    }

    if (exit_flag_detection_cpt == 16) {
      exit_flag_detection++;
      Serial.print("flag detection:"); Serial.println(exit_flag_detection);
    }
    else {
      exit_flag_detection = 0;
    }
  }
  return 0;
}


int look_around() {
  int nb_colones = 4;
  int nb_lines = 4;
  int exit_flag_detection_range = 80;
  int exit_flag_detection = 0;
  int lastvalueX=0;
  int lastvalueY=0;

  while (exit_flag_detection < 3) {
    
    int major_positionX = -1;
    int major_positionY = -1;
    int major_positionLast = 0xFF;
    int exit_flag_detection_cpt = 0;
    int range = 0xFF;
    //int distanceAll=512;

    if (nb_lines <= 0) {
      nb_lines = 1;
    }
    if (nb_colones <= 0) {
      nb_colones = 1;
    }

    int nb_ROI = nb_colones * nb_lines;
    int data=0;
    int cpt_data=0;
    
    for (int i = 0; i < nb_lines; i++) {
      for (int j = 0; j < nb_colones; j++) {
        if((i==0 || i==3) || (j==0 || j==3)){
          
          distanceSensor.setROI(MATRICE_DIMENSION / (nb_colones), MATRICE_DIMENSION / (nb_lines), opticalCalcul(i, j, nb_lines, nb_colones));
          distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
          while (!distanceSensor.checkForDataReady()) {
            delay(1);
          }
          int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
          
          if (distance >= range) {
            distance = range;
          }
          else if (distance <= exit_flag_detection_range) {
            exit_flag_detection_cpt++;
          }

          if (distance < range) {
            data |= 1<<cpt_data;
          }
  
          if (major_positionLast > distance && (i==0 || i==3) && (j==0 || j==3)) {
            //Serial.print("(");Serial.print(i);Serial.print(j);Serial.print(")");Serial.print("< ");Serial.print(distance);Serial.print(" >");
            //Serial.println();
            major_positionX = i;
            major_positionY = j;
            major_positionLast = distance;
          }
          distanceSensor.clearInterrupt();
          distanceSensor.stopRanging();
          if(cpt_data<12){cpt_data++;}
        }
      }
    }
    // 1248 // 1 caractere
    // 1  2 // 2 caractere
    // 4  8 // 2 caractere
    // 1248 // 3 caractere
    //Serial.print("DATA=");Serial.print(data,HEX);Serial.print(" --- ");Serial.print(data, BIN);Serial.print("---");
    int newvalueY;
    int newvalueX;
    
    if(data == 0x001 || data == 0x003 || data == 0x011 || data == 0x013 || data == 0x017 || data == 0x053 || data == 0x057){// || data == 0x017 || data == 0x053 || data == 0x057){
      Serial.println("TOP LEFT");// Top left
      newvalueY=-10;newvalueX=-10;
    }
    else if(data == 0x100 || data == 0x300 || data == 0x140 || data == 0x340 || data == 0x350 || data == 0x740 || data == 0x750){
      Serial.println("BOT LEFT");// Bot left
      newvalueY=-10;newvalueX=10;
    }
    else if(data == 0x008 || data == 0x00C || data == 0x028 || data == 0x02C || data == 0x02E || data == 0x0AC || data == 0x0AE){
      Serial.println("TOP RIGHT");// Top right
      newvalueY=10;newvalueX=-10;
    }
    else if(data == 0x800 || data == 0x880 || data == 0xC00 || data == 0xC80 || data == 0xE80 || data == 0xCA0 || data == 0xEA0){
      Serial.println("BOT RIGHT");// Bot right
      newvalueY=10;newvalueX=10;
    }
    else if(data == 0x00F || data == 0x02F || data == 0x01F || data == 0x03F){
      Serial.println("TOP");// top
      newvalueY=0;newvalueX=-10;
    }
    else if(data == 0x151 || data == 0x153 || data == 0x351 || data == 0x353){
      Serial.println("LEFT");// left
      newvalueY=-10;newvalueX=0;
    }
    else if(data == 0xF00 || data == 0xF40 || data == 0xF80 || data == 0xFC0){
      Serial.println("BOT");// bot
      newvalueY=0;newvalueX=10;
    }
    else if(data == 0x8A8 || data == 0x8AC || data == 0xCA8 || data == 0xCAC){
      Serial.println("RIGHT");// right
      newvalueY=10;newvalueX=0;
    }
    else if(data == 0x0F0 || data == 0xFF0 || data == 0x0FF || data == 0xFFF){
      Serial.println("CENTER");// right
      newvalueY=0;newvalueX=0;
    }
    // C84 
    else{
      Serial.print("--->");Serial.println(data,HEX);
      //newvalueY=0;newvalueX=0;
    }
    
    //int newvalueY = (major_positionY-2)*5;
    //int newvalueX = (major_positionX-2)*5;
    
    if (major_positionLast == 0xFF) {
      newvalueY=0;
      newvalueX=0;
    }
    if(lastvalueY != newvalueY || lastvalueX != newvalueX) {
      HH_lookFromTo(lastvalueY, lastvalueX, newvalueY,newvalueX);
      lastvalueY = newvalueY;
      lastvalueX = newvalueX;
    }

    if (exit_flag_detection_cpt == 16) {
      exit_flag_detection++;
      Serial.print("flag detection:"); Serial.println(exit_flag_detection);
    }
    else {
      exit_flag_detection = 0;
    }
  }
  return 0;
}


/*
   Check the entire grid of the sensor. If it detect a value inside the defined range
   Return 1 if something is detected
*/
int detect_All() {
  int range = 250;
  int distanceAll = MAX_DISTANCE_SHORT_DETECTION;
  distanceSensor.setROI(16, 16, 198); 
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady()) {
    delay(1);
  }
  distanceAll = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  if (distanceAll >= MAX_DISTANCE_SHORT_DETECTION) {
    distanceAll = MAX_DISTANCE_SHORT_DETECTION;
  }
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  if (distanceAll < range) {
    return 1;
  }
  else {
    return 0;
  }
}

/*
   Check the entire grid of the sensor. If it detect a value inside the defined range
   Return 1 if something is detected
*/
int detect_All_longDistance() {
  int range = 2000;
  int distanceAll = MAX_DISTANCE_LONG_DETECTION;
  distanceSensor.setROI(16, 16, 198); 
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady()) {
    delay(1);
  }
  distanceAll = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  if (distanceAll >= MAX_DISTANCE_LONG_DETECTION) {
    distanceAll = MAX_DISTANCE_LONG_DETECTION;
  }
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();

  if (distanceAll < range) {
    return 1;
  }
  else {
    return 0;
  }
}

int detect_LeftRightUpStatic() { // this function detect 4 mouvements : Left, right, up, and static
  int range = 220;
  int directionLRUS = 0;
  int counter_static = 0;
  int static_timer = 10;
  //Serial.println("Enter Function : detect_LeftRightUpStatic");

  while (directionLRUS != CODE_RIGHT && directionLRUS != CODE_LEFT && directionLRUS != CODE_UP && directionLRUS != CODE_STATIC) {
    int distanceL = 511;
    distanceSensor.setROI(4, 4, 106); // left
    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    while (!distanceSensor.checkForDataReady()) {
      delayMicroseconds(10);
    }
    distanceL = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    if (distanceL >= MAX_DISTANCE_SHORT_DETECTION) {
      distanceL = MAX_DISTANCE_SHORT_DETECTION;
    }
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    int distanceR = 511;
    distanceSensor.setROI(4, 4, 10); // right
    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    while (!distanceSensor.checkForDataReady()) {
      delayMicroseconds(10);
    }
    distanceR = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    if (distanceR >= MAX_DISTANCE_SHORT_DETECTION) {
      distanceR = MAX_DISTANCE_SHORT_DETECTION;
    }
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    int distanceU = 511;
    distanceSensor.setROI(4, 4, 193); // left
    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    while (!distanceSensor.checkForDataReady()) {
      delayMicroseconds(10);
    }
    distanceU = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    if (distanceU >= MAX_DISTANCE_SHORT_DETECTION) {
      distanceU = MAX_DISTANCE_SHORT_DETECTION;
    }
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();

    // Détection en deux temps avec flag directionLRUS_tmp
    if (distanceR < range && distanceL < range && distanceU < range) {
      if (directionLRUS == 0) {
        directionLRUS = 1;
        Serial.print("DETECT ");
      }

      if (counter_static >= static_timer) {
        directionLRUS = CODE_STATIC; 
        //Serial.print("STATIC "); // change mode
      }
      else {
        counter_static++;
      }
    } // detect something
    else if (distanceR < range && distanceL > range && distanceU > range && directionLRUS == 1) {
      directionLRUS = CODE_RIGHT;
      Serial.print("RIGHT ");
    }
    else if (distanceR > range && distanceL < range && distanceU > range && directionLRUS == 1) {
      directionLRUS = CODE_LEFT;
      Serial.print("LEFT ");
    }
    else if (distanceR > range && distanceL > range && distanceU < range && directionLRUS == 1) {
      directionLRUS = CODE_UP;
      Serial.print("UP ");
    }
    else if (distanceR > range && distanceL > range && distanceU > range) {
      directionLRUS = 0;
      counter_static = 0;
    }

    if (directionLRUS > 0) {
      Serial.print("(");
      Serial.print(distanceL);
      Serial.print("-");
      Serial.print(distanceU);
      Serial.print("-");
      Serial.print(distanceR);
      Serial.print(")");
      Serial.println(counter_static);
    };

    if (distanceR > range && distanceL > range && distanceU > range) {
      return 0;
    }
  }
  return directionLRUS;
}

int detect_LeftRightStatic_longDistance() { // this function detect 3 mouvements : Left, right and static
  int range = 2000;
  int directionLRS = 0;
  int counter_static = 0;
  int range_static = 220;
  int static_timer = 10;

  while (directionLRS != CODE_RIGHT && directionLRS != CODE_LEFT && directionLRS != CODE_STATIC) {
    
    int distanceL = 0xFFFF;
    distanceSensor.setROI(4, 4, 106); // left
    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    while (!distanceSensor.checkForDataReady()) {
      delayMicroseconds(10);
    }
    distanceL = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    if (distanceL >= MAX_DISTANCE_LONG_DETECTION) {
      distanceL = MAX_DISTANCE_LONG_DETECTION;
    }
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();
    
    int distanceR = 0xFFFF;
    distanceSensor.setROI(4, 4, 10); // right
    distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
    while (!distanceSensor.checkForDataReady()) {
      delayMicroseconds(10);
    }
    distanceR = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
    if (distanceR >= MAX_DISTANCE_LONG_DETECTION) {
      distanceR = MAX_DISTANCE_LONG_DETECTION;
    }
    distanceSensor.clearInterrupt();
    distanceSensor.stopRanging();
    
    //Serial.print("DEBUG : ");Serial.print(distanceL);Serial.print(" - ");Serial.println(distanceR);
    // Détection en deux temps avec flag directionLRS_tmp
    if (distanceR < range_static && distanceL < range_static) {
      //if (directionLRS == 0) {
      //  directionLRS = 1;
      //  Serial.print("DETECT ");
      //}

      if (counter_static >= static_timer) {
        directionLRS = CODE_STATIC; Serial.print("STATIC "); // change mode
      }
      else {
        counter_static++;
      }
    } // detect something
    else if (distanceR < range && distanceL > range) {
      if(directionLRS==2){
        directionLRS = CODE_RIGHT;
      }
      else{
        directionLRS = 3;
      }
      
      //Serial.print("RIGHT ");
    }
    else if (distanceR > range && distanceL < range) {
      if(directionLRS==3){
        directionLRS = CODE_LEFT;
      }
      else{
        directionLRS = 2;
      }
    }
    else if (distanceR > range && distanceL > range) {
      directionLRS = 0;
      counter_static = 2;
    }

    if (directionLRS > 0) {
      Serial.print("(");
      Serial.print(distanceL);
      Serial.print("-");
      Serial.print(distanceR);
      Serial.print(")");
      Serial.print(counter_static);
      Serial.print("-");
      Serial.println(directionLRS);
    };

    if (distanceR > range && distanceL > range) {
      return 0;
    }
  }
  return directionLRS;
}

uint8_t opticalCalcul(int i, int j, int nb_lines, int nb_colones) {
  // 1 ROI = 199
  // 2 ROI = 167 - 231 (gauche droite)
  // 2 ROI = 195 - 60 (haut bas)
  // 4 ROI = 163 - 227 - 92 - 28
  // 16 ROI = 00=145, 01=177, 02=209, 03=241, 10=148, 11=181, 12=213, 13=245, 20=110, 21=78, 22=46, 23=14, 30=106, 31=74, 32=42, 33=10
  //Serial.print("Optical calcul ");Serial.print(i);Serial.print("  ");Serial.print(j);Serial.print("  ");Serial.print(nb_lines);Serial.print("  ");Serial.println(nb_colones);
  if (nb_lines == 1 && nb_colones == 1) {
    return 199;
  }
  else if (nb_lines == 1 && nb_colones == 2) {
    if (j == 0) {
      return 167;
    } else {
      return 231;
    }
  }
  else if (nb_lines == 2 && nb_colones == 1) {
    if (i == 0) {
      return 195;
    } else {
      return 60;
    }
  }
  else if (nb_lines == 2 && nb_colones == 2) {
    if (i == 0 && j == 0) {
      return 163;
    }
    else if (i == 0 && j == 1) {
      return 227;
    }
    else if (i == 1 && j == 0) {
      return 92;
    }
    else if (i == 1 && j == 1) {
      return 28;
    }
  }
  else if (nb_lines == 4 && nb_colones == 4) {
    if (i == 0 && j == 0) {
      return 145;
    }
    else if (i == 0 && j == 1) {
      return 177;
    }
    else if (i == 0 && j == 2) {
      return 209;
    }
    else if (i == 0 && j == 3) {
      return 241;
    }
    else if (i == 1 && j == 0) {
      return 148;
    }
    else if (i == 1 && j == 1) {
      return 181;
    }
    else if (i == 1 && j == 2) {
      return 213;
    }
    else if (i == 1 && j == 3) {
      return 245;
    }
    else if (i == 2 && j == 0) {
      return 110;
    }
    else if (i == 2 && j == 1) {
      return 78;
    }
    else if (i == 2 && j == 2) {
      return 46;
    }
    else if (i == 2 && j == 3) {
      return 14;
    }
    else if (i == 3 && j == 0) {
      return 106;
    }
    else if (i == 3 && j == 1) {
      return 74;
    }
    else if (i == 3 && j == 2) {
      return 42;
    }
    else if (i == 3 && j == 3) {
      return 10;
    }
  }
  else {
    return 0;
  }
}

/*
uint8_t opticalTable[16][16] = {
  {128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248},
  {129, 137, 145, 153, 161, 169, 177, 185, 193, 201, 209, 217, 225, 233, 241, 249},
  {130, 138, 146, 154, 162, 170, 178, 186, 194, 202, 210, 218, 226, 234, 242, 250},
  {131, 139, 146, 155, 163, 171, 179, 187, 195, 203, 211, 219, 227, 235, 243, 251},
  {132, 140, 147, 156, 164, 172, 180, 188, 196, 204, 212, 220, 228, 236, 244, 252},
  {133, 141, 148, 157, 165, 173, 181, 189, 197, 205, 213, 221, 229, 237, 245, 253},
  {134, 142, 149, 158, 166, 174, 182, 190, 198, 206, 214, 222, 230, 238, 246, 254},
  {135, 143, 150, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255},
  {127, 119, 111, 103, 95, 87, 79, 71, 63, 55, 47, 39, 31, 23, 15, 7},
  {126, 118, 110, 102, 94, 86, 78, 70, 62, 54, 46, 38, 30, 22, 14, 6},
  {125, 117, 109, 101, 93, 85, 77, 69, 61, 53, 45, 37, 29, 21, 13, 5},
  {124, 116, 108, 100, 92, 84, 76, 68, 60, 52, 44, 36, 28, 20, 12, 4},
  {123, 115, 107, 99, 91, 83, 75, 67, 59, 51, 43, 35, 27, 19, 11, 3},
  {122, 114, 106, 98, 90, 82, 74, 66, 58, 50, 42, 34, 26, 18, 10, 2},
  {121, 113, 105, 97, 89, 81, 73, 65, 57, 49, 41, 33, 25, 17, 9, 1},
  {120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0}
};

uint8_t optical_table[256] = {
  128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216, 224, 232, 240, 248,
  129, 137, 145, 153, 161, 169, 177, 185, 193, 201, 209, 217, 225, 233, 241, 249,
  130, 138, 146, 154, 162, 170, 178, 186, 194, 202, 210, 218, 226, 234, 242, 250,
  131, 139, 146, 155, 163, 171, 179, 187, 195, 203, 211, 219, 227, 235, 243, 251,
  132, 140, 147, 156, 164, 172, 180, 188, 196, 204, 212, 220, 228, 236, 244, 252,
  133, 141, 148, 157, 165, 173, 181, 189, 197, 205, 213, 221, 229, 237, 245, 253,
  134, 142, 149, 158, 166, 174, 182, 190, 198, 206, 214, 222, 230, 238, 246, 254,
  135, 143, 150, 159, 167, 175, 183, 191, 199, 207, 215, 223, 231, 239, 247, 255,
  127, 119, 111, 103, 95, 87, 79, 71, 63, 55, 47, 39, 31, 23, 15, 7,
  126, 118, 110, 102, 94, 86, 78, 70, 62, 54, 46, 38, 30, 22, 14, 6,
  125, 117, 109, 101, 93, 85, 77, 69, 61, 53, 45, 37, 29, 21, 13, 5,
  124, 116, 108, 100, 92, 84, 76, 68, 60, 52, 44, 36, 28, 20, 12, 4,
  123, 115, 107, 99, 91, 83, 75, 67, 59, 51, 43, 35, 27, 19, 11, 3,
  122, 114, 106, 98, 90, 82, 74, 66, 58, 50, 42, 34, 26, 18, 10, 2,
  121, 113, 105, 97, 89, 81, 73, 65, 57, 49, 41, 33, 25, 17, 9, 1,
  120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0
};
*/
