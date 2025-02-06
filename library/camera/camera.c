#include "camera.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"

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
    if(CameraData[0] == 255)YellowX = 999;
    else YellowX = CameraData[0];
    if(CameraData[1] == 255)YellowY = 999;
    else YellowY = CameraData[1];
    if(CameraData[2] == 255)BlueX = 999;
    else BlueX = CameraData[2];
    if(CameraData[3] == 255)BlueY = 999;
    else BlueY = CameraData[3];
    if(mode == 1 || mode == 3 || mode == 99){
        if(CameraData[4] == 255 || (15 < AngleX && AngleX < 345))LeftWall = 999;
        else LeftWall = CameraData[4];
        if(CameraData[5] == 255 || (15 < AngleX && AngleX < 345))RightWall = 999;
        else RightWall = CameraData[5];
    }else{
        if(CameraData[4] == 255 || (15 < AngleX && AngleX < 345))RightWall = 999;
        else RightWall = 255 - CameraData[4];
        if(CameraData[5] == 255 || (15 < AngleX && AngleX < 345))LeftWall = 999;
        else LeftWall = 255 - CameraData[5];
    }
    
    if(SerialWatch = 'c'){
        printf("YellowX : %d YellowY : %d BlueX : %d BlueY : %d LeftWall : %d RightWall : %d\n"
        ,YellowX,YellowY,BlueX,BlueY,LeftWall,RightWall);
    }
}