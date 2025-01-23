#include <stdio.h>
#include "line.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../config.h"
#include "actions.h"

#define MCP23017_ADDRESS_1 0x20
#define MCP23017_ADDRESS_2 0x21

int LineSetup(){
    i2c_init(i2c0,400000);
    gpio_set_function(SDA0pin, GPIO_FUNC_I2C);
    gpio_set_function(SCL0pin, GPIO_FUNC_I2C);
    int i2c_result;
    i2c_result = i2c_write_blocking(i2c0,MCP23017_ADDRESS_1,(uint8_t[]){0x05,0x80},2,true);
    if(i2c_result == PICO_ERROR_GENERIC || i2c_result == PICO_ERROR_TIMEOUT){
        return i2c_result;
    }
    i2c_write_blocking(i2c0,MCP23017_ADDRESS_2,(uint8_t[]){0x05,0x80},2,true);
    if(i2c_result == PICO_ERROR_GENERIC || i2c_result == PICO_ERROR_TIMEOUT){
        return i2c_result;
    }
}

void UseLineSensor(){
    i2c_write_raw_blocking(i2c0,(uint8_t[]){MCP23017_ADDRESS_1,0x09},2);
    uint8_t data1 = i2c_read_byte_raw(i2c0);
    i2c_write_raw_blocking(i2c0,(uint8_t[]){MCP23017_ADDRESS_1,0x19},2);
    uint8_t data2 = i2c_read_byte_raw(i2c0);
    for(int a = 0;a < 8;a++){
        LineSensorABCD[a] = (data1 >> a) & 1 ;
        if(SerialWatch == 'l')printf("%d",LineSensorABCD[a]);
    }
    for(int a = 8;a < 16;a++){
        LineSensorABCD[a] = (data1 >> a-8) & 1 ;
        if(SerialWatch == 'l')printf("%d",LineSensorABCD[a]);
    }
    i2c_write_raw_blocking(i2c0,(uint8_t[]){MCP23017_ADDRESS_2,0x09},2);
    data1 = i2c_read_byte_raw(i2c0);
    i2c_write_raw_blocking(i2c0,(uint8_t[]){MCP23017_ADDRESS_2,0x19},2);
    data2 = i2c_read_byte_raw(i2c0);
    for(int a = 0;a < 8;a++){
        LineSensorE[a] = (data1 >> a) & 1 ;
        if(SerialWatch == 'l')printf("%d",LineSensorE[a]);
    }
    for(int a = 8;a < 16;a++){
        LineSensorE[a] = (data1 >> a-8) & 1 ;
        if(SerialWatch == 'l')printf("%d",LineSensorE[a]);
    }
    if(SerialWatch == 'l') printf("\n");
    AllLineSensorA = LineSensorABCD[0] + LineSensorABCD[1] + LineSensorABCD[2] + LineSensorABCD[3];
    AllLineSensorB = LineSensorABCD[4] + LineSensorABCD[5] + LineSensorABCD[6] + LineSensorABCD[7];
    AllLineSensorC = LineSensorABCD[8] + LineSensorABCD[9] + LineSensorABCD[10] + LineSensorABCD[11];
    AllLineSensorD = LineSensorABCD[12] + LineSensorABCD[13] + LineSensorABCD[14] + LineSensorABCD[15];
    AllLineSensorE = LineSensorE[0] + LineSensorE[1] + LineSensorE[2] + LineSensorE[3] + LineSensorE[4] + LineSensorE[5] + LineSensorE[6] + LineSensorE[7] + LineSensorE[8] + LineSensorE[9] + LineSensorE[10] + LineSensorE[11] + LineSensorE[12] + LineSensorE[13] + LineSensorE[14] + LineSensorE[15];
    AllLineSensor = AllLineSensorA + AllLineSensorB + AllLineSensorC + AllLineSensorD + AllLineSensorE;
    if(AllLineSensorE > 12){
        Brake();
        printf("ラインセンサに異常が起きています");
        gpio_put(Bupin,1);
        while(1){}
    }
}