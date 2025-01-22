#include <stdio.h>
#include "line.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../config.h"

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

}