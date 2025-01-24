#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"
#include "actions.h"
#include "attack.h"
#include "camera.h"
#include "defence.h"
#include "gyro.h"
#include "line.h"
#include "motor.h"
#include "Nano33IoT.h"
#include "../config.h"

int main()
{
    stdio_init_all();
    PinSetup();
    CameraSetup();
    GyroSetup();
    LineSetup();
    MotorSetup();
    Nano33IoTSetup();

    while (true) {
        
    }
}