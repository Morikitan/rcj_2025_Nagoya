#include <stdio.h>
#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../config.h"

void MainMotorState(int motor, int state, int speed) {
  if (motor == 1) {
    if (state == 0) {
      analogWrite(MMpin1_1, speed);
      analogWrite(MMpin1_2, 0);
    } else if (state == 1) {
      analogWrite(MMpin1_1, 0);
      analogWrite(MMpin1_2, speed);
    } else if (state == 2) {
      analogWrite(MMpin1_1, 0);
      analogWrite(MMpin1_2, 0);
    } else if (state == 3) {
      if(speed == 255){
        gpio_put(MMpin1_1, 1);
        gpio_put(MMpin1_2, 1);
      }else{
        analogWrite(MMpin1_1, speed);
        analogWrite(MMpin1_2, speed);
      }
    }
  } else if (motor == 2) {
    if (state == 0) {
      analogWrite(MMpin2_1, speed);
      analogWrite(MMpin2_2, 0);
    } else if (state == 1) {
      analogWrite(MMpin2_1, 0);
      analogWrite(MMpin2_2, speed);
    } else if (state == 2) {
      analogWrite(MMpin2_1, 0);
      analogWrite(MMpin2_2, 0);
    } else if (state == 3) {
      if(speed == 255){
        gpio_put(MMpin2_1, 1);
        gpio_put(MMpin2_2, 1);
      }else{
        analogWrite(MMpin2_1, speed);
        analogWrite(MMpin2_2, speed);
      }
    }
  } else if (motor == 3) {
    if (state == 0) {
      analogWrite(MMpin3_1, speed);
      analogWrite(MMpin3_2, 0);
    } else if (state == 1) {
      analogWrite(MMpin3_1, 0);
      analogWrite(MMpin3_2, speed);
    } else if (state == 2) {
      analogWrite(MMpin3_1, 0);
      analogWrite(MMpin3_2, 0);
    } else if (state == 3) {
      if(speed == 255){
        gpio_put(MMpin3_1, 1);
        gpio_put(MMpin3_2, 1);
      }else{
        analogWrite(MMpin3_1, speed);
        analogWrite(MMpin3_2, speed);
      }
    }
  } else if (motor == 4) {
    if (state == 0) {
      analogWrite(MMpin4_1, speed);
      analogWrite(MMpin4_2, 0);
    } else if (state == 1) {
      analogWrite(MMpin4_1, 0);
      analogWrite(MMpin4_2, speed);
    } else if (state == 2) {
      analogWrite(MMpin4_1, 0);
      analogWrite(MMpin4_2, 0);
    } else if (state == 3) {
      if(speed == 255){
        gpio_put(MMpin4_1, 1);
        gpio_put(MMpin4_2, 1);
      }else{
        analogWrite(MMpin4_1, speed);
        analogWrite(MMpin4_2, speed);
      }
    }
  }
}