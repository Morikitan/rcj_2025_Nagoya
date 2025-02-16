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
bool isMotorDutyLine = false;

void Attack(){
    UseBLE();
    UseLineSensor();
    //回転を打ち消す
    if (AllLineSensor > 0){
      gpio_put(TSpin6, 1);
      float LineDeltaTime = 0.0;
      float DeltaTime = 0.0;
      Brake();
      int LineDuty[4];
      if(isMotorDutyLine = true){
        //MotorDutyの反対に進む
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
        //移動方向を逆にする
        for(int a = 0;a <= 3;a++){
          LineDuty[a] = MotorDuty[a] * -1.5;
        }
      }else{
        //反応したラインセンサから遠ざかるように進む
        if(AllLineSensorA + LineSensorE[15] + LineSensorE[0] + LineSensorE[1] > 0){
          if(AllLineSensorB + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] > 0){
            //左端
            LineDuty[0] = 0;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = 0;
            LineDuty[3] = -LineSpeed;
          }else if(AllLineSensorD + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] > 0){
            //右端
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = 0;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = 0;
          }else{
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = -LineSpeed;
          }
        }else if(AllLineSensorB + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] > 0){
          if(AllLineSensorC + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] == 0){
            //相手のゴール際で事故らないようにするため
            LineDuty[0] = (int)(LineSpeed / 2);
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = (int)(LineSpeed / 2);
            LineDuty[3] = -LineSpeed;
          }else{
            LineDuty[0] = LineSpeed;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = -LineSpeed;
          }
        }else if(AllLineSensorC + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] > 0){
          if(AllLineSensorB + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] > 0){
            //左端
            LineDuty[0] = LineSpeed;
            LineDuty[1] = 0;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = 0;
          }else if(AllLineSensorD + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] > 0){
            //右端
            LineDuty[0] = 0;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = 0;
            LineDuty[3] = LineSpeed;
          }else{
            LineDuty[0] = LineSpeed;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = LineSpeed;
          }
        }else if(AllLineSensorD + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] > 0){
          if(AllLineSensorC + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] == 0){
            //相手のゴール際で事故らないようにするため
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = (int)(LineSpeed / 2);
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = (int)(LineSpeed / 2);
          }else{
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = LineSpeed;
          }
        }
      }
      
      MainPreTime = time_us_32();
      while (LineDeltaTime < 0.03) {
        UseAllSensor();
        if (AllLineSensor <= ErorrLineSensor) {
          LineDeltaTime += (time_us_32() - MainPreTime) / 1000000;
        } else {
          LineDeltaTime = 0;
        }
        DeltaTime += (time_us_32() - MainPreTime) / 1000000;
        MainPreTime = time_us_32();
        if(DeltaTime > 0.5 && isMotorDutyLine == true){
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
        isMotorDutyLine = false;
        //マカオシュートの準備～実行
        if (AngleX > 170 && AngleX < 190) {
          //マカオシュートする
          //反転してるのでカメラの向きが変わる
          if(225 < OpponentGoalAngle && OpponentGoalAngle < 360){
            //ゴールの左側の奥
            isBreak = false;
            while (AngleX < 210) {
                UseLineSensor();
                UseGyroSensor();
                MainMotorState(1, 0, LeastTurnSpeed);
                MainMotorState(2, 0, LeastTurnSpeed);
                MainMotorState(3, 1, LeastTurnSpeed);
                MainMotorState(4, 1, LeastTurnSpeed);
                if(AllLineSensor > ErorrLineSensor){
                  isBreak = true;
                  break;
                }
            }
            if(isBreak = false) Makao(false,120);
          }else if(0 < OpponentGoalAngle && OpponentGoalAngle < 135){
            //ゴールの右側の奥
            isBreak = false;
            while (AngleX > 150) {
                UseLineSensor();
                UseGyroSensor();
                MainMotorState(1, 1, LeastTurnSpeed);
                MainMotorState(2, 1, LeastTurnSpeed);
                MainMotorState(3, 0, LeastTurnSpeed);
                MainMotorState(4, 0, LeastTurnSpeed);
                if(AllLineSensor > ErorrLineSensor){
                  isBreak = true;
                  break;
                }
            }
            if(isBreak = false) Makao(true,240);
          }else if(OpponentGoalDistance < 100){
            if (180 < OpponentGoalAngle) {
              //ゴールの左側にいるとき
              Makao(true,270);
            } else{
              //ゴールの右側にいるとき
              Makao(false,90);
            }
          }else{
            isMotorDutyLine = true;
            if(OpponentGoalAngle < 180){
              //ゴールが左側にある
              ChaseBall(OpponentGoalAngle - 20,true);
            }else if(180 < OpponentGoalAngle && OpponentGoalAngle < 400){
              //ゴールが右側にある
              ChaseBall(OpponentGoalAngle + 20,true);
            }else{
              //ゴールが遠すぎる
              if(LeftWall < RightWall){
                //ゴールが左側にある
                ChaseBall(160,true);
              }else{
                //ゴールが右側にある
                ChaseBall(200,true);
              }
            }
          }
        } else {
            //後ろを向く
            if (AngleX > 180) {
              sleep_ms(50);
              while (AngleX > 189) {
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
        isMotorDutyLine = true;
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
            ChaseBall(BallAngle,false);
          } else if (BallDistance == 3) {
            ChaseBall(BallAngle,false);
          } else {
            if ((-60 <= BallAngle && BallAngle <= 60) || (300 <= BallAngle && BallAngle <= 420)) {
              ChaseBall(BallAngle * 1.15,false);
            } else {
              ChaseBall(BallAngle * 1.25,false);
            }
          }
        }
      }
    }
}

void ChaseBall(float angle,bool isMakao){
  if(isMakao == true){
    if (AngleX < 180) {
      if (TurnSpeed * (180 - AngleX) / 180 > 40) {
        AngleSpeed = 40;
        AngleSpeedI = 0;
      } else {
        AngleSpeed = TurnSpeed * (180 - AngleX) / 180;
      }
    } else {
      if (TurnSpeed * (AngleX - 180) / 180 > 40) {
        AngleSpeed = -40;
        AngleSpeedI = 0;
      } else {
        AngleSpeed = TurnSpeed * (AngleX - 180) / 180 * -1;
      }
    }
  }else{
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

void Makao(bool isClockWise,int TargetAngle){
  Brake();
  sleep_ms(100);
  if(isClockWise == true){
    while (TargetAngle - 150 < AngleX && AngleX <= TargetAngle) {
      UseLineSensor();
      UseGyroSensor();
      MainMotorState(1, 3, 255);
      MainMotorState(2, 0, (int)((AngleX - (TargetAngle - 120) * 3)));
      MainMotorState(3, 1, (int)((AngleX - (TargetAngle - 120) * 3)));
      MainMotorState(4, 1, (int)((AngleX - (TargetAngle - 120) * 3)));

      if(AngleX > TargetAngle - 10) DribblerMotorState(3,190);

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
        if(AngleX > 180){
          MainMotorState(1,0,LeastTurnSpeed);
          MainMotorState(2,0,LeastTurnSpeed);
          MainMotorState(3,1,LeastTurnSpeed);
          MainMotorState(4,1,LeastTurnSpeed);
        }else{
          MainMotorState(1,1,LeastTurnSpeed);
          MainMotorState(2,1,LeastTurnSpeed);
          MainMotorState(3,0,LeastTurnSpeed);
          MainMotorState(4,0,LeastTurnSpeed);
        }
      }
    }
  }else{
    while (TargetAngle <= AngleX && AngleX < TargetAngle + 150) {
      UseLineSensor();
      UseGyroSensor();
      MainMotorState(1, 1, (int)((TargetAngle + 120 - AngleX) * 3));
      MainMotorState(2, 1, (int)((TargetAngle + 120 - AngleX) * 3));
      MainMotorState(3, 0, (int)((TargetAngle + 120 - AngleX) * 3));
      MainMotorState(4, 3, 255);

      if(AngleX < TargetAngle + 10) DribblerMotorState(3,190);

      if(AllLineSensor > ErorrLineSensor){
        isBreak = true;
        break;
      }
    }
    if(isBreak == false){
      Brake();
      sleep_ms(500);
      makao = 0;
      while(10 < AngleX && AngleX < 350){
        UseGyroSensor();
        if(AngleX > 180){
          MainMotorState(1,0,LeastTurnSpeed);
          MainMotorState(2,0,LeastTurnSpeed);
          MainMotorState(3,1,LeastTurnSpeed);
          MainMotorState(4,1,LeastTurnSpeed);
        }else{
          MainMotorState(1,1,LeastTurnSpeed);
          MainMotorState(2,1,LeastTurnSpeed);
          MainMotorState(3,0,LeastTurnSpeed);
          MainMotorState(4,0,LeastTurnSpeed);
        }
      }
    }
  }
  DribblerMotorState(0,190);
}