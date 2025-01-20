#include "attack.h"
#include "actions.h"
#include "motor.h"
#include "gyro.h"
#include "../config.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <math.h>

float AngleSpeed = 0;
float AngleSpeedI = 0;
__uint32_t BallPreTime = 0;

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
  if((time_us_32() - BallPreTime) / 1000000 > 0.5){
    BallPreTime = time_us_32();
  }
  AngleSpeedI += AngleSpeed * ((time_us_32() - BallPreTime) / 1000000) * 0.5;
  MotorDuty[0] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[1] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[2] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);
  MotorDuty[3] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);

  UseMotorDuty();

  if (SerialWatch == 'm') {
    printf("BallAngle : %f AngleX : %f AngleSpeed : %f",BallAngle,AngleX,AngleSpeed);
    printf(" motor1 : %d m2 : %d m3 : %d m4 : %d",MotorDuty[0],MotorDuty[1],MotorDuty[2],MotorDuty[3]);
    printf(" 回転 : %d 縦 : %d 横 : %d\n",MotorDuty[0] + MotorDuty[1] - MotorDuty[2] - MotorDuty[3],MotorDuty[0] + MotorDuty[1] + MotorDuty[2] + MotorDuty[3],MotorDuty[0] - MotorDuty[1] + MotorDuty[2] - MotorDuty[3]);  //反時計が正
  }
  BallPreTime = time_us_32();
}