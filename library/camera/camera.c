#include "camera.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"

int CameraDataNumber = 0;

void CameraSetup(){
    uart_init(uart0,9600);
    gpio_set_function(TX0pin, GPIO_FUNC_UART);  // TXピン
    gpio_set_function(RX0pin, GPIO_FUNC_UART);  // RXピン

    gpio_pull_up(TX0pin);
    gpio_pull_up(RX0pin);
}

void UseCamera(){
    while(uart_is_readable(uart0)){
        if(CameraDataNumber == 0){
            uart_read_blocking(uart0,&YellowX,1);
        }else if(CameraDataNumber == 1){
            uart_read_blocking(uart0,&YellowY,1);
        }else if(CameraDataNumber == 2){
            uart_read_blocking(uart0,&BlueX,1);
        }else if(CameraDataNumber == 3){
            uart_read_blocking(uart0,&BlueY,1);
        }
        if(CameraDataNumber == 3)CameraDataNumber = 0;
        else CameraDataNumber++;  
    }
    if(SerialWatch = 'c'){
        printf("YellowX : %u YellowY : %u BlueX : %u BlueY : %u\n",YellowX,YellowY,BlueX,BlueY);
    }
}