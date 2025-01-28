#include <stdio.h>
#include "../config.h"
#include "gyro.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define BNO_ADDRESS 0x28

void GyroSetup(){
    i2c_init(i2c1,115200);
    gpio_set_function(SDA1pin, GPIO_FUNC_I2C);
    gpio_set_function(SCL1pin, GPIO_FUNC_I2C);
    i2c_write_blocking(i2c1,BNO_ADDRESS,(uint8_t[]){0x00},1,true);
    uint8_t chip_id;
    i2c_read_blocking(i2c1, BNO_ADDRESS, &chip_id, 1, false); 
    if (chip_id != 0xA0) {
        while (true) {
            printf("bno055が見つかりません。\n");
            sleep_ms(1000);
        }
    }
    uint8_t config[][3] = {
        {0x3D, 0x00, 80},
        {0x3E, 0x00, 80},
        {0x07, 0x00, 80},
        {0x3D, 0x08, 80}
    };
    for (int i = 0; i < 4; i++) {
        uint8_t reg = config[i][0];
        uint8_t value = config[i][1];
        uint8_t delay = config[i][2];

        uint8_t data[2] = {reg, value};
        i2c_write_blocking(i2c1, BNO_ADDRESS, data, 2, false); 
        sleep_ms(delay); 
    }
    printf("bno055は正常に起動しました。\n");
}

void UseGyroSensor(){
    uint8_t buffer[2];
    i2c_write_blocking(i2c1, BNO_ADDRESS, (uint8_t[]){0x1A}, 1, true); 
    i2c_read_blocking(i2c1, BNO_ADDRESS, buffer, 2, false); 
    AngleX = ((buffer[1] << 8) | buffer[0]) / 16.0;
    if(SerialWatch == 'a'){
        printf("AngleX : %f\n",AngleX);
    }
    if(mode == 2 || mode == 4){
        if (AngleX > 180){
            AngleX -= 180;
        }else{
            AngleX += 180;
        }
    }
}