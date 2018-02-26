#include "main.h"
#include "ports.h"

void initializeIO() {
  //led port
  pinMode(LED, OUTPUT_OD);
}


void initialize() {

  setTeamName("574C Centurion");

  //setup sensors
  driveEncLeft = encoderInit(D_ENC_L1, D_ENC_L2, false);
  driveEncRight = encoderInit(D_ENC_R1, D_ENC_R2, true);
  gyro = gyroInit(GYRO, 0);
  gyroReset(gyro);
  sonar = ultrasonicInit(ULTRA_Y, ULTRA_O);

  //lcd
  lcdInit(uart1);
  lcdClear(uart1);
  lcdSetBacklight(uart1, true);

  buttonInit();

  //expander light check
	while(analogRead(EXPANDER) < 500){
    lcdSetText(uart1, 1, "!Check Pwr Exp!");
		digitalWrite(LED, LOW);
		delay(500);
		digitalWrite(LED, HIGH);
		delay(500);
	}

  int selected = 0; //layered selection
  while(selected < 2){
    lcdSetText(uart1, 2, "");
    while(selected < 1){
      //logic
      if(buttonIsNewPress(LCD_CENT)){
        selected++;
      }else if(buttonIsNewPress(LCD_LEFT)){
        if(auton > -1)auton--;
      }else if(buttonIsNewPress(LCD_RIGHT)){
        if(auton < 3)auton++;
      }

      //display
      switch(auton){
        case -1:
          lcdSetText(uart1, 1, "Skills");
          break;
        case 0:
          lcdSetText(uart1, 1, "None");
          break;
        case 1:
          lcdSetText(uart1, 1, "5pt");
          break;
        case 2:
          lcdSetText(uart1, 1, "20pt");
          break;
        case 3:
          lcdSetText(uart1, 1, "Tower");
          break;
      }
      delay(20);
    }
    lcdSetText(uart1, 2, "Left ----- Right");
    if(buttonIsNewPress(LCD_CENT)){
      selected--;
    }else if(buttonIsNewPress(LCD_LEFT)){
      selected++;
      lcdSetText(uart1, 2,"left selected");
    }else if(buttonIsNewPress(LCD_RIGHT)){
      selected++;
      lcdSetText(uart1, 2,"right selected");
      autoRight = true;
    }
    delay(20);
  }
}
