#include "camera.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"

int CameraDataNumber = 1;
unsigned char CameraData[] = {0,0,0,0};

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
        if(CameraDataNumber == 1){
            uart_read_blocking(uart0,&CameraData[0],1);
        }else if(CameraDataNumber == 2){
            uart_read_blocking(uart0,&CameraData[1],1);
        }else if(CameraDataNumber == 3){
            uart_read_blocking(uart0,&CameraData[2],1);
        }else if(CameraDataNumber == 4){
            uart_read_blocking(uart0,&CameraData[3],1);
        }
        if(CameraDataNumber == 4)CameraDataNumber = 1;
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
    if(SerialWatch = 'c'){
        printf("YellowX : %d YellowY : %d BlueX : %d BlueY : %d\n",YellowX,YellowY,BlueX,BlueY);
    }
}