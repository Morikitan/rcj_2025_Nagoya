#include "defence.h"
#include "actions.h"
#include "motor.h"
#include "line.h"
#include "Nano33IoT.h"
#include "gyro.h"
#include "camera.h"
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
    //ディフェンダー
    UseAllSensor();
    if (AllLineSensorA > 0 && AllLineSensorE == 0) {
      //A群が反応 → 上がる
      DefenceTime = 0;
      DefenceDeltaTime = time_us_32() / 1000000.0;
      while (AllLineSensorE == 0 && AllLineSensorC == 0 && DefenceTime < 0.5) {
        UseBLE();
        UseLineSensor();
        MainMotorState(1, 0, LargeDefaultSpeed1);
        MainMotorState(2, 0, LargeDefaultSpeed2);
        MainMotorState(3, 0, LargeDefaultSpeed3);
        MainMotorState(4, 0, LargeDefaultSpeed4);
        DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
        DefenceDeltaTime = time_us_32() / 1000000.0;
        if (SerialWatch == 'd') {
          printf("A群\n");
        }
      }
    } else if (AllLineSensorC > 0 && AllLineSensorE == 0) {
      //C群が反応 → 下がる
      DefenceTime = 0;
      DefenceDeltaTime = time_us_32() / 1000000.0;
      while (AllLineSensorE == 0 && AllLineSensorA == 0 && DefenceTime < 0.5) {
        UseBLE();
        UseLineSensor();
        MainMotorState(1, 1, LeastSpeed);
        MainMotorState(2, 1, LeastSpeed);
        MainMotorState(3, 1, LeastSpeed);
        MainMotorState(4, 1, LeastSpeed);
        DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
        DefenceDeltaTime = time_us_32() / 1000000.0;
        if (SerialWatch == 'd') {
          printf("C群\n");
        }
      }
    }else {
      DefenceAngle = GetLineAngle();
      //ボールを追いかける動き
      
      if(AllLineSensor == 0){
        DefenceStart();
        DefenceTime = 0;
      }else if(MyGoalAngle > 260){
        while(MyGoalAngle > 240){
          UseCamera();
          UseGyroSensor();
          MotorDuty[0] = DefaultSpeed1;
          MotorDuty[1] = DefaultSpeed2;
          MotorDuty[2] = DefaultSpeed3;
          MotorDuty[3] = DefaultSpeed4;
          Turn();
          UseMotorDuty();
        }
        Brake();
        DefenceAngle = 1.57;
        VectorAbsoluteValue = 0;
        sleep_ms(100);
      }else if(MyGoalAngle < 100){
        while(MyGoalAngle < 120){
          UseCamera();
          UseGyroSensor();
          MotorDuty[0] = DefaultSpeed1;
          MotorDuty[1] = DefaultSpeed2;
          MotorDuty[2] = DefaultSpeed3;
          MotorDuty[3] = DefaultSpeed4;
          Turn();
          UseMotorDuty();
        }
        Brake();
        DefenceAngle = 1.57;
        VectorAbsoluteValue = 0;
        sleep_ms(100);
      }else if ((10 < BallAngle && BallAngle <= 135 && MyGoalAngle < 210 && ((-2.47 < DefenceAngle && DefenceAngle < -0.67) || (0.67 < DefenceAngle && DefenceAngle < 2.47))) || MyGoalAngle < 150 || DefenceAngle <= -2.47 || 2.47 <= DefenceAngle) {
        //右側にボールがある → 右側へ移動する 2π/3 と -π/3
        if(DefenceAngle > 0){
          MotorDuty[0] = DefaultSpeed1 * (cos(DefenceAngle - 1.57) + sin(DefenceAngle - 1.57));
          MotorDuty[1] = DefaultSpeed2 * (-cos(DefenceAngle - 1.57) + sin(DefenceAngle - 1.57));
          MotorDuty[2] = DefaultSpeed3 * (cos(DefenceAngle - 1.57) + sin(DefenceAngle - 1.57));
          MotorDuty[3] = DefaultSpeed4 * (-cos(DefenceAngle - 1.57) + sin(DefenceAngle - 1.57));
        }else{
          MotorDuty[0] = DefaultSpeed1 * (cos(DefenceAngle + 1.57) + sin(DefenceAngle + 1.57));
          MotorDuty[1] = DefaultSpeed2 * (-cos(DefenceAngle + 1.57) + sin(DefenceAngle + 1.57));
          MotorDuty[2] = DefaultSpeed3 * (cos(DefenceAngle + 1.57) + sin(DefenceAngle + 1.57));
          MotorDuty[3] = DefaultSpeed4 * (-cos(DefenceAngle + 1.57) + sin(DefenceAngle + 1.57));
        }
        DefenceTime = 0;
      } else if ((225 <= BallAngle && BallAngle < 350 && MyGoalAngle > 150) || MyGoalAngle > 210 || (-0.67 <= DefenceAngle && DefenceAngle <= 0.67) ){
        //左側にボールがある → 左側へ移動する
        if(DefenceAngle > 0){
          MotorDuty[0] = DefaultSpeed1 * (-cos(DefenceAngle - 1.57) - sin(DefenceAngle - 1.57));
          MotorDuty[1] = DefaultSpeed2 * (cos(DefenceAngle - 1.57) - sin(DefenceAngle - 1.57));
          MotorDuty[2] = DefaultSpeed3 * (-cos(DefenceAngle - 1.57) - sin(DefenceAngle - 1.57));
          MotorDuty[3] = DefaultSpeed4 * (cos(DefenceAngle - 1.57) - sin(DefenceAngle - 1.57));
        }else{
          MotorDuty[0] = DefaultSpeed1 * (-cos(DefenceAngle + 1.57) - sin(DefenceAngle + 1.57));
          MotorDuty[1] = DefaultSpeed2 * (cos(DefenceAngle + 1.57) - sin(DefenceAngle + 1.57));
          MotorDuty[2] = DefaultSpeed3 * (-cos(DefenceAngle + 1.57) - sin(DefenceAngle + 1.57));
          MotorDuty[3] = DefaultSpeed4 * (cos(DefenceAngle + 1.57) - sin(DefenceAngle + 1.57));
        }
        DefenceTime = 0;
      }else{
        //正面にボールがある → 待機
        MotorDuty[0] = 0;
        MotorDuty[1] = 0;
        MotorDuty[2] = 0;
        MotorDuty[3] = 0;
        //ボールが数秒正面にあったら押し出す
        DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
        if(DefenceTime > 2){
          DefenceTime = 0;
          DefenceDeltaTime = time_us_32() / 1000000.0;
          while(DefenceTime < 1.3 && ((-60 < BallAngle && BallAngle < 60) || (300 < BallAngle && BallAngle < 420))){
            UseBLE();
            UseBallSensor();
            UseGyroSensor();
            if((-60 < BallAngle && BallAngle < -20) || (300 < BallAngle && BallAngle < 330)){
              MotorDuty[0] = 0;
              MotorDuty[1] = 180;
              MotorDuty[2] = 0;
              MotorDuty[3] = 180;
            }else if((20 < BallAngle && BallAngle < 60) || (380 < BallAngle && BallAngle < 420)){
              MotorDuty[0] = 180;
              MotorDuty[1] = 0;
              MotorDuty[2] = 180;
              MotorDuty[3] = 0;
            }else{
              MotorDuty[0] = DefaultSpeed1;
              MotorDuty[1] = DefaultSpeed2;
              MotorDuty[2] = DefaultSpeed3;
              MotorDuty[3] = DefaultSpeed4;
            }
            //正面を向くための補正
            Turn();
            //計算した値を出力
            UseMotorDuty();
            DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
            DefenceDeltaTime = time_us_32() / 1000000.0;
          }
          DefenceStart();
        }
        LineTrace();
      }
      DefenceDeltaTime = time_us_32() / 1000000.0;
      //正位置につくための補正
    }

    LineTrace();
    //正面を向くための補正
    Turn();
    if (SerialWatch == 'd') {
      printf("BaAngle : %f DeAngle : %f MGAngle : %fTurnDuty : %f motor1 : %d m2 : %d m3 : %d m4 : %d 回転 : %d 縦 : %d 横 : %d\n"
      ,BallAngle,DefenceAngle,MyGoalAngle,TurnDuty,MotorDuty[0],MotorDuty[1],MotorDuty[2],MotorDuty[3],MotorDuty[0] + MotorDuty[1] - MotorDuty[2] - MotorDuty[3]
      ,MotorDuty[0] + MotorDuty[1] + MotorDuty[2] + MotorDuty[3],MotorDuty[0] - MotorDuty[1] + MotorDuty[2] - MotorDuty[3]);
    }
    //計算した値を出力
    UseMotorDuty();
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
    UseGyroSensor();
    if(AngleX > 180){
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
  }
  UseLineSensor();
  while ((AllLineSensorC < 2 && AllLineSensorE < 2) || MyGoalDistance == 999) {
    UseLineSensor();
    UseGyroSensor();
    UseCamera();
    //自ゴールに向かって進む
    Return();
    //正面を向くための補正
    Turn();
    //計算した値を出力
    UseMotorDuty(); 
  }
  //位置の微調整
  DefenceTime = 0;
  DefenceDeltaTime = time_us_32() / 1000000.0;
  gpio_put(TSpin6,1);
  while(AllLineSensorB == 0 || AllLineSensorD == 0){
    UseLineSensor();
    UseGyroSensor();
    UseCamera();
    if(AllLineSensorC + LineSensorE[6] + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] + LineSensorE[10] > 0 || DefenceTime > 1.0){
      MotorDuty[0] = -LeastSpeed;
      MotorDuty[1] = -LeastSpeed;
      MotorDuty[2] = -LeastSpeed;
      MotorDuty[3] = -LeastSpeed;
    }else{
      MotorDuty[0] = LeastSpeed;
      MotorDuty[1] = LeastSpeed;
      MotorDuty[2] = LeastSpeed;
      MotorDuty[3] = LeastSpeed;
    }
    Turn();
    UseMotorDuty();
    DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
    DefenceDeltaTime = time_us_32() / 1000000.0;
  }
  gpio_put(TSpin6,0);
  Brake();
  sleep_ms(250);
  DefenceTime = 0;
}

void Return(){
  if(MyGoalDistance == 999){
    gpio_put(TSpin6,1);
      //壁を利用する
      if(RightWall < LeftWall){
        MotorDuty[0] = (int)(DefaultSpeed * cos((-150) * 3.1415 / 180));
        MotorDuty[1] = (int)(DefaultSpeed * sin((-150) * 3.1415 / 180));
        MotorDuty[2] = (int)(DefaultSpeed * cos((-150) * 3.1415 / 180));
        MotorDuty[3] = (int)(DefaultSpeed * sin((-150) * 3.1415 / 180));
      }else{
        MotorDuty[0] = (int)(DefaultSpeed * cos((-120) * 3.1415 / 180));
        MotorDuty[1] = (int)(DefaultSpeed * sin((-120) * 3.1415 / 180));
        MotorDuty[2] = (int)(DefaultSpeed * cos((-120) * 3.1415 / 180));
        MotorDuty[3] = (int)(DefaultSpeed * sin((-120) * 3.1415 / 180));
      }
  }else if(MyGoalDistance > 77){
    gpio_put(TSpin6,0);
    MotorDuty[0] = (int)(DefaultSpeed * cos((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[1] = (int)(DefaultSpeed * sin((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[2] = (int)(DefaultSpeed * cos((MyGoalAngle * -1 + 45) * 3.1415 / 180));
    MotorDuty[3] = (int)(DefaultSpeed * sin((MyGoalAngle * -1 + 45) * 3.1415 / 180));
  }else{
    gpio_put(TSpin6,0);
    MotorDuty[0] = DefaultSpeed1;
    MotorDuty[1] = DefaultSpeed1;
    MotorDuty[2] = DefaultSpeed1;
    MotorDuty[3] = DefaultSpeed1;
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

void LineTrace(){
  float DutyX = cos(DefenceAngle) * VectorSpeed * VectorAbsoluteValue;
  float DutyY = sin(DefenceAngle) * VectorSpeed * VectorAbsoluteValue;
  MotorDuty[0] += (int)(DutyX + DutyY);
  MotorDuty[1] += (int)(-DutyX + DutyY);
  MotorDuty[2] += (int)(DutyX + DutyY);
  MotorDuty[3] += (int)(-DutyX + DutyY);
}