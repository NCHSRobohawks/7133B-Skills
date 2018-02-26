#include "main.h"
#include "math.h"

//generic drive functions =============================================
void leftD(int vel){
  motorSet(DRIVEL1, vel);
  motorSet(DRIVEL2, vel);
  motorSet(DRIVEL3, vel);
  motorSet(DRIVEL4, vel);
}
void rightD(int vel){
  motorSet(DRIVER1, vel);
  motorSet(DRIVER2, vel);
  motorSet(DRIVER3, vel);
  motorSet(DRIVER4, vel);
}
void drive(int vel){
  rightD(-vel);
  leftD(-vel);
}

// power curve =============================================
void tankSigLPC(){
  double left;
  double right;
  double lJoy = joystickGetAnalog(1, 3);
  double rJoy = joystickGetAnalog(1, 2);
  int deadzone = 8;

  if(mode == 4){
    int lnew = -lJoy;
    lJoy = -rJoy;
    rJoy = lnew;
  }

  if(abs(lJoy) > deadzone){
    left = log(256/(lJoy+128)-1)/(-.025);
  }else{
    left = 0;
  }
  if(abs(rJoy) > deadzone){
    right = log(256/(rJoy+128)-1)/(-.025);
  }else{
    right = 0;
  }

  leftD(left);
  rightD(right);
}

// autonomous drive functions =============================================
// forward and backward
void autoDrive(int sp){
  encoderReset(driveEncLeft);
  encoderReset(driveEncRight);

  double kp = 0.6;
  int kc = 60;
  int brake = -60;
  int dir = 1; //forward
  if(sp < 0){ //backward
    dir = 0;
    brake = -brake;
  }

  while(1){
    int sv = encoderGet(driveEncLeft);
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
    u = ultrasonicGet(sonar);
    delay(10);
  }while(u > 6 || u == 0); // detect pylon in a certain range
  drive(0); // stop drive
}

void gyTurn(int sp){
  if(autoRight == true) sp = -sp; // inverted turn speed for right auton
  double kp = 3.5;
  if(abs(sp - gyroGet(gyro)) > 110) kp = 1.8;
  if(abs(sp - gyroGet(gyro)) > 150) kp = 1.2;
  if(abs(sp - gyroGet(gyro)) > 180) kp = .8;
  int kc = 60;
  int brake = 60;

  //set direction
  int dir = 0; //left
  if(sp - gyroGet(gyro) < 0) dir = 1;
  if(dir == 1) brake = -brake;


  while(1){
    int sv = gyroGet(gyro); // get sensor

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

    lcdPrint(uart1, 1, "gyro: %d", sv);
    delay(20);
  }
  leftD(brake);
  rightD(-brake);
  delay(100);
  drive(0); // stop drive
}
