#pragma once

/*******************
a 機体の角度(AngleX)
b ボールセンサーの値(BallDistance,BallAngle)
l ラインセンサーの値(0か1で受け取る)
m モーターに加わる電圧(AngleXもセットで)
t 1回の経過時間(ミリ秒)
B BLEの接続状況
*******************/
char SerialWatch = 'b';

//mode
int mode = 0;  //mode = 99で全部正転

//gyro senso
float AngleX = 0, AngleY, AngleZ;

//ball sensor
float BallAngle;  //999.0でボール持ってる。-999.0で行方不明
int BallDistance;

//line sensor
char LineSensorABCD[16];
char LineSensorE[16];
int AllLineSensorA, AllLineSensorB, AllLineSensorC, AllLineSensorD, AllLineSensorE, AllLineSensor;
int ErorrLineSensor = 0;

//public times


//others
int MotorDuty[4];

//speed
#define LeastTurnSpeed 40
#define LeastSpeed 60
#define TurnSpeed 120
#define DefenceTurnSpeed 120
#define DefaultSpeed 220
#define DefaultSpeed1 100
#define DefaultSpeed2 125
#define DefaultSpeed3 125
#define DefaultSpeed4 100
#define LineSpeed 120
#define RDefence 10
#define LDefence 10
#define DefenceSpeed 120

//MainMotor
#define MMpin1_1 6
#define MMpin1_2 7
#define MMpin2_1 10
#define MMpin2_2 11
#define MMpin3_1 12
#define MMpin3_2 13
#define MMpin4_1 14
#define MMpin4_2 15

//DribblerMotor
#define DMpin1 16
#define DMpin2 17

//UART
#define TX0pin 0
#define RX0pin 1
#define TX1pin 8
#define RX1pin 9

//I2C
#define SDA1pin 2
#define SCL1pin 3
#define SDA0pin 4
#define SCL0pin 5

//TactSwitch
#define TSpin1 18
#define TSpin2 19
#define TSpin3 20
#define TSpin4 21
#define TSpin5 22
#define TSpin6 26

//Buzzer
#define Bupin 27

//DribblerSensor
#define DSpin 28