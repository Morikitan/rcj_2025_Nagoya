#include "motor.h"
#include "actions.h"
#include "../config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "line.h"
#include "gyro.h"
#include "Nano33IoT.h"
#include "camera.h"

char SerialWatch;
int mode;
float AngleX; 
float AngleY; 
float AngleZ;
int ErorrLineSensor;
int makao;

void VariableSetup(){
  /*******************
  a 機体の角度(AngleX)
  b ボールセンサーの値(BallDistance,BallAngle)
  l ラインセンサーの値(0か1で受け取る)
  m モーターに加わる電圧(AngleXもセットで)
  t 1回の経過時間(ミリ秒)
  B BLEの接続状況
  c カメラの値
  *******************/
  SerialWatch = 'b';

  //mode
  mode = 0;  //mode = 99で全部正転

  //gyro sensor
  AngleX = 0; 
  AngleY = 0; 
  AngleZ = 0;

  //line sensor
  ErorrLineSensor = 0;

  //others 
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
  gpio_set_dir(TSpin1,GPIO_IN);
  gpio_set_dir(TSpin2,GPIO_IN);
  gpio_set_dir(TSpin3,GPIO_IN);
  gpio_set_dir(TSpin4,GPIO_IN);
  gpio_set_dir(TSpin5,GPIO_OUT);
  gpio_set_dir(TSpin6,GPIO_OUT);
  gpio_set_dir(Bupin,GPIO_OUT);
}

void UseAllSensor(){
  UseBallSensor();
  UseLineSensor();
  UseGyroSensor();
  UseBLE();
  UseCamera();
}