#include <stdio.h>
#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../config.h"

void MotorSetup(){
  gpio_set_function(MMpin1_1,GPIO_FUNC_PWM);
  gpio_set_function(MMpin1_2,GPIO_FUNC_PWM);
  gpio_set_function(MMpin2_1,GPIO_FUNC_PWM);
  gpio_set_function(MMpin2_2,GPIO_FUNC_PWM);
  gpio_set_function(MMpin3_1,GPIO_FUNC_PWM);
  gpio_set_function(MMpin3_2,GPIO_FUNC_PWM);
  gpio_set_function(MMpin4_1,GPIO_FUNC_PWM);
  gpio_set_function(MMpin4_2,GPIO_FUNC_PWM);
  gpio_set_function(DMpin1,GPIO_FUNC_PWM);
  gpio_set_function(DMpin2,GPIO_FUNC_PWM);
}

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

void DribblerMotorState(int state,int speed){
  if (state == 0) {
    analogWrite(DMpin1, speed);
    analogWrite(DMpin2, 0);
  } else if (state == 1) {
    analogWrite(DMpin1, 0);
    analogWrite(DMpin2, speed);
  } else if (state == 2) {
    analogWrite(DMpin1, 0);
    analogWrite(DMpin2, 0);
  } else if (state == 3) {
    if(speed == 255){
        gpio_put(DMpin1, 1);
        gpio_put(DMpin2, 1);
      }else{
        analogWrite(DMpin1, speed);
        analogWrite(DMpin2, speed);
      }
  }
}

void analogWrite(int gpio,int duty){
  uint slice_num = pwm_gpio_to_slice_num(gpio);
  uint channel = pwm_gpio_to_channel(gpio);
  pwm_set_clkdiv(slice_num, 124);
  pwm_set_wrap(slice_num, 2047);
  pwm_set_chan_level(slice_num, channel, duty*8);
}