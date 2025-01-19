#include <stdio.h>
#include "gyro.h"

float AngleX = 0, AngleY, AngleZ;
__uint8_t a, b;
int c;
#define bno055 Serial1
#define BNO_DELAY 400     //delayMicroseconds
#define BNO_WAIT_DELAY 3  //delay

void UseGyroSensor(){
    
}