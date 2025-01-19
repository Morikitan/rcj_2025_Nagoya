#include "actions.h"
#include "motor.h"
#include "gyro.h"
#include "../config.h"

float AngleSpeed = 0;
float AngleSpeedI = 0;

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
  AngleSpeedI += AngleSpeed * ((micros() - BallPreTime) / 1000000) * 0.5;
  MotorDuty[0] = int(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[1] = int(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) + AngleSpeed + AngleSpeedI);
  MotorDuty[2] = int(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);
  MotorDuty[3] = int(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) - AngleSpeed - AngleSpeedI);
  //MotorDuty[0] = int(100 + AngleSpeed + AngleSpeedI);
  //MotorDuty[1] = int(100 + AngleSpeed + AngleSpeedI);
  //Mo/torDuty[2] = int(100 - AngleSpeed - AngleSpeedI);
  //MotorDuty[3] = int(100 - AngleSpeed - AngleSpeedI);
  //digitalWrite(TSpin3,HIGH);

  UseMotorDuty();

  if (SerialWatch == "mot") {
    Serial.print("BallAngle : ");
    Serial.print(BallAngle);
    Serial.print(" AngleX : ");
    Serial.print(AngleX);
    Serial.print(" AngleSpeed : ");
    Serial.print(AngleSpeed);
    Serial.print(" motor1 : ");
    Serial.print(MotorDuty[0]);
    Serial.print(" m2 : ");
    Serial.print(MotorDuty[1]);
    Serial.print(" m3 : ");
    Serial.print(MotorDuty[2]);
    Serial.print(" m4 : ");
    Serial.print(MotorDuty[3]);
    Serial.print(" 回転 : ");  //反時計が正
    Serial.print(MotorDuty[0] + MotorDuty[1] - MotorDuty[2] - MotorDuty[3]);
    Serial.print(" 縦 : ");
    Serial.print(MotorDuty[0] + MotorDuty[1] + MotorDuty[2] + MotorDuty[3]);
    Serial.print(" 横 : ");
    Serial.println(MotorDuty[0] - MotorDuty[1] + MotorDuty[2] - MotorDuty[3]);
  }
  BallPreTime = micros();
  PreAngleX = AngleX;
}