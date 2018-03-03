#pragma config(Sensor, in1,    mogo,           sensorPotentiometer)
#pragma config(Sensor, in2,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  L,              sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  R,              sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  sonar,          sensorQuadEncoder)
#pragma config(Motor,  port1,           mogo1,         tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           driveL1,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           driveL2,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           driveR1,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           driveR2,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           driveL3,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           driveL4,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           driveR3,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           driveR4,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          mogo2,         tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void leftD(int vel){
  motor[driveL1] = vel;
  motor[driveL2] = vel;
  motor[driveL3] = vel;
  motor[driveL4] = vel;
}
void rightD(int vel){
  motor[driveR1] = vel;
  motor[driveR2] = vel;
  motor[driveR3] = vel;
  motor[driveR4] = vel;
}
void drive(int vel){
  rightD(-vel);
  leftD(-vel);
}
void scoop(int vel){
	motor[mogo1] = vel;
	motor[mogo2] = vel;
}


void autoScoop(int target){
  //0 = down, 1 = up
  if(target == 0){
    scoop(-127);
    while(SensorValue(mogo) > 2000) delay(20);
  }else{
    scoop(127);
    while(SensorValue(mogo) > 2000) delay(20);
  }
  scoop(0);
}

void manualDrop(){
  scoop(-127);
  delay(1000);
  scoop(0);
}


// autonomous drive functions =============================================
// forward and backward
void autoDrive(int sp){
  SensorValue[L] = 0;
  SensorValue[R] = 0;
  double kp = 0;
  int kc = 60;
  int brake = -60;
  int dir = 1; //forward
  if(sp < 0){ //backward
    dir = 0;
    brake = -brake;
  }

  while(1){
    int sv = SensorValue(L);
    int error = sp-sv;
    int speed = error*kp;

    //speed normalization
    if(dir == 1 && speed < kc) speed = kc;
    if(dir == 0 && speed > -kc) speed = -kc;

    drive(speed);

    if(dir == 0 && error > 0) break;
    if(dir == 1 && error < 0) break;
  }
  drive(-brake);
  delay(200);
  drive(0); // stop drive
}


void sonarDrive(){
  drive(127); // start driving forward
  int u; // initialize the container for gyro
  do{
    u = SensorValue[gyro];
    delay(10);
  }while(u > 6 || u == 0); // detect pylon in a certain range
  drive(0); // stop drive
}

void gyTurn(int sp){
  double kp = 3.5;
  if(abs(sp - SensorValue(gyro)) > 110) kp = 1.8;
  if(abs(sp - SensorValue(gyro)) > 150) kp = 1.2;
  if(abs(sp - SensorValue(gyro)) > 180) kp = .8;
  int kc = 60;
  int brake = 60;

  //set direction
  int dir = 0; //left
  if(sp - SensorValue[gyro] < 0) dir = 1;
  if(dir == 1) brake = -brake;


  while(1){
    int sv = SensorValue(gyro); // get sensor

    //calculate speed
    int error = sp-sv;
    int speed = error*kp;

    //velocity normalization
    if(speed > 127) speed = 127;
    if(speed < -127) speed = -127;

    //kc enforcement
    if(dir == 0 && speed < kc) speed = kc;
    if(dir == 1 && speed > -kc) speed = -kc;

    //end loop check
    if(dir == 0 && error <= 0) break;
    if(dir == 1 && error >= 0) break;

    leftD(-speed);
    rightD(speed);

    delay(20);
  }
  leftD(brake);
  rightD(-brake);
  delay(100);
  drive(0); // stop drive
}

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
  SensorValue[gyro] = 0;


  //2 /////////////////////////////////////////////////////
  //grab
  autoScoop(0);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-400);
  SensorValue[gyro] = 0;
  gyTurn(152);
  //drop
  drive(40);
  manualDrop();
  SensorValue[gyro] = 0;
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
  SensorValue[gyro] = 0;
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
  SensorValue[gyro] = 0;
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
  SensorValue[gyro] = 0;
  autoDrive(-150); //reverse

  //6 //////////////////////////////////////////////////
  //grab
  SensorValue[gyro] = 0;
  autoScoop(0);
  gyTurn(-158);
  sonarDrive();
  autoScoop(1);
  //alignment
  autoDrive(-400);
  SensorValue[gyro] = 0;
  gyTurn(150);
  //drop
  drive(40);
  manualDrop();
  SensorValue[gyro] = 0;
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
  SensorValue[gyro] = 0;
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
  SensorValue[gyro] = 0;
  autoDrive(-150); //reverse

  //park
  gyTurn(100);
  autoDrive(1000);
}

// testing PID ===============================================================
void driveTest(){
  gyTurn(300);

}

void ram(){
  autoDrive(5000);
}


// control center ===============================================================
task autonomous() {
  SensorValue[gyro] = 0;
  skills();
  motor[driveL1] = 0;
  motor[driveL2] = 0;
  motor[driveL3] = 0;
  motor[driveL4] = 0;
  motor[driveR1] = 0;
  motor[driveR2] = 0;
  motor[driveR3] = 0;
  motor[driveR4] = 0;
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
const unsigned short trueSpeed[128] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 21 , 21, 21, 22, 22, 22, 23, 24, 24,
	25, 25, 25, 25, 26, 27, 27, 28, 28, 28,
	28, 29, 30, 30, 30, 31, 31, 32, 32, 32,
	33, 33, 34, 34, 35, 35, 35, 36, 36, 37,
	37, 37, 37, 38, 38, 39, 39, 39, 40, 40,
	41, 41, 42, 42, 43, 44, 44, 45, 45, 46,
	46, 47, 47, 48, 48, 49, 50, 50, 51, 52,
	52, 53, 54, 55, 56, 57, 57, 58, 59, 60,
	61, 62, 63, 64, 65, 66, 67, 67, 68, 70,
	71, 72, 72, 73, 74, 76, 77, 78, 79, 79,
	80, 81, 83, 84, 84, 86, 86, 87, 87, 88,
	88, 89, 89, 90, 90,127,127,127
};

//LINEAR DRIVE VARIABLES
int threshold = 10;
int speedLeft;
int speedRight;
string rightDrive= "rightDrive";
string leftDrive = "leftDrive";

//SETSPEED FUNCTION
void setSpeed(){
    if(speedLeft > 127) speedLeft = 127;
    if(speedLeft < -127) speedLeft = -127;
		if(speedRight > 127) speedRight = 127;
    if(speedRight < -127) speedRight = -127;
    int absSpeedL = trueSpeed[abs(speedLeft)]*(speedLeft/abs(speedLeft+0.0001));
		int absSpeedR = trueSpeed[abs(speedRight)]*(speedRight/abs(speedRight+0.0001));
    leftD(absSpeedL);
    rightD(absSpeedR);
}
task usercontrol()
{
  // User control code here, inside the loop

  while (true)
  {
    scoop((vexRT[Btn5U] * 127) + (vexRT[Btn5D] * -127));
		speedLeft = vexRT[Ch3] + vexRT[Ch4];
		speedRight = vexRT[Ch3] -  vexRT[Ch4];
		setspeed();
  }
}