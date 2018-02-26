#include "main.h"

//claw grip definitions and variables
static int thresh = 20;
static int sp = -1000; // set to low number to trigger a reset
int gripSpeed = -127;

void claw(int vel){
  motorSet(CLAW, vel);
}

//state changer if claw stops moving
void clawGrip(int speed){
  int sv = analogRead(CLAWPOT);

  //reset the threshold if outside of bound
  if(abs(sv-sp) > thresh){
    sp = sv;
  }else{
    speed = speed/6;
  }

  claw(speed);
}

void clawOp(){
  if(buttonIsNewPress(JOY1_6D)){
    if(gripSpeed < 0){
      gripSpeed = 127;
    }else{
      gripSpeed = -127;
    }
    sp = -1000; // reset
  }
  clawGrip(gripSpeed);
}
