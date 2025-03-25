#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "motor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "../config.h"

float DribblerDuty = 0;

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
      analogWrite(MMpin1_1, speed);
      analogWrite(MMpin1_2, speed);
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
      
        analogWrite(MMpin2_1, speed);
        analogWrite(MMpin2_2, speed);
      
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
      
        analogWrite(MMpin3_1, speed);
        analogWrite(MMpin3_2, speed);
      
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
      analogWrite(MMpin4_1, speed);
      analogWrite(MMpin4_2, speed);
    }
  }
}

void DribblerMotorState(int state,int speed){
  if(state == 0 && DribblerDuty < 0){
    DribblerDuty = 0;
  }else if(state == 1 && DribblerDuty > 0){
    DribblerDuty = 0;
  }
  while(fabsf(speed - fabsf(DribblerDuty)) > 1 && state != 3){
    if(state == 0){
      DribblerDuty += (speed - DribblerDuty) * 0.3;
    }else if(state == 1){
      DribblerDuty -= (speed + DribblerDuty) * 0.3;
    }else if(state == 2){
      DribblerDuty -= DribblerDuty * 0.3;
    }
    if(DribblerDuty > 255){
      analogWrite(DMpin1, 255);
      analogWrite(DMpin2, 0);
    }else if(DribblerDuty >= 0){
      analogWrite(DMpin1, (int)(DribblerDuty));
      analogWrite(DMpin2, 0);
    }else if(DribblerDuty > -255){
      analogWrite(DMpin1, 0);
      analogWrite(DMpin2, abs((int)(DribblerDuty)));
    }else{
      analogWrite(DMpin1, 0);
      analogWrite(DMpin2, 255);
    }
  }  
  if(state == 3){
      analogWrite(DMpin1, speed);
      analogWrite(DMpin2, speed);
      DribblerDuty = 0;
    }else{
      if(DribblerDuty > 255){
        analogWrite(DMpin1, 255);
        analogWrite(DMpin2, 0);
      }else if(DribblerDuty >= 0){
        analogWrite(DMpin1, (int)(DribblerDuty));
        analogWrite(DMpin2, 0);
      }else if(DribblerDuty > -255){
        analogWrite(DMpin1, 0);
        analogWrite(DMpin2, abs((int)(DribblerDuty)));
      }else{
        analogWrite(DMpin1, 0);
        analogWrite(DMpin2, 255);
      }
    }
}

//周波数をf[Hz]とすると
//(pico2)150×1000×1000 = f × clkdiv × (warp + 1) clkdiv = 588.235
//(pico) 125×1000×1000 = f × clkdiv × (warp + 1) clkdiv = 488.281
//よって今は f = 1.0[kHz]
void analogWrite(int gpio,int duty){
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    uint channel = pwm_gpio_to_channel(gpio);
    pwm_set_clkdiv(slice_num, 588.235);
    pwm_set_wrap(slice_num, 255);
    pwm_set_chan_level(slice_num, channel, duty);
    pwm_set_enabled(slice_num, true);
}