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
#include "defence.h"

float AngleSpeed = 0;
float AngleSpeedI = 0;
float LastATime = -2;
float LastCTime = -2;
uint32_t BallPreTime = 0;
uint32_t MainPreTime = 0;
uint32_t PreTime1;
bool isBreak = false;
bool isMotorDutyLine = false;

void Attack(){
    UseBLE();
    //ライン上に乗っているかを判定する
    LineMove();
    UseGyroSensor();
    UseBallSensor();
    UseCamera();
    if (BallAngle == 999/* && (time_us_32() - PreTime1) / 10000000.0 > 0.1*/) {
      if((time_us_32() - PreTime1) / 1000000.0 > 0.2){
      isMotorDutyLine = false;
      //マカオシュートの準備～実行
        //反転してるときはカメラの向きが変わる
        if(OpponentGoalAngle != 999 &&((AngleX <= 45 && 45 - AngleX < OpponentGoalAngle && OpponentGoalAngle < 180 - AngleX) ||
         (45 < AngleX && AngleX <= 180 && ((405 - AngleX < OpponentGoalAngle && OpponentGoalAngle <= 360)||(0 <= OpponentGoalAngle && OpponentGoalAngle < 180 - AngleX))) ||
         (AngleX > 180 &&                  405 - AngleX < OpponentGoalAngle && OpponentGoalAngle < 540 - AngleX) )){
          //ゴールの左側の奥?
          isBreak = false;
          while (AngleX < 250 || 270 < AngleX) {
            UseLineSensor();
            UseGyroSensor();
            if(70 < AngleX && AngleX < 250){
              MainMotorState(1, 0, LeastTurnSpeed);
              MainMotorState(2, 0, LeastTurnSpeed);
              MainMotorState(3, 1, LeastTurnSpeed);
              MainMotorState(4, 1, LeastTurnSpeed);
            }else{
              MainMotorState(1, 1, LeastTurnSpeed);
              MainMotorState(2, 1, LeastTurnSpeed);
              MainMotorState(3, 0, LeastTurnSpeed);
              MainMotorState(4, 0, LeastTurnSpeed);
            }
            if(AllLineSensor > ErorrLineSensor){
              isBreak = true;
              break;
            }
          }
          if(isBreak == false) Makao(false,180);
        }else if(OpponentGoalAngle != 999 && ((AngleX <= 180 &&         180 - AngleX < OpponentGoalAngle && OpponentGoalAngle < 315 - AngleX) ||
         (180 < AngleX && AngleX <= 315 && ((540 - AngleX < OpponentGoalAngle && OpponentGoalAngle < 360) || 0 < OpponentGoalAngle && OpponentGoalAngle < 315 - AngleX)) ||
         (AngleX > 315 &&                    540 - AngleX < OpponentGoalAngle && OpponentGoalAngle < 675 - AngleX) )){
          //ゴールの右側の奥
          isBreak = false;
          while (AngleX < 90 || 110 < AngleX) {
            UseLineSensor();
            UseGyroSensor();
            if(AngleX < 90 || 270 < AngleX){
              MainMotorState(1, 0, LeastTurnSpeed);
              MainMotorState(2, 0, LeastTurnSpeed);
              MainMotorState(3, 1, LeastTurnSpeed);
              MainMotorState(4, 1, LeastTurnSpeed);
            }else{
              MainMotorState(1, 1, LeastTurnSpeed);
              MainMotorState(2, 1, LeastTurnSpeed);
              MainMotorState(3, 0, LeastTurnSpeed);
              MainMotorState(4, 0, LeastTurnSpeed);
            }
            if(AllLineSensor > ErorrLineSensor){
              isBreak = true;
              break;
            }
          }
          if(isBreak == false) Makao(true,180);
        }else if(OpponentGoalDistance < 115){
          isBreak = false;
          gpio_put(Bupin,1);
          while (AngleX < 170 || 190 < AngleX) {
            UseLineSensor();
            UseGyroSensor();
            if(180 < AngleX){
              MainMotorState(1, 1, LeastTurnSpeed);
              MainMotorState(2, 1, LeastTurnSpeed);
              MainMotorState(3, 0, LeastTurnSpeed);
              MainMotorState(4, 0, LeastTurnSpeed);
            }else{
              MainMotorState(1, 0, LeastTurnSpeed);
              MainMotorState(2, 0, LeastTurnSpeed);
              MainMotorState(3, 1, LeastTurnSpeed);
              MainMotorState(4, 1, LeastTurnSpeed);
            }
            if(AllLineSensor > ErorrLineSensor){
              isBreak = true;
              break;
            }
          }
          if (360 - AngleX > OpponentGoalAngle) {
            //ゴールの左側にいるとき
            Makao(true,280);
          } else{
            //ゴールの右側にいるとき
            Makao(false,80);
            
          }
          gpio_put(Bupin,0);
        }else{
          isMotorDutyLine = true;
          if(OpponentGoalAngle < 180){
            //ゴールが左側にある
            ChaseBall(OpponentGoalAngle + 20,true);
          }else if(180 < OpponentGoalAngle && OpponentGoalAngle < 400){
            //ゴールが右側にある
            ChaseBall(OpponentGoalAngle - 20,true);
          }else{
            //ゴールが遠すぎる
            if(LeftWall < RightWall){
              //ゴールが左側にある
              ChaseBall(AngleX * -1 - 20,true);
            }else{
              //ゴールが右側にある
              ChaseBall(AngleX * -1 + 20,true);
            }
          }
        }
      }
    } else {
      PreTime1 = time_us_32();
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
        /*if (BallDistance == 4) {
          ChaseBall(BallAngle,false);
        } else if (BallDistance == 3) {
          ChaseBall(BallAngle * 1.25,false);
        } else */{
          if ((-60 <= BallAngle && BallAngle <= 60) || (300 <= BallAngle && BallAngle <= 420)) {
            ChaseBall(BallAngle * 1.5,false);
          } else {
            ChaseBall(BallAngle * 1.5,false);
          }
        }
      }
    }
}

void LineMove(){
    UseLineSensor();
    if (AllLineSensor > 0){
      float LineDeltaTime = 0.0;
      float DeltaTime = 0.0;
      Brake();
      int LineDuty[4];
      {
        //反応したラインセンサから遠ざかるように進む
        if(AllLineSensorA + LineSensorE[15] + LineSensorE[0] + LineSensorE[1] > 0 || (AllLineSensorC + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] > 0 && time_us_32() / 1000000.0 - LastATime < 2 )){
          LastATime = time_us_32() / 1000000.0;
          if(45 <= OpponentGoalAngle && OpponentGoalAngle <= 110 || LeftWall <= 71){
            //左端
            LineDuty[0] = -LineSpeed / 2;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = -LineSpeed / 2;
            LineDuty[3] = -LineSpeed;
          }else if(250 <= OpponentGoalAngle && OpponentGoalAngle <= 315 || RightWall <= 55){
            //右端
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = -LineSpeed / 2;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = -LineSpeed / 2;
          }else{
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = -LineSpeed;
          }
        }else if(AllLineSensorB + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] > 0){
          if(OpponentGoalDistance <= 200){
            //相手のゴール際で事故らないようにするため
            LineDuty[0] = LineSpeed / 2;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = LineSpeed / 2;
            LineDuty[3] = -LineSpeed;
          }else if(MyGoalDistance <= 200){
            LineDuty[0] = LineSpeed;
            LineDuty[1] = -LineSpeed / 2;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = -LineSpeed / 2;
          }else{
            LineDuty[0] = LineSpeed;
            LineDuty[1] = -LineSpeed;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = -LineSpeed;
          }
        }else if(AllLineSensorC + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] > 0 || (AllLineSensorA + LineSensorE[15] + LineSensorE[0] + LineSensorE[1] > 0 && time_us_32() / 1000000.0 - LastCTime < 2 )){
          LastCTime = time_us_32() / 1000000.0;
          if((80 <= MyGoalAngle && MyGoalAngle <= 135) || LeftWall <= 71){
            //左端
            LineDuty[0] = LineSpeed;
            LineDuty[1] = LineSpeed / 1.3;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = LineSpeed / 2;
          }else if((225 <= MyGoalAngle && MyGoalAngle <= 280) || RightWall <= 55){
            //右端
            LineDuty[0] = LineSpeed / 2;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = LineSpeed / 2;
            LineDuty[3] = LineSpeed;
          }else{
            LineDuty[0] = LineSpeed;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = LineSpeed;
            LineDuty[3] = LineSpeed;
          }
        }else if(AllLineSensorD + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] > 0){
          if(OpponentGoalDistance <= 200){
            //相手のゴール際で事故らないようにするため
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = LineSpeed / 2;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = LineSpeed / 2;
          }else if(MyGoalDistance <= 200){
            LineDuty[0] = -LineSpeed / 2;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = -LineSpeed / 2;
            LineDuty[3] = LineSpeed;
          }else{
            LineDuty[0] = -LineSpeed;
            LineDuty[1] = LineSpeed;
            LineDuty[2] = -LineSpeed;
            LineDuty[3] = LineSpeed;
          }
        }
      }
      MotorDuty[0] = LineDuty[0];
      MotorDuty[1] = LineDuty[1];
      MotorDuty[2] = LineDuty[2];
      MotorDuty[3] = LineDuty[3];
      UseMotorDuty();
      MainPreTime = time_us_32() / 1000000.0;
      while (LineDeltaTime < 0.05) {
        UseAllSensor();
        if (AllLineSensor <= ErorrLineSensor) {
          LineDeltaTime += time_us_32() / 1000000.0 - MainPreTime;
        } else {
          LineDeltaTime = 0;
        }
        DeltaTime += time_us_32() / 1000000.0 - MainPreTime;
        MainPreTime = time_us_32() / 1000000.0;
      }
    }
}

void ChaseBall(float angle,bool isMakao){
  if(isMakao == true){
    if (AngleX < 180) {
      if (TurnSpeed * (180 - AngleX) / 180 > 40) {
        AngleSpeed = 40;
      } else {
        AngleSpeed = TurnSpeed * (180 - AngleX) / 180;
      }
    } else {
      if (TurnSpeed * (AngleX - 180) / 180 > 40) {
        AngleSpeed = -40;
      } else {
        AngleSpeed = TurnSpeed * (AngleX - 180) / 180 * -1;
      }
    }
  }else{
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
  }
  float SinSpeed2;
  float Gensoku;
  //ボールを持っているときに左右の動きの動きをつける
  if(isMakao == true)SinSpeed2 = SinSpeed * sin(BallPreTime / 1000000.0);
  else SinSpeed2 = 0;
  //壁際に近い時は減速する
  if(LeftWall < 50 || RightWall < 50) Gensoku = 1.0;
  else Gensoku = 1.0;

  MotorDuty[0] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) * Gensoku + AngleSpeed + SinSpeed2);
  MotorDuty[1] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) * Gensoku + AngleSpeed - SinSpeed2);
  MotorDuty[2] = (int)(DefaultSpeed * cos((angle * -1 + 45) * 3.1415 / 180) * Gensoku - AngleSpeed + SinSpeed2);
  MotorDuty[3] = (int)(DefaultSpeed * sin((angle * -1 + 45) * 3.1415 / 180) * Gensoku - AngleSpeed - SinSpeed2);

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
  sleep_ms(300);
  if(isClockWise == true){
    while (TargetAngle - 150 < AngleX && AngleX <= TargetAngle) {
      UseLineSensor();
      UseGyroSensor();
      if((int)((AngleX - (TargetAngle - 140)) * 3.5) > 255){
        MainMotorState(1, 0, 255);
        MainMotorState(2, 0, 255);
        MainMotorState(3, 1, 255);
        MainMotorState(4, 1, 255);
      }else if((int)((AngleX - (TargetAngle - 140))* 3.5) > 100){
        MainMotorState(1, 0, (int)((AngleX - (TargetAngle - 140)) * 3.5));
        MainMotorState(2, 0, (int)((AngleX - (TargetAngle - 140)) * 3.5));
        MainMotorState(3, 1, (int)((AngleX - (TargetAngle - 140)) * 3.5));
        MainMotorState(4, 1, (int)((AngleX - (TargetAngle - 140)) * 3.5));
      }else{
        MainMotorState(1, 0, 100);
        MainMotorState(2, 0, 100);
        MainMotorState(3, 1, 100);
        MainMotorState(4, 1, 100);
      }
      if(AngleX > TargetAngle - 10){
        DribblerMotorState(0,50);
      }

      /*if(AllLineSensor > ErorrLineSensor){
        isBreak = true;
        break;
      }*/
    }
    if(isBreak == false){
      Brake();
      sleep_ms(100);
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
      if((int)((TargetAngle + 140 - AngleX) * 3.5) > 255){
        MainMotorState(1, 1, 255);
        MainMotorState(2, 1, 255);
        MainMotorState(4, 0, 255);
        MainMotorState(3, 0, 255);
      }else if((int)((TargetAngle + 140 - AngleX) * 3.5) > 100){
        MainMotorState(1, 1, (int)((TargetAngle + 140 - AngleX) * 3.5));
        MainMotorState(2, 1, (int)((TargetAngle + 140 - AngleX) * 3.5));
        MainMotorState(4, 0, (int)((TargetAngle + 140 - AngleX) * 3.5));
        MainMotorState(3, 0, (int)((TargetAngle + 140 - AngleX) * 3.5));
      }else{
        MainMotorState(1, 1, 100);
        MainMotorState(2, 1, 100);
        MainMotorState(4, 0, 100);
        MainMotorState(3, 0, 100);
      }

      if(AngleX < TargetAngle + 10){
        DribblerMotorState(0,50);
      }

      /*if(AllLineSensor > ErorrLineSensor){
        isBreak = true;
        break;
      }*/
    }
    if(isBreak == false){
      Brake();
      sleep_ms(100);
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
  DribblerMotorState(0,255);
  if(mode == 9 || mode == 10){
    mode -= 6;
    DefenceStart();
  }
}