#include "Nano33IoT.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>
#include "../config.h"
#include "hardware/adc.h"
#include "actions.h"

bool isBLE = true;
bool UsedBLE = false;

void Nano33IoTSetup(){
    uart_init(uart1,115200);
    gpio_set_function(TX1pin, GPIO_FUNC_UART);  // TXピン
    gpio_set_function(RX1pin, GPIO_FUNC_UART);  // RXピン
    //実際は消す → 消した
    //gpio_pull_up(TX1pin);
    //gpio_pull_up(RX1pin);
}

void UseBallSensor(){
    int kurikaesi = 0;
    while (!uart_is_writable(uart1)) {  
        kurikaesi++;
        if(kurikaesi > 100){
            kurikaesi = 0;
            if(SerialWatch == 'b'){
                printf("送信できません。\n");
            }
        }
    }
    //BallSensorの値を要求
    //uart_write_blocking(uart1,(uint8_t[]){0x01},1);
    uart_putc(uart1,0x01); 
    
    uint8_t data = 0;
    if(SerialWatch == 'b'){
        printf("データを待っています");
    }
    while(!uart_is_readable(uart1)){}

    uart_read_blocking(uart1,&data,1); 
    if(data == PICO_ERROR_TIMEOUT || data == PICO_ERROR_GENERIC){
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
        }else if(data % 64 == 62){
            BallAngle = 999;
        }else {
            BallAngle = data % 64 * 11.25;
        }
        if (SerialWatch == 'b') {
            printf("data : 0x%x BallDistance : %d BallAngle : %f\n",data,BallDistance,BallAngle);
        }
    }
}

void UseBLE(){
    if(isBLE == true){
        if(UsedBLE == false || !(mode == 1 || mode == 2 || mode == 9 || mode == 10)){
            int kurikaesi = 0;
            while (!uart_is_writable(uart1)) {  
                kurikaesi++;
                if(kurikaesi > 100){
                    kurikaesi = 0;
                    //uart_flush();
                    if(SerialWatch == 'B'){
                        printf("送信できません\n");
                    }
                }
            }
            if(mode == 1 || mode == 2 || mode == 99){
                uart_putc(uart1,0x02); 
            }else{
                uart_putc(uart1,0x03); 
            }

            uint8_t data = 0;
            if(SerialWatch == 'B'){
                printf("データを待っています");
            }
            while(!uart_is_readable(uart1)){}

            uart_read_blocking(uart1,&data,1);

            if(data == PICO_ERROR_TIMEOUT || data == PICO_ERROR_GENERIC){
                //BallAngle = -999;
                if(SerialWatch == 'B'){
                    printf("データの受信に失敗しました\n");
                }
            }else{
                if(data == 0x01){
                    UsedBLE = true;
                    if(SerialWatch == 'B'){
                        printf("BLEは正常です。\n");
                    }
                }else if(data == 0x02){
                    if(SerialWatch == 'B'){
                        printf("BLEの接続に失敗しています。\n");
                    }
                }else if(data == 0x03){
                    mode += 4;
                    UsedBLE = false;
                    if(SerialWatch == 'B'){
                        printf("アタッカーに代わります。\n");
                    }
                    uart_putc(uart1,0x04); 
                    while(!uart_is_readable(uart1)){
                        if(SerialWatch == 'B'){
                            printf("データを待っています\n");
                        }
                    }
                    uart_read_blocking(uart1,&data,1); 
                    if(data == PICO_ERROR_TIMEOUT || data == PICO_ERROR_GENERIC){
                        if(SerialWatch == 'B'){
                            printf("データの受信に失敗しました\n");
                        }
                    }else{
                        if(SerialWatch == 'B'){
                            printf("アタッカーに替われました。\n");
                        }
                    }
                }else if(data == 0x06){
                    //待機する
                    while(!uart_is_readable(uart1)){
                        Brake();
                    }

                    uart_read_blocking(uart1,&data,1);

                    if(data == 0x01){
                        UsedBLE = true;
                        if(SerialWatch == 'B'){
                            printf("BLEは正常です。\n");
                        }
                    }
                } 
            }
        }
    }   
}