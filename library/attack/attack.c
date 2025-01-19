#include "actions.h"
#include "motor.h"
#include "gyro.h"
#include "../config.h"
#include <stdio.h>
#include <math.h>

float AngleSpeed = 0;
float AngleSpeedI = 0;
float BallPreTime = 0;

void Attack(){

}

void CheseBall(float angle){
    if (AngleX > 180) {
    if (TurnSpeed * (360 - AngleX) / 180 > 40) {
      AngleSpeed = 40;
      AngleSpeedI = 0;
    } else {
      AngleSpeed = TurnSpeed * (360 - AngleX) / 180;
    }
  } else {
    if (TurnSpeed * AngleX / 180 > 40) {
      AngleSpeed = -40;
      AngleSpeedI = 0;
    } else {
      AngleSpeed = TurnSpeed * AngleX / 180 * -1;
    }
  }
  if((micros() - BallPreTime) / 1000000 > 0.5){
    BallPreTime = micros();
  }
  AngleSpeedI += AngleSpeed * ((micros() - BallPreTime) / 1000000) * 0.5;
  MotorDuty[0] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[1] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[2] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);
  MotorDuty[3] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);
  //MotorDuty[0] = int(100 + AngleSpeed + AngleSpeedI);
  //MotorDuty[1] = int(100 + AngleSpeed + AngleSpeedI);
  //Mo/torDuty[2] = int(100 - AngleSpeed - AngleSpeedI);
  //MotorDuty[3] = int(100 - AngleSpeed - AngleSpeedI);
  //digitalWrite(TSpin3,HIGH);

  UseMotorDuty();

  if (SerialWatch == 'm') {
    printf("BallAngle : %f AngleX : %f AngleSpeed : %f",BallAngle,AngleX,AngleSpeed);
    printf(" motor1 : %d m2 : %d m3 : %d m4 : %d",MotorDuty[0],MotorDuty[1],MotorDuty[2],MotorDuty[3]);
    printf(" 回転 : %d 縦 : %d 横 : %d\n",MotorDuty[0] + MotorDuty[1] - MotorDuty[2] - MotorDuty[3],MotorDuty[0] + MotorDuty[1] + MotorDuty[2] + MotorDuty[3],MotorDuty[0] - MotorDuty[1] + MotorDuty[2] - MotorDuty[3]);  //反時計が正
  }
  BallPreTime = micros();
}