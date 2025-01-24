#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "actions/actions.h"
#include "attack/attack.h"
#include "camera/camera.h"
#include "defence/defence.h"
#include "gyro/gyro.h"
#include "line/line.h"
#include "motor/motor.h"
#include "Nano33IoT/Nano33IoT.h"
#include "config.h"

int main()
{
    stdio_init_all();
    PinSetup();

    gpio_put(Bupin,1);

    CameraSetup();
    GyroSetup();
    LineSetup();
    MotorSetup();
    Nano33IoTSetup();

    gpio_put(Bupin,0);

    while (true) {
        if(mode = 0){
            if(gpio_get(TSpin1 == 1)){
                mode = 1;
                DribblerMotorState(0,255);
            }else if(gpio_get(TSpin2 == 1)){
                mode = 2;
                DribblerMotorState(0,255);
            }else if(gpio_get(TSpin3 == 1)){
                mode = 3;
                DribblerMotorState(1,255);
            }else if(gpio_get(TSpin4 == 1)){
                mode = 4;
                DefenceStart();
                DribblerMotorState(1,255);
            }
        }else if(mode == 1 || mode == 2){
            Attack();
        }else if(mode == 3 || mode == 4){
            Defence();
        }else{

        }
    }
}