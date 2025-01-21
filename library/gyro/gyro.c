#include <stdio.h>
#include "../config.h"
#include "gyro.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

float AngleX = 0, AngleY, AngleZ;
__uint8_t a, b;
int c;
#define bno055 Serial1
#define BNO_DELAY 400     //delayMicroseconds
#define BNO_WAIT_DELAY 3  //delay

void UseGyroSensor(){
    i2c_init(i2c1,115200);
    gpio_set_function(SDA1pin, GPIO_FUNC_I2C);
    gpio_set_function(SCL1pin, GPIO_FUNC_I2C);
    gpio_pull_up(SDA1pin);
    gpio_pull_up(SCL1pin);
    i2c_write_blocking(i2c1,);
}