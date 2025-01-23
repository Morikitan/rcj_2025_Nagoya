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

float DefenceTime = 0;
float DefenceDeltaTime = 0;

void Defence(){
    //ディフェンダー
    UseAllSensor();
    if (AllLineSensorA > 0) {
      //A群が反応 → 上がる{
        while (AllLineSensorE == 0) {
          UseBLE();
          UseLineSensor();
          MainMotorState(1, 0, 80);
          MainMotorState(2, 0, 80);
          MainMotorState(3, 0, 80);
          MainMotorState(4, 0, 80);
        }
    } else if (AllLineSensorC > 0) {
      //C群が反応 → 下がる
      while (AllLineSensorE == 0) {
        UseBLE();
        UseLineSensor();
        MainMotorState(1, 1, 80);
        MainMotorState(2, 1, 80);
        MainMotorState(3, 1, 85);
        MainMotorState(4, 1, 85);
      }
    } else if(AllLineSensorB + AllLineSensorE > 0 && AllLineSensorD + LineSensorE[11] + LineSensorE[12] + LineSensorE[13]== 0){
      gpio_put(TSpin6,1);
      while(AllLineSensorD == 0){
        UseBLE();
        UseAllSensor();
        if(0 <= BallAngle && BallAngle < 180){
          if(180 < AngleX && AngleX <= 355){
            MainMotorState(1, 0, LeastTurnSpeed);
            MainMotorState(2, 0, LeastTurnSpeed);
            MainMotorState(3, 1, LeastTurnSpeed);
            MainMotorState(4, 1, LeastTurnSpeed);
          }else if(5 <= AngleX && AngleX < 180){
            MainMotorState(1, 1, LeastTurnSpeed);
            MainMotorState(2, 1, LeastTurnSpeed);
            MainMotorState(3, 0, LeastTurnSpeed);
            MainMotorState(4, 0, LeastTurnSpeed);
          }else if(AllLineSensorB == 0 && AllLineSensorD == 0 && LineSensorE[0] + LineSensorE[1] + LineSensorE[15] + AllLineSensorA > 0){
            MainMotorState(1, 0, LeastSpeed);
            MainMotorState(2, 0, LeastSpeed);
            MainMotorState(3, 0, LeastSpeed);
            MainMotorState(4, 0, LeastSpeed);
          }else if((AllLineSensorB == 0 && AllLineSensorD == 0 && LineSensorE[8] + LineSensorE[7] + LineSensorE[9] + AllLineSensorC > 0) || AllLineSensor == 0){
            MainMotorState(1, 1, LeastSpeed);
            MainMotorState(2, 1, LeastSpeed);
            MainMotorState(3, 1, LeastSpeed);
            MainMotorState(4, 1, LeastSpeed);
          }else if(90 < BallAngle && BallAngle <= 180){
            while(90 < BallAngle && BallAngle <= 180){
              UseAllSensor();
              if(AllLineSensorB + AllLineSensorE == 0){
                MotorDuty[0] = LeastSpeed;
                MotorDuty[1] = -LeastSpeed;
                MotorDuty[2] = LeastSpeed;
                MotorDuty[3] = -LeastSpeed;
                Turn();
                UseMotorDuty();
              }else if(AllLineSensorA + LineSensorE[0]+ LineSensorE[1]+ LineSensorE[15] > 0){
                Brake();
              }else{
                MotorDuty[0] = -LeastSpeed;
                MotorDuty[1] = -LeastSpeed;
                MotorDuty[2] = -LeastSpeed;
                MotorDuty[3] = -LeastSpeed;
                Turn();
                UseMotorDuty();
              }
            }
            while(AllLineSensorD == 0 && AllLineSensor != 0){
              UseAllSensor();
              MotorDuty[0] = LeastSpeed;
              MotorDuty[1] = LeastSpeed;
              MotorDuty[2] = LeastSpeed;
              MotorDuty[3] = LeastSpeed;
              Turn();
              UseMotorDuty();
            }
            Brake();
            sleep_ms(250);
          }else{
            Brake();
          }   
        }else{
          MotorDuty[0] = -DefaultSpeed1;
          MotorDuty[1] = DefaultSpeed2;
          MotorDuty[2] = -DefaultSpeed3;
          MotorDuty[3] = DefaultSpeed4;
          Zyoge();
          Turn();
          UseMotorDuty();
        }
      }
      gpio_put(TSpin6,0);
    }else if(AllLineSensorD + AllLineSensorE > 0 && AllLineSensorB + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] == 0){
      gpio_put(TSpin6,1);
      while(AllLineSensorB == 0){
        UseBLE();
        UseAllSensor();
        //LineSensorE[8]を利用すれば位置ずれを治せる可能性
        if(0 <= BallAngle && BallAngle < 180){
          MotorDuty[0] = DefaultSpeed1;
          MotorDuty[1] = -DefaultSpeed2;
          MotorDuty[2] = DefaultSpeed3;
          MotorDuty[3] = -DefaultSpeed4;
          Zyoge();
          Turn();
          UseMotorDuty();
        }else{
          if(180 < AngleX && AngleX <= 358){
            MainMotorState(1, 0, LeastTurnSpeed);
            MainMotorState(2, 0, LeastTurnSpeed);
            MainMotorState(3, 1, LeastTurnSpeed);
            MainMotorState(4, 1, LeastTurnSpeed);
          }else if(2 <= AngleX && AngleX < 180){
            MainMotorState(1, 1, LeastTurnSpeed);
            MainMotorState(2, 1, LeastTurnSpeed);
            MainMotorState(3, 0, LeastTurnSpeed);
            MainMotorState(4, 0, LeastTurnSpeed);
          }else if(AllLineSensorB == 0 && AllLineSensorD == 0 && LineSensorE[0] + LineSensorE[1] + LineSensorE[15] + AllLineSensorA > 0){
            MainMotorState(1, 0, LeastSpeed);
            MainMotorState(2, 0, LeastSpeed);
            MainMotorState(3, 0, LeastSpeed);
            MainMotorState(4, 0, LeastSpeed);
          }else if(AllLineSensorB == 0 && AllLineSensorD == 0 && LineSensorE[8] + LineSensorE[7] + LineSensorE[9] + AllLineSensorC > 0 || AllLineSensor == 0 || LineSensorE[6] + LineSensorE[10] == 1){
            MainMotorState(1, 1, LeastSpeed);
            MainMotorState(2, 1, LeastSpeed);
            MainMotorState(3, 1, LeastSpeed);
            MainMotorState(4, 1, LeastSpeed);
          }else if(180 <= BallAngle && BallAngle < 270){
            while(180 <= BallAngle && BallAngle < 270){
              UseAllSensor();
              if(AllLineSensorD + AllLineSensorE == 0){
                MotorDuty[0] = -LeastSpeed;
                MotorDuty[1] = LeastSpeed;
                MotorDuty[2] = -LeastSpeed;
                MotorDuty[3] = LeastSpeed;
                Turn();
                UseMotorDuty();
              }else if(AllLineSensorA + LineSensorE[0]+ LineSensorE[1]+ LineSensorE[15] > 0){
                Brake();
              }else{
                MotorDuty[0] = -LeastSpeed;
                MotorDuty[1] = -LeastSpeed;
                MotorDuty[2] = -LeastSpeed;
                MotorDuty[3] = -LeastSpeed;
                Turn();
                UseMotorDuty();
              }
            }
            while(AllLineSensorB == 0 && AllLineSensor != 0){
              UseAllSensor();
              MotorDuty[0] = LeastSpeed;
              MotorDuty[1] = LeastSpeed;
              MotorDuty[2] = LeastSpeed;
              MotorDuty[3] = LeastSpeed;
              Turn();
              UseMotorDuty();
            }
            Brake();
            sleep_ms(250);
          }else{
            Brake();
          }
        }
      }
      gpio_put(TSpin6,0);
    }else {
      //ボールを追いかける動き
      if (135 < BallAngle && BallAngle < 225) {
        //後ろにボールがある → 下がる
        MotorDuty[0] = -100;
        MotorDuty[1] = -100;
        MotorDuty[2] = -100;
        MotorDuty[3] = -100;
        DefenceTime = 0;
      }else if (10 < BallAngle && BallAngle <= 135) {
        //右側にボールがある → 右側へ移動する
        MotorDuty[0] = DefaultSpeed1;
        MotorDuty[1] = -DefaultSpeed2;
        MotorDuty[2] = DefaultSpeed3;
        MotorDuty[3] = -DefaultSpeed4;
        DefenceTime = 0;
      } else if (225 <= BallAngle && BallAngle < 350) {
        //左側にボールがある → 左側へ移動する
        MotorDuty[0] = -DefaultSpeed1;
        MotorDuty[1] = DefaultSpeed2;
        MotorDuty[2] = -DefaultSpeed3;
        MotorDuty[3] = DefaultSpeed4;
        DefenceTime = 0;
      } else if(AllLineSensor == 0){
        MotorDuty[0] = -75;
        MotorDuty[1] = -75;
        MotorDuty[2] = -75;
        MotorDuty[3] = -75;
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
          while(DefenceTime < 1.5 && ((-60 < BallAngle && BallAngle < 60) || (300 < BallAngle && BallAngle < 420))){
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
              MotorDuty[0] = 120;
              MotorDuty[1] = 120;
              MotorDuty[2] = 130;
              MotorDuty[3] = 130;
            }
            //正面を向くための補正
            Turn();
            //計算した値を出力
            UseMotorDuty();
            DefenceTime += time_us_32() / 1000000.0 - DefenceDeltaTime;
            DefenceDeltaTime = time_us_32() / 1000000.0;
          }
          UseLineSensor();
          while(AllLineSensor == 0){
            UseBLE();
            UseLineSensor();
            UseGyroSensor();
            MotorDuty[0] = -80;
            MotorDuty[1] = -80;
            MotorDuty[2] = -85;
            MotorDuty[3] = -85;
            //正面を向くための補正
            Turn();
            //計算した値を出力
            UseMotorDuty();
          }
          Brake();
          sleep_ms(250);
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
      }
      DefenceDeltaTime = time_us_32() / 1000000.0;
      //正位置につくための補正
      Zyoge();
    }
    //正面を向くための補正
    Turn();
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
      MotorDuty[0] += TurnSpeed * (360 - AngleX) / 180;
      MotorDuty[1] += TurnSpeed * (360 - AngleX) / 180;
      MotorDuty[2] -= TurnSpeed * (360 - AngleX) / 180;
      MotorDuty[3] -= TurnSpeed * (360 - AngleX) / 180;
    }else{
      MotorDuty[0] += 20;
      MotorDuty[1] += 20;
      MotorDuty[2] -= 20;
      MotorDuty[3] -= 20;
    }
  } else {
    if(TurnSpeed * AngleX / 180 < 20){
      MotorDuty[0] -= TurnSpeed * AngleX / 180;
      MotorDuty[1] -= TurnSpeed * AngleX / 180;
      MotorDuty[2] += TurnSpeed * AngleX / 180;
      MotorDuty[3] += TurnSpeed * AngleX / 180;
    }else{
      MotorDuty[0] -= 20;
      MotorDuty[1] -= 20;
      MotorDuty[2] += 20;
      MotorDuty[3] += 20;
    }
  }
}

void DefenceStart() {
  UseGyroSensor();
  UseBallSensor();
  bool isKaiten = false;
  bool BallMuki;
  if(BallAngle == -999){
    isKaiten = true;
  }else{
    if(0 <= BallAngle && BallAngle < 180){
      BallMuki = false; //自ゴールから見て右側にいる
    }else{
      BallMuki = true; //自ゴールから見て左側にいる
    }
  }
  while (10 < AngleX && AngleX < 350) {
    UseAllSensor();
    MainMotorState(1, 0, LeastTurnSpeed);
    MainMotorState(2, 0, LeastTurnSpeed);
    MainMotorState(3, 1, LeastTurnSpeed);
    MainMotorState(4, 1, LeastTurnSpeed);
  }
  Brake();
  sleep_ms(250);
  if(isKaiten == true){
    do{
      UseBallSensor();
    }while(BallAngle = -999);
    if(0 <= BallAngle && BallAngle < 180){
      BallMuki = true; //自ゴールから見て左側にいる
    }else{
      BallMuki = false; //自ゴールから見て右側にいる
    }
  }
  UseLineSensor();
  while (AllLineSensorC == 0 && AllLineSensorE == 0) {
    UseAllSensor();
    if (BallAngle < 170 || 190 < BallAngle) {
      if(BallMuki == false){
        //自ゴールから見て右側にいる
        MotorDuty[0] = -120;
        MotorDuty[1] = -80;
        MotorDuty[2] = -120;
        MotorDuty[3] = -80;
      }else{
        //自ゴールから見て左側にいる
        MotorDuty[0] = -80;
        MotorDuty[1] = -120;
        MotorDuty[2] = -80;
        MotorDuty[3] = -120;
      }
      //正面を向くための補正
      Turn();
      //計算した値を出力
      UseMotorDuty();
    }else{
      Brake();
    }   
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