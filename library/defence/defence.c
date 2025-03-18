#include "defence.h"
#include "actions.h"
#include "motor.h"
#include "line.h"
#include "Nano33IoT.h"
#include "gyro.h"
#include "../config.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <math.h>

float DefenceTime = 0;
float DefenceDeltaTime = 0;
float TurnDuty;
float VectorAbsoluteValue = 0;
float DefenceAngle;
#define WallDistance 80 //ゴールラインがない時の壁からの距離
#define GoalDistance 70 //線上にいるときの自分のゴールの距離

void Defence(){
    
}

void Zyoge(){
  if (LineSensorE[0] == 1) {
        //やや前に行きすぎ → 少し下がる
        MotorDuty[0] += 10;
        MotorDuty[1] += 10;
        MotorDuty[2] += 10;
        MotorDuty[3] += 10;
      }
      if (LineSensorE[1] == 1 || LineSensorE[15] == 1) {
        //やや前に行きすぎ → 少し下がる
        MotorDuty[0] += 7;
        MotorDuty[1] += 7;
        MotorDuty[2] += 7;
        MotorDuty[3] += 7;
      }
      if (LineSensorE[2] == 1 || LineSensorE[14] == 1) {
        //やや前に行きすぎ → 少し下がる
        MotorDuty[0] += 3;
        MotorDuty[1] += 3;
        MotorDuty[2] += 3;
        MotorDuty[3] += 3;
      }
      if (LineSensorE[8] == 1) {
        //やや後ろに行きすぎ → 少し上がる
        MotorDuty[0] -= 10;
        MotorDuty[1] -= 10;
        MotorDuty[2] -= 10;
        MotorDuty[3] -= 10;
      }
      if (LineSensorE[7] == 1 || LineSensorE[9] == 1) {
        //やや後ろに行きすぎ → 少し上がる
        MotorDuty[0] -= 7;
        MotorDuty[1] -= 7;
        MotorDuty[2] -= 7;
        MotorDuty[3] -= 7;
      }
      if (LineSensorE[6] == 1 || LineSensorE[10] == 1) {
        //やや後ろに行きすぎ → 少し上がる
        MotorDuty[0] -= 3;
        MotorDuty[1] -= 3;
        MotorDuty[2] -= 3;
        MotorDuty[3] -= 3;
      }
  if(AllLineSensorA > 0){
    MotorDuty[0] += 30;
    MotorDuty[1] += 30;
    MotorDuty[2] += 30;
    MotorDuty[3] += 30;
  }
  if(AllLineSensorC > 0 /*|| AllLineSensor == 0*/){
    MotorDuty[0] -= 30;
    MotorDuty[1] -= 30;
    MotorDuty[2] -= 30;
    MotorDuty[3] -= 30;
  }
}

void Turn(){
  if (AngleX > 180) {
    if(TurnSpeed * (360 - AngleX) / 180 < 20){
      TurnDuty = TurnSpeed * (360 - AngleX) / 180;
    }else{
      TurnDuty = 20;
    }
  } else {
    if(TurnSpeed * AngleX / 180 < 20){
      TurnDuty = TurnSpeed * AngleX / 180 * -1;
    }else{
      TurnDuty = -20;
    }
  }
  MotorDuty[0] += TurnDuty;
  MotorDuty[1] += TurnDuty;
  MotorDuty[2] -= TurnDuty;
  MotorDuty[3] -= TurnDuty;
}

void DefenceStart() {
  UseGyroSensor();
  UseBallSensor();
  while (10 < AngleX && AngleX < 350) {
    UseAllSensor();
    MainMotorState(1, 0, LeastTurnSpeed);
    MainMotorState(2, 0, LeastTurnSpeed);
    MainMotorState(3, 1, LeastTurnSpeed);
    MainMotorState(4, 1, LeastTurnSpeed);
  }
  UseLineSensor();
  while (AllLineSensorC == 0 && AllLineSensorE == 0) {
    UseAllSensor();
    Return();
    //正面を向くための補正
    Turn();
    //計算した値を出力
    UseMotorDuty(); 
  }
  //位置の微調整
  DefenceTime = 0;
  DefenceDeltaTime = time_us_32() / 1000000.0;
  while(AllLineSensorB == 0 || AllLineSensorD == 0){
    UseAllSensor();
    if(AllLineSensorC + LineSensorE[6] + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] + LineSensorE[10] > 0 || DefenceTime > 1.0){
      MotorDuty[0] = -50;
      MotorDuty[1] = -50;
      MotorDuty[2] = -50;
      MotorDuty[3] = -50;
    }else{
      MotorDuty[0] = 50;
      MotorDuty[1] = 50;
      MotorDuty[2] = 50;
      MotorDuty[3] = 50;
    }
    Turn();
    UseMotorDuty();
    DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
    DefenceDeltaTime = time_us_32() / 1000000.0;
  }
  Brake();
  sleep_ms(250);
  DefenceTime = 0;
}

void Return(){
  if(MyGoalDistance == 999){
    if(OpponentGoalDistance == 999){
      //壁を利用する
      if(RightWall < LeftWall){
        MotorDuty[0] = (int)(DefaultSpeed * cos((210 * -1 + 45) * 3.1415 / 180));
        MotorDuty[1] = (int)(DefaultSpeed * sin((210 * -1 + 45) * 3.1415 / 180));
        MotorDuty[2] = (int)(DefaultSpeed * cos((210 * -1 + 45) * 3.1415 / 180));
        MotorDuty[3] = (int)(DefaultSpeed * sin((210 * -1 + 45) * 3.1415 / 180));
      }else{
        MotorDuty[0] = (int)(DefaultSpeed * cos((150 * -1 + 45) * 3.1415 / 180));
        MotorDuty[1] = (int)(DefaultSpeed * sin((150 * -1 + 45) * 3.1415 / 180));
        MotorDuty[2] = (int)(DefaultSpeed * cos((150 * -1 + 45) * 3.1415 / 180));
        MotorDuty[3] = (int)(DefaultSpeed * sin((150 * -1 + 45) * 3.1415 / 180));
      }
    }else{
      MotorDuty[0] = (int)(DefaultSpeed * cos(((180 - OpponentGoalAngle) * -1 + 45) * 3.1415 / 180));
      MotorDuty[1] = (int)(DefaultSpeed * sin(((180 - OpponentGoalAngle) * -1 + 45) * 3.1415 / 180));
      MotorDuty[2] = (int)(DefaultSpeed * cos(((180 - OpponentGoalAngle) * -1 + 45) * 3.1415 / 180));
      MotorDuty[3] = (int)(DefaultSpeed * sin(((180 - OpponentGoalAngle) * -1 + 45) * 3.1415 / 180));
    }
  }else{
    MotorDuty[0] = (int)(DefaultSpeed * cos((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[1] = (int)(DefaultSpeed * sin((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[2] = (int)(DefaultSpeed * cos((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[3] = (int)(DefaultSpeed * sin((MyGoalAngle * -1 + 45) * 3.1415 / 180));
  } 
}

float GetLineAngle(){
  //Vectorは0度～360度(反時計回り)で考えます。
  bool DoneLineSensor[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  float Vector[8] = {0,0,0,0,0,0,0,0};
  int VectorNumber = 0;
  //連続したラインセンサ群の取得
  for(int i = 0;i < 16;i++){
    if(LineSensorE[i] == 1 && DoneLineSensor[i] == false){
      if(i == 0){
        //LineSensor[0]だけ反時計回り側にあるセンサを考える
        if(LineSensorE[15] == 1){
          DoneLineSensor[15] = true;
          if(LineSensorE[14] == 1){
            DoneLineSensor[14] = true;
            if(LineSensorE[13] == 1){
              DoneLineSensor[13] = true;
              if(LineSensorE[12] == 1){
                DoneLineSensor[12] = true;
                if(LineSensorE[11] == 1){
                  DoneLineSensor[11] = true;
                  Vector[VectorNumber] -= 56.25;
                }else{
                  Vector[VectorNumber] -= 45;
                }
              }else{
                Vector[VectorNumber] -= 33.75;
              }
            }else{
              Vector[VectorNumber] -= 22.5;
            }
          }else{
            Vector[VectorNumber] -= 11.25;
          }
        }
      }
      DoneLineSensor[i] = true;
      if(i >= 15){
        //存在しない値を考えないようにする
        Vector[VectorNumber] += 22.5 * i;
        VectorNumber++;
        continue;
      }
      if(LineSensorE[i + 1] == 1){
        DoneLineSensor[i + 1] = true;
        if(i >= 14){
          Vector[VectorNumber] += 11.25 + 22.5 * i;
          VectorNumber++;
          continue;
        }
        if(LineSensorE[i + 2] == 1){
          DoneLineSensor[i + 2] = true;
          if(i >= 13){
            Vector[VectorNumber] += 22.5 + 22.5 * i;
            VectorNumber++;
            continue;
          }
          if(LineSensorE[i + 3] == 1){
            DoneLineSensor[i + 3] = true;
            if(i >= 12){
              Vector[VectorNumber] += 33.75 + 22.5 * i;
              VectorNumber++;
              continue;
            }
            if(LineSensorE[i + 4] == 1){
              DoneLineSensor[i + 4] = true;
              if(i >= 11){
                Vector[VectorNumber] += 45 + 22.5 * i;
                VectorNumber++;
                continue;
              }
              if(LineSensorE[i + 5] == 1){
                DoneLineSensor[i + 5] = true;
                Vector[VectorNumber] += 56.25 + 22.5 * i;
              }else{
                Vector[VectorNumber] += 45 + 22.5 * i;
              }
            }else{
              Vector[VectorNumber] += 33.75 + 22.5 * i;
            }
          }else{
            Vector[VectorNumber] += 22.5 + 22.5 * i;
          }
        }else{
          Vector[VectorNumber] += 11.25 + 22.5 * i;
        }
      }else{
        Vector[VectorNumber] += 22.5 * i;
      }
      VectorNumber++;
    }
  }
  //ベクトルの合成をする
  float VectorX = 0;
  float VectorY = 0;
  for(int i = 0;i < VectorNumber;i++){
    VectorX -= sin(Vector[i] / 180.0 * 3.1415);
    VectorY += cos(Vector[i] / 180.0 * 3.1415);
    if(SerialWatch == 'v' || SerialWatch == 'l'){
      printf("%d : %f ",i,Vector[i]);
    }
  }
  if(VectorNumber == 0){
    VectorX = 999;
    VectorY = 999;
  }else{
    VectorX /= (float)VectorNumber;
    VectorY /= (float)VectorNumber;
  }
  VectorAbsoluteValue = sqrt(VectorX * VectorX + VectorY * VectorY);
  if(SerialWatch == 'v'){
    printf(" 向き(真右が0度) : ");
    if(VectorX == 999 && VectorY == 999){
      printf("%f\n",DefenceAngle);
    }else if(VectorX == 0 && VectorY == 0){
      printf("真ん中\n");
    }else{
      printf("%f\n",atan2(VectorY,VectorX));
    }
  }
  //-π～πまででもとめられる。正面がπ/2になる
  if(VectorX == 0 && VectorX == 0){
    return -1.57;
  }else if(VectorX == 999 && VectorY == 999){
    return DefenceAngle;
  }else{
    return atan2(VectorY,VectorX);
  }
}