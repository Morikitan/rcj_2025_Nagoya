#pragma once

/*******************
a 機体の角度(AngleX)
b ボールセンサーの値(BallDistance,BallAngle)
l ラインセンサーの値(0か1で受け取る)
m モーターに加わる電圧(AngleXもセットで)
t 1回の経過時間(ミリ秒)
B BLEの接続状況
*******************/
extern char SerialWatch;

//mode
extern int mode;  //mode = 99で全部正転
extern char isYellowMyGoal;

//gyro sensor
extern float AngleX, AngleY, AngleZ;

//ball sensor
extern float BallAngle;  //999.0でボール持ってる。-999.0で行方不明
extern int BallDistance;

//line sensor
extern char LineSensorABCD[16];
extern char LineSensorE[16];
extern int AllLineSensorA, AllLineSensorB, AllLineSensorC, AllLineSensorD, AllLineSensorE, AllLineSensor;
extern int ErorrLineSensor;

//camera 
extern int MyGoalX;
extern int MyGoalY;
extern int OpponentGoalX;
extern int OpponentGoalY;
extern double MyGoalAngle;
extern float MyGoalDistance;
extern double OpponentGoalAngle;
extern float OpponentGoalDistance;
extern int LeftWall;
extern int RightWall;

//others
extern int MotorDuty[4];
extern int makao;

//speed
#define LeastTurnSpeed 85
#define LeastSpeed 100
#define TurnSpeed 120
#define DefaultSpeed 220
#define DefaultSpeed1 150
#define DefaultSpeed2 150
#define DefaultSpeed3 150
#define DefaultSpeed4 150
#define LargeDefaultSpeed1 220
#define LargeDefaultSpeed2 220
#define LargeDefaultSpeed3 220
#define LargeDefaultSpeed4 220
#define LineSpeed 200
#define DefenceSpeed 120
#define SinSpeed 10
#define VectorSpeed 30

//MainMotor
#define MMpin1_1 7
#define MMpin1_2 6
#define MMpin2_1 13
#define MMpin2_2 12
#define MMpin3_1 10
#define MMpin3_2 11
#define MMpin4_1 14
#define MMpin4_2 15

//DribblerMotor
#define DMpin1 17
#define DMpin2 16

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