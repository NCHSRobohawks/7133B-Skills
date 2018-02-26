#ifndef _DRIVE_H_
#define _DRIVE_H_

void tankSigLPC();
void autoDrive(int distance);
void gyTurn(int distance);
void sonarDrive();

void leftD(int vel);
void rightD(int vel);
void drive(int vel);
void driveInit();

#endif
