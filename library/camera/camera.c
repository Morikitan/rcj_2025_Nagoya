#include "camera.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"
#include <math.h>

int CameraDataNumber = 1;
unsigned char CameraData[] = {0,0,0,0,0,0};
unsigned char data = 0;

void CameraSetup(){
    uart_init(uart0,9600);
    gpio_set_function(TX0pin, GPIO_FUNC_UART);  // TXピン
    gpio_set_function(RX0pin, GPIO_FUNC_UART);  // RXピン

    gpio_pull_up(TX0pin);
    gpio_pull_up(RX0pin);
}

//開始ビットを決めて制御する
void UseCamera(){
    while(uart_is_readable(uart0)){
        uart_read_blocking(uart0,&data,1);
        if(data = 0x02){
            CameraDataNumber = 0;
        }else{
            CameraData[CameraDataNumber - 1] = data;
        }
        if(CameraDataNumber == 6)CameraDataNumber = 1;
        else CameraDataNumber++;  
    }

    if(isYellowMyGoal == 1){
        if(CameraData[0] == 255)MyGoalX = 999;
        else MyGoalX = CameraData[0];
        if(CameraData[1] == 255)MyGoalY = 999;
        else MyGoalY = CameraData[1];
        if(CameraData[2] == 255)OpponentGoalX = 999;
        else OpponentGoalX = CameraData[2];
        if(CameraData[3] == 255)OpponentGoalY = 999;
        else OpponentGoalY = CameraData[3];
    }else{
        if(CameraData[0] == 255)OpponentGoalX = 999;
        else OpponentGoalX = CameraData[0];
        if(CameraData[1] == 255)OpponentGoalY = 999;
        else OpponentGoalY = CameraData[1];
        if(CameraData[2] == 255)MyGoalX = 999;
        else MyGoalX = CameraData[2];
        if(CameraData[3] == 255)MyGoalY = 999;
        else MyGoalY = CameraData[3];
    }
    if(CameraData[4] == 255 || (15 < AngleX && AngleX < 345))LeftWall = 999;
    else LeftWall = CameraData[4];
    if(CameraData[5] == 255 || (15 < AngleX && AngleX < 345))RightWall = 999;
    else RightWall = CameraData[5];

    if(MyGoalX == 255 && MyGoalY == 255){
        MyGoalAngle = 999;
        MyGoalDistance = 999;
    }else{
        //正面が0度で時計回りの角度に変換する。
        MyGoalAngle = atan2((128.0-MyGoalY),(MyGoalX-128.0)) / 3.1415 * -180 + 90.0;
        if(MyGoalAngle < 0) MyGoalAngle += 360;
        //三平方の定理で相対距離を求める
        MyGoalDistance = sqrt((MyGoalX-128.0)*(MyGoalX-128.0)+(MyGoalY-128.0)*(MyGoalY-128.0));
    }
    
    if(OpponentGoalX == 255 && OpponentGoalY == 255){
        OpponentGoalAngle = 999;
        OpponentGoalDistance = 999;
    }else{
        //正面が0度で時計回りの角度に変換する。
        OpponentGoalAngle = atan2((128.0-MyGoalY),(MyGoalX-128.0)) / 3.1415 * -180 + 90.0;
        if(OpponentGoalAngle < 0) OpponentGoalAngle += 360;
        //三平方の定理で相対距離を求める
        OpponentGoalDistance = sqrt((OpponentGoalX-128.0)*(OpponentGoalX-128.0)+(OpponentGoalY-128.0)*(OpponentGoalY-128.0));
    }
    
    if(SerialWatch = 'c'){
        printf("MyGoalDis : %f Angle : %f OppGoalDis : %f Angle : %d LeftWall : %d RightWall : %d\n"
        ,MyGoalDistance,MyGoalAngle,OpponentGoalDistance,OpponentGoalAngle,LeftWall,RightWall);
    }
}