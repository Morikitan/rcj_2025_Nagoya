#include "camera.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"
#include <math.h>

int CameraDataNumber = 1;
int HowManyData = 0;
uint8_t CameraData1[] = {0,0,0,0,0,0,0};
uint8_t CameraData[] = {0,0,0,0,0,0,0};
uint8_t data = 0;

void CameraSetup(){
    uart_init(uart0,9600);
    gpio_set_function(TX0pin, GPIO_FUNC_UART);  // TXピン
    gpio_set_function(RX0pin, GPIO_FUNC_UART);  // RXピン
}

//開始ビットを決めて制御する
void UseCamera(){
    //HowManyData = 0;
    while(uart_is_readable(uart0)){
        uart_read_blocking(uart0,CameraData1,7);
        if(SerialWatch == 'c')printf("%u %u %u %u %u %u %u",CameraData1[0],CameraData1[1],CameraData1[2],CameraData1[3],CameraData1[4],CameraData1[5],CameraData1[6]);
    }
    for(int i = 0;i < 7;i++){
        if(CameraData1[i] == 2){
            for(int j = 0;j < 7;j++){
                if(i + j >= 7){
                    CameraData[j] = CameraData1[i + j - 7];
                }else{
                    CameraData[j] = CameraData1[i + j];
                }
            }
        }
    }
    if(SerialWatch == 'c')printf("%u %u %u %u %u %u %u\n",CameraData[0],CameraData[1],CameraData[2],CameraData[3],CameraData[4],CameraData[5],CameraData[6]);
    if(isYellowMyGoal == 1){
        if(CameraData[1] == 255)MyGoalX = 999;
        else MyGoalX = (int)CameraData[1];
        if(CameraData[2] == 255)MyGoalY = 999;
        else MyGoalY = (int)CameraData[2];
        if(CameraData[3] == 255)OpponentGoalX = 999;
        else OpponentGoalX = (int)CameraData[3];
        if(CameraData[4] == 255)OpponentGoalY = 999;
        else OpponentGoalY = (int)CameraData[4];
    }else{
        if(CameraData[1] == 255)OpponentGoalX = 999;
        else OpponentGoalX = CameraData[1];
        if(CameraData[2] == 255)OpponentGoalY = 999;
        else OpponentGoalY = CameraData[2];
        if(CameraData[3] == 255)MyGoalX = 999;
        else MyGoalX = CameraData[3];
        if(CameraData[4] == 255)MyGoalY = 999;
        else MyGoalY = CameraData[4];
    }

    if(CameraData[5] == 255)LeftWall = 999;
    else LeftWall = 128 - (int)CameraData[5];
    if(CameraData[6] == 255)RightWall = 999;
    else RightWall = (int)CameraData[6] - 128;

    if(MyGoalX == 999 || MyGoalY == 999){
        MyGoalAngle = 999.0;
        MyGoalDistance = 999.0;
    }else{
        //正面が0度で時計回りの角度に変換する。
        MyGoalAngle = atan2((128.0-MyGoalY),(MyGoalX-128.0)) / 3.1415 * -180 + 90.0;
        if(MyGoalAngle < 0) MyGoalAngle += 360;
        //三平方の定理で相対距離を求める
        MyGoalDistance = sqrt((MyGoalX-128.0)*(MyGoalX-128.0)+(MyGoalY-128.0)*(MyGoalY-128.0));
    }
    
    if(OpponentGoalX == 999 || OpponentGoalY == 999){
        OpponentGoalAngle = 999.0;
        OpponentGoalDistance = 999.0;
    }else{
        //正面が0度で時計回りの角度に変換する。
        OpponentGoalAngle = atan2((128.0-OpponentGoalY),(OpponentGoalX-128.0)) / 3.1415 * -180 + 90.0;
        if(OpponentGoalAngle < 0) OpponentGoalAngle += 360;
        //三平方の定理で相対距離を求める
        OpponentGoalDistance = sqrt((OpponentGoalX-128.0)*(OpponentGoalX-128.0)+(OpponentGoalY-128.0)*(OpponentGoalY-128.0));
    }
    
    if(SerialWatch == 'c'){
        printf("MyGoalDis : %f Angle : %f OppGoalDis : %f Angle : %f LWall : %d RWall : %d\n"
        ,MyGoalDistance,MyGoalAngle,OpponentGoalDistance,OpponentGoalAngle,LeftWall,RightWall);
    }
}