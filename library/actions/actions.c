#include "motor.h"
#include "actions.h"
#include "../config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "line.h"
#include "gyro.h"
#include "Nano33IoT.h"
#include "camera.h"
#include "hardware/adc.h"

char SerialWatch;
int mode;  //mode = 99で全部正転
float AngleX, AngleY, AngleZ;
float BallAngle;  //999.0でボール持ってる。-999.0で行方不明
int BallDistance;
char LineSensorABCD[16];
char LineSensorE[16];
int AllLineSensorA, AllLineSensorB, AllLineSensorC, AllLineSensorD, AllLineSensorE, AllLineSensor;
int ErorrLineSensor;
char isYellowMyGoal;
int MyGoalX;
int MyGoalY;
int OpponentGoalX;
int OpponentGoalY;
double MyGoalAngle;
float MyGoalDistance;
double OpponentGoalAngle;
float OpponentGoalDistance;
int LeftWall;
int RightWall;
int MotorDuty[4];
int makao;
float PMotorDuty[4] = {0,0,0,0};

#define Kp 0.3

void VariableSetup(){
  /*******************
  a 機体の角度(AngleX)
  b ボールセンサーの値(BallDistance,BallAngle)
  l ラインセンサーの値(0か1で受け取る)
  m モーターに加わる電圧(AngleXもセットで)
  t 1回の経過時間(ミリ秒)
  B BLEの接続状況
  c カメラの値
  v ベクトル
  *******************/
  SerialWatch = 'l';

  //必ず変更しましょう。1で黄色ゴールが自分側(相手にシュートされる側)。0で逆
  isYellowMyGoal = 1;

  //mode
  mode = 0;  //mode = 99で全部正転

  //gyro sensor
  AngleX = 0; 
  AngleY = 0; 
  AngleZ = 0;

  //line sensor
  ErorrLineSensor = 2;

  //camera
  MyGoalX = 0;
  MyGoalY = 0;
  OpponentGoalX = 0;
  OpponentGoalY = 0;
  MyGoalAngle = 0;
  MyGoalDistance = 0;
  OpponentGoalAngle = 0;
  OpponentGoalDistance = 0;
  LeftWall = 0;
  RightWall = 0;

  makao = 0;
}

void Brake(){
    MainMotorState(1, 3, 255);
    MainMotorState(2, 3, 255);
    MainMotorState(3, 3, 255);
    MainMotorState(4, 3, 255);
}

void UseMotorDuty(){
  for (int a = 0; a <= 3; a++) {
    if (MotorDuty[a] > 255) {
      MainMotorState(a + 1, 0, 255);
    } else if (MotorDuty[a] > 0) {
      MainMotorState(a + 1, 0, MotorDuty[a]);
    } else if (MotorDuty[a] > -255) {
      MainMotorState(a + 1, 1, MotorDuty[a] * -1);
    } else {
      MainMotorState(a + 1, 1, 255);
    }
  }
}

void PinSetup(){
  gpio_init(TSpin1);
  gpio_init(TSpin2);
  gpio_init(TSpin3);
  gpio_init(TSpin4);
  gpio_init(TSpin5);
  gpio_init(TSpin6);
  gpio_init(Bupin);
  gpio_init(DSpin);
  gpio_set_dir(TSpin1,GPIO_IN);
  gpio_set_dir(TSpin2,GPIO_IN);
  gpio_set_dir(TSpin3,GPIO_IN);
  gpio_set_dir(TSpin4,GPIO_IN);
  gpio_set_dir(TSpin5,GPIO_IN);
  gpio_set_dir(TSpin6,GPIO_IN);
  gpio_set_dir(Bupin,GPIO_OUT);
  gpio_pull_down(TSpin5);
}

void UseAllSensor(){
  UseBallSensor();
  UseLineSensor();
  UseGyroSensor();
  UseBLE();
  UseCamera();
}

void UsePMotorDuty() {
  for (int a = 0; a <= 3; a++) {
    if (PMotorDuty[a] > 255) {
      MainMotorState(a + 1, 0, 255);
    } else if (PMotorDuty[a] > 0) {
      MainMotorState(a + 1, 0, (int)(PMotorDuty[a]));
    } else if (PMotorDuty[a] > -255) {
      MainMotorState(a + 1, 1, (int)(PMotorDuty[a] * -1));
    } else {
      MainMotorState(a + 1, 1, 255);
    }
  }
}

void PMainMotorState(int motor, int state, int speed){
  if(state == 0){
    PMotorDuty[motor - 1] += (speed - PMotorDuty[motor - 1]) * Kp;
  }else if(state == 1){
    PMotorDuty[motor - 1] -= (speed + PMotorDuty[motor - 1]) * Kp;
  }else if(state == 2){
    PMotorDuty[motor - 1] -= PMotorDuty[motor - 1] * Kp;
  }else if(state == 3){
    PMotorDuty[motor - 1] = 0;
  }
}
