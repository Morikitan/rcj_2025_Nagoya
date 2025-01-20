#include "Nano33IoT.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"

void Nano33IoTSetup(){
    uart_init(uart1,115200);
    gpio_set_function(TX1pin, GPIO_FUNC_UART);  // TXピン
    gpio_set_function(RX1pin, GPIO_FUNC_UART);  // RXピン
}

void UseBallSensor(){
    int kurikaesi = 0;
    while (!uart_is_writable(uart1)) {  
        kurikaesi++;
        if(kurikaesi > 100){
            kurikaesi = 0;
            uart_flush();
            if(SerialWatch == 'b'){
            printf("flushしました。\n");
            }
        }
    }
    //BallSensorの値を要求
    uart_putc(uart1,0x01); 
    uart_putc(uart1,0x03); 
    uint8_t data = uart_getc_timeout(uart1, 10); 
    if(data == PICO_ERROR_TIMEOUT){
        BallAngle = -999;
        if(SerialWatch == 'b'){
            printf("データの受信に失敗しました\n");
        }
    }else{
        if (data >= 192) BallDistance = 4;
        else if (data >= 128) BallDistance = 3;
        else if (data >= 64) BallDistance = 2;
        else BallDistance = 1;
        //下位6bitはBallAnlge
        if (data % 64 == 63) {
            BallAngle = -999;
        } else {
            BallAngle = data % 64 * 11.25;
        }
        if (SerialWatch == 'b') {
            printf("data : 0x%x BallDistance : %d BallAngle : %f \n",data,BallDistance,BallAngle);
        }
    }
}

void UseBLE(){
    int kurikaesi = 0;
    while (!uart_is_writable(uart1)) {  
        kurikaesi++;
        if(kurikaesi > 100){
            kurikaesi = 0;
            uart_flush();
            if(SerialWatch == 'B'){
                printf("flushしました。\n");
            }
        }
    }
    //BallSensorの値を要求
    if(mode == 1 || mode == 2 || mode == 99){
        uart_putc(uart1,0x02); 
    }else{
        uart_putc(uart1,0x03); 
    }
    uint8_t data = uart_getc_timeout(uart1, 10); 
    if(data == PICO_ERROR_TIMEOUT){
        BallAngle = -999;
        if(SerialWatch == 'b'){
            printf("データの受信に失敗しました\n");
        }
    }else{
        
    }
}