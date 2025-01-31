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
#include "Nano33IoT.h"
#include "line.h"
#include "camera.h"

float AngleSpeed = 0;
float AngleSpeedI = 0;
uint32_t BallPreTime = 0;
uint32_t MainPreTime = 0;
bool isBreak = false;

void Attack(){
    UseBLE();
    UseLineSensor();
    //回転を打ち消す
    if (AllLineSensor > 0){
      gpio_put(TSpin6, 1);
      float LineDeltaTime = 0.0;
      float DeltaTime = 0.0;
      Brake();
      if (AngleX > 180) {
        if (TurnSpeed * (360 - AngleX) / 180 > 40) {
          MotorDuty[0] -= 40;
          MotorDuty[1] -= 40;
          MotorDuty[2] += 40;
          MotorDuty[3] += 40;
        } else {
          MotorDuty[0] -= TurnSpeed * (360 - AngleX) / 180;
          MotorDuty[1] -= TurnSpeed * (360 - AngleX) / 180;
          MotorDuty[2] += TurnSpeed * (360 - AngleX) / 180;
          MotorDuty[3] += TurnSpeed * (360 - AngleX) / 180;
        }
      } else {
        if (TurnSpeed * AngleX / 180 > 40) {
          MotorDuty[0] += 40;
          MotorDuty[1] += 40;
          MotorDuty[2] -= 40;
          MotorDuty[3] -= 40;
        } else {
          MotorDuty[0] += TurnSpeed * AngleX / 180;
          MotorDuty[1] += TurnSpeed * AngleX / 180;
          MotorDuty[2] -= TurnSpeed * AngleX / 180;
          MotorDuty[3] -= TurnSpeed * AngleX / 180;
        }
      }
      int LineDuty[4];
      //移動方向を逆にする
      for(int a = 0;a <= 3;a++){
        LineDuty[a] = MotorDuty[a] * -1.5;
      }
      MainPreTime = time_us_32();
      while (LineDeltaTime < 0.03) {
        UseBLE();
        UseAllSensor();
        if (AllLineSensor <= ErorrLineSensor) {
          LineDeltaTime += (time_us_32() - MainPreTime) / 1000000;
        } else {
          LineDeltaTime = 0;
        }
        DeltaTime += (time_us_32() - MainPreTime) / 1000000;
        MainPreTime = time_us_32();
        if(DeltaTime > 1.5){
          Brake();
          DeltaTime = 0.5;
          //LineSensorの再起動
          GyroSetup();
          MainPreTime = time_us_32();
        }else if(DeltaTime > 0.5){
          if (AngleX > 180) {
            if (TurnSpeed * (360 - AngleX) / 180 > 40) {
              AngleSpeed = 40;
            } else {
              AngleSpeed = TurnSpeed * (360 - AngleX) / 180;
            }
          } else {
            if (TurnSpeed * AngleX / 180 > 40) {
              AngleSpeed = -40;
            } else {
              AngleSpeed = TurnSpeed * AngleX / 180 * -1;
            }
          }
          MotorDuty[0] = LineDuty[0] + AngleSpeed;
          MotorDuty[1] = LineDuty[1] + AngleSpeed;
          MotorDuty[2] = LineDuty[2] - AngleSpeed;
          MotorDuty[3] = LineDuty[3] - AngleSpeed;
          UseMotorDuty();
        }
      }
    } else {
      gpio_put(TSpin6,0);
      UseGyroSensor();
      UseBallSensor();
      UseCamera();
      if (BallAngle == 999) {
        //マカオシュートの準備～実行
        if (AngleX > 170 && AngleX < 190) {
          //マカオシュートする
          if (makao == 0) {
            //左側にいるとき
            Brake();
            sleep_ms(100);  //とりあえずマカオシュートの前に待ってる。いらないかもです。
            while (120 < AngleX && AngleX <= 270) {
              UseBLE();
              UseLineSensor();
              UseGyroSensor();
              MainMotorState(1, 3, 255);
              MainMotorState(2, 2, (int)((AngleX - 150) * 3));
              MainMotorState(3, 1, (int)((AngleX - 150) * 3));
              MainMotorState(4, 1, (int)((AngleX - 150) * 3));
              if(AngleX > 260) DribblerMotorState(3,190);
              if(AllLineSensor > ErorrLineSensor){
                isBreak = true;
                break;
              }
            }
            if(isBreak == false){
              Brake();
              sleep_ms(500);
              makao = 1;
              while(10 < AngleX && AngleX < 350){
                  UseGyroSensor();
                  MainMotorState(1,1,LeastTurnSpeed);
                  MainMotorState(2,1,LeastTurnSpeed);
                  MainMotorState(3,0,LeastTurnSpeed);
                  MainMotorState(4,0,LeastTurnSpeed);
              }
            }
            DribblerMotorState(0,190);
          } else {
            //右側にいるとき
            Brake();
            sleep_ms(100);  //とりあえずマカオシュートの前に待ってる。いらないかもです。
            while (90 <= AngleX && AngleX < 240) {
              UseBLE();
              UseLineSensor();
              UseGyroSensor();
              MainMotorState(1, 1, (int)((210 - AngleX) * 3));
              MainMotorState(2, 1, (int)((210 - AngleX) * 3));
              MainMotorState(3, 2, (int)((210 - AngleX) * 3));
              MainMotorState(4, 3, 255);
              if(AngleX < 100) DribblerMotorState(3,190);
              if(AllLineSensor > ErorrLineSensor){
                isBreak = true;
                break;
              }
            }
            if(isBreak == false){
              Brake();
              sleep_ms(500);
              makao = 0;
            }
            DribblerMotorState(0,190);
          }
        } else {
          //後ろを向く
          if (AngleX > 180) {
            sleep_ms(50);
            while (AngleX > 189) {
              UseBLE();
              UseLineSensor();
              UseGyroSensor();
              MainMotorState(1, 1, LeastTurnSpeed);
              MainMotorState(2, 1, LeastTurnSpeed);
              MainMotorState(3, 0, LeastTurnSpeed);
              MainMotorState(4, 0, LeastTurnSpeed);
              if(AllLineSensor > ErorrLineSensor){
                break;
              }
            }
            Brake();
          } else {
            sleep_ms(50);
            while (AngleX < 171) {
              UseBLE();
              UseLineSensor();
              UseGyroSensor();
              MainMotorState(1, 0, LeastTurnSpeed);
              MainMotorState(2, 0, LeastTurnSpeed);
              MainMotorState(3, 1, LeastTurnSpeed);
              MainMotorState(4, 1, LeastTurnSpeed);
              if(AllLineSensor > ErorrLineSensor){
                break;
              }
            }
            Brake();
          }
        }
      } else {
        //ボールを拾いに行く
        if (BallAngle == -999) {
          //ボールがどこかわからない
          MainMotorState(1, 2, 0);
          MainMotorState(2, 2, 0);
          MainMotorState(3, 2, 0);
          MainMotorState(4, 2, 0);
        } else {
          //ボールのほうへ全速前進
          if (BallAngle >= 180) {
            BallAngle -= 360;
          } else if (BallAngle < -180) {
            BallAngle += 360;
          }
          if (BallDistance == 4) {
            ChaseBall(BallAngle);
          } else if (BallDistance == 3) {
            ChaseBall(BallAngle);
          } else {
            if ((-60 <= BallAngle && BallAngle <= 60) || (300 <= BallAngle && BallAngle <= 420)) {
              ChaseBall(BallAngle * 1.15);
            } else {
              ChaseBall(BallAngle * 1.25);
            }
          }
        }
      }
    }
}

void ChaseBall(float angle){
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