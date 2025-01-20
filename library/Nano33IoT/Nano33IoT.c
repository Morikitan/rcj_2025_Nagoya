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
        }
    }
    uart_putc(uart1,0x01);  //BallSensorの値を要求
    uint8_t received_char = uart_getc_timeout(uart0, 10); 
    if(received_char == PICO_ERROR_TIMEOUT){
        BallAngle = -999;
        if(SerialWatch == 'b'){
            printf("データの受信に失敗しました\n");
        }
    }else{
        
    }
}