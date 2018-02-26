#include "main.h"
#include "lift.h"
#include "arm.h"
#include "scoop.h"
#include "drive.h"
#include "claw.h"
#include "ports.h"
#include "sensorTargets.h"

//PID targets
static int liftTarget = 0;
static int armTarget = 0;
static int clawTarget = -1000;

//PID tasks
void liftTask(){
  liftPID(liftTarget);
}
void armTask(){
  armPID(armTarget);
}
void clawTask(){
  clawGrip(clawTarget);
}

//drive to pylon program =======================================
void pickUp(){
  clawTarget = 127; // close claw
  armTarget = AP_AUTO; // raise arm
  liftTarget = LP_BOT;
  drive(127);
  autoScoop(0); // deploy scoop
  sonarDrive(); //go get pylon
  autoScoop(1); // bring scoop up
  liftTarget = LP_LOW;
  armTarget = AP_FRONT; // drop arm to score cone
}

// program 1 ===============================================================
void pylon5() {

  pickUp(); //drive to pylon
  autoDrive(-700);

  //cone drop
  clawTarget = -127; // open claw
  while(analogRead(CLAWPOT) > CP_OPEN) delay(20);
  armTarget = AP_AUTO;
  liftTarget = LP_BOT;

  gyTurn(-180);//face the zone
  autoScoop(0);

  //reverse out of zone
  autoDrive(-300);
}


// program 2 ===============================================================
void pylon20(){
  pickUp(); //drive to pylon

  autoDrive(-850);
  gyTurn(-132);//face the zone
  autoDrive(210);
  gyTurn(-213);

  //cone drop
  clawTarget = -127; // open claw
  while(analogRead(CLAWPOT) > CP_OPEN) delay(20);
  armTarget = AP_AUTO;
  liftTarget = LP_BOT;

  drive(127);
  scoop(-127);
  delay(1200);
  drive(20);
  delay(200);
  scoop(0);

  //reverse out of zone
  autoDrive(-500);
  scoop(-60);
  gyTurn(-398);
}

void skills(){
  //1 /////////////////////////////////////////////////////
  pylon20();
  gyroReset(gyro);


  //2 /////////////////////////////////////////////////////
  //grab
  autoScoop(0);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-400);
  gyroReset(gyro);
  gyTurn(152);
  //drop
  drive(40);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-300); //reverse


  //3 /////////////////////////////////////////////////////
  //grab
  gyTurn(-164);
  autoScoop(0);
  scoop(-20);
  sonarDrive();
  scoop(127);
  //alignment
  autoDrive(1050);
  //drop
  drive(50);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-150); //reverse


  //4 /////////////////////////////////////////////////////
  //grab
  gyTurn(85);
  autoDrive(130);
  gyTurn(115);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-950);
  gyTurn(90);
  autoDrive(-360);
  gyTurn(0);
  autoDrive(270);
  //drop
  drive(127);
  scoop(-127);
  delay(700);
  drive(60);
  delay(200);
  scoop(0);
  gyroReset(gyro);
  autoDrive(-500);

  //5 //////////////////////////////////////////////////
  //grab
  autoScoop(0);
  gyTurn(-90);
  autoDrive(800);
  gyTurn(-130);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-1000);
  gyTurn(-105);
  autoDrive(-300);
  gyTurn(-20);
  //drop
  drive(30);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-150); //reverse

  //6 //////////////////////////////////////////////////
  //grab
  gyroReset(gyro);
  autoScoop(0);
  gyTurn(-158);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-400);
  gyroReset(gyro);
  gyTurn(150);
  //drop
  drive(40);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-250);


  //7 /////////////////////////////////////////////////////////
  //grab
  gyTurn(-165);
  autoScoop(0);
  scoop(-20);
  sonarDrive();
  scoop(127);
  //alignment
  autoDrive(1050);
  //drop
  drive(50);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-150); //reverse


  //8 /////////////////////////////////////////////////////////
  //grab
  gyTurn(85);
  autoDrive(230);
  gyTurn(115);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-950);
  gyTurn(90);
  autoDrive(-360);
  gyTurn(0);
  //drop
  drive(40);
  manualDrop();
  gyroReset(gyro);
  autoDrive(-150); //reverse

  //park
  gyTurn(100);
  autoDrive(1000);
}

// testing PID ===============================================================
void driveTest(){
  gyTurn(300);
  lcdPrint(uart1, 1, "dif: %d", abs(encoderGet(driveEncLeft) - encoderGet(driveEncRight)));
  lcdPrint(uart1, 2, "e %d %d", encoderGet(driveEncLeft), encoderGet(driveEncRight));
}

void ram(){
  autoDrive(5000);
}

void tower(){
  clawTarget = 127;
  delay(500);
  liftTarget = LP_LOW + 200;
  autoDrive(-600);
  drive(60);
  delay(300);
  armTarget = AP_STACK + 200;
  delay(700);
  armTarget = AP_STACK;
  clawTarget = -127;
  delay(500);
  autoDrive(600);
  delay(500);
}

// control center ===============================================================
void autonomous() {
  gyroReset(gyro);
  liftTarget = analogRead(LIFTPOT); // calibrate the PID starting values
  armTarget = analogRead(ARMPOT);

  //start all tasks
  TaskHandle aHandle = taskRunLoop(armTask, 20); //start arm
  TaskHandle lHandle = taskRunLoop(liftTask, 20); //start lift
  TaskHandle cHandle = taskRunLoop(clawTask, 20); //start claw

  switch(auton){
    case -1:
      //autoRight = true;
      skills();
      break;
    case 0:
      //autoDrive(300);
      break; //dont run auton
    case 1:
      pylon5();
      break;
    case 2:
      pylon20();
      break;
    case 3:
      tower();
      break;
  }

  //stop all tasks
  taskDelete(lHandle);
  taskDelete(aHandle);
  taskDelete(cHandle);
  motorStopAll();
}
