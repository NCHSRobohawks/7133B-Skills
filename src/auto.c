#include "main.h"
#include "scoop.h"
#include "drive.h"
#include "ports.h"
#include "sensorTargets.h"


//drive to pylon program =======================================
void pickUp(){
  drive(127);
  autoScoop(0); // deploy scoop
  sonarDrive(); //go get pylon
  autoScoop(1); // bring scoop up
}


// program 2 ===============================================================
void pylon20(){
  pickUp(); //drive to pylon

  autoDrive(-850);
  gyTurn(-132);//face the zone
  autoDrive(210);
  gyTurn(-213);


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


// control center ===============================================================
void autonomous() {
  gyroReset(gyro);
  skills();
  motorStopAll();
}
