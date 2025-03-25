#include <stdio.h>
#include "line.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "../config.h"
#include "actions.h"

#define MCP23017_ADDRESS_1 0x20
#define MCP23017_ADDRESS_2 0x21

int LineSetup(){
    i2c_init(i2c0,400000);
    gpio_set_function(SDA0pin, GPIO_FUNC_I2C);
    gpio_set_function(SCL0pin, GPIO_FUNC_I2C);
    //割り込みを有効化
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x04,0xFF,0xFF},3,false);
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x08,0xFF,0xFF},3,false);
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x0A,0x42},2,false);
    int i2c_result;
    return 0;
}

void UseLineSensor(){
    uint8_t data1;
    uint8_t data2;
    float I2CTime = time_us_32() / 1000000.0;
    bool isTimeOut = false;
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x12},1,true);
    while(i2c_get_read_available(i2c0)){
        if(time_us_32() / 1000000.0 - I2CTime > 0.1){
            isTimeOut = true;
            break;
        }
    }
    if(isTimeOut){
        if(SerialWatch == 'l')printf("0x20はエラー");
    }else{
        i2c_read_blocking(i2c0,MCP23017_ADDRESS_1,&data1,1,true);
        //i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x13},1,true);
        i2c_read_blocking(i2c0,MCP23017_ADDRESS_1,&data2,1,false);
        //printf("data1 : 0x%x data2 : 0x%x ",data1,data2);
        if(SerialWatch == 'l')printf("AB : ");
        for(int a = 0;a < 8;a++){
            LineSensorABCD[a] = (data1 >> a) & 1 ;
            if(SerialWatch == 'l')printf("%d ",LineSensorABCD[a]);
        }
        if(SerialWatch == 'l')printf("CD : ");
        for(int a = 8;a < 16;a++){
            LineSensorABCD[a] = (data2 >> a-8) & 1 ;
            if(SerialWatch == 'l')printf("%d ",LineSensorABCD[a]);
        }
    }
    isTimeOut = false;
    I2CTime = time_us_32() / 1000000.0;
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_2,(uint8_t[]){0x12},1,true);
    while(i2c_get_read_available(i2c0)){
        if(time_us_32() / 1000000.0 - I2CTime > 0.1){
            isTimeOut = true;
            break;
        }
    }
    if(isTimeOut){
        if(SerialWatch == 'l')printf("0x21はエラー");
    }else{
        i2c_read_blocking(i2c0,MCP23017_ADDRESS_2,&data1,1,true);
        i2c_read_blocking(i2c0,MCP23017_ADDRESS_2,&data2,1,false);
        if(SerialWatch == 'l')printf("E : ");
        for(int a = 0;a < 8;a++){
            LineSensorE[a] = (data1 >> a) & 1 ;
            if(SerialWatch == 'l')printf("%d ",LineSensorE[a]);
        }
        for(int a = 8;a < 16;a++){
            LineSensorE[a] = (data2 >> a-8) & 1 ;
            if(SerialWatch == 'l')printf("%d ",LineSensorE[a]);
        }
    }
    if(SerialWatch == 'l') printf("\n");
    AllLineSensorA = LineSensorABCD[0] + LineSensorABCD[1] + LineSensorABCD[2] + LineSensorABCD[3];
    AllLineSensorB = LineSensorABCD[4] + LineSensorABCD[5] + LineSensorABCD[6] + LineSensorABCD[7];
    AllLineSensorC = LineSensorABCD[8] + LineSensorABCD[9] + LineSensorABCD[10] + LineSensorABCD[11];
    AllLineSensorD = LineSensorABCD[12] + LineSensorABCD[13] + LineSensorABCD[14] + LineSensorABCD[15];
    if(AllLineSensorA + AllLineSensorB + AllLineSensorC + AllLineSensorD == 16){
        AllLineSensorA = 0;AllLineSensorB = 0;AllLineSensorC = 0;AllLineSensorD = 0;
    }
    AllLineSensorE = LineSensorE[0] + LineSensorE[1] + LineSensorE[2] + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] + LineSensorE[6] + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] + LineSensorE[10] + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] + LineSensorE[14] + LineSensorE[15];
    if(AllLineSensorE > 12){
        AllLineSensorE = 0;
    }
    AllLineSensor = AllLineSensorA + AllLineSensorB + AllLineSensorC + AllLineSensorD + AllLineSensorE;
}