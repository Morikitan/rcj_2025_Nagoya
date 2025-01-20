#include "motor.h"
#include "actions.h"
#include "../config.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

void Brake(){
    MainMotorState(1, 3, 255);
    MainMotorState(2, 3, 255);
    MainMotorState(3, 3, 255);
    MainMotorState(4, 3, 255);
}

void UseMotorDuty(){
  for (int a = 0; a <= 3; a++) {
    if (MotorDuty[a] > 255) {
      MainMotorState(a + 1, 0, 255);
    } else if (MotorDuty[a] > 0) {
      MainMotorState(a + 1, 0, MotorDuty[a]);
    } else if (MotorDuty[a] > -255) {
      MainMotorState(a + 1, 1, MotorDuty[a] * -1);
    } else {
      MainMotorState(a + 1, 1, 255);
    }
  }
}

void PinSetup(){
  gpio_init(TSpin1);
  gpio_init(TSpin2);
  gpio_init(TSpin3);
  gpio_init(TSpin4);
  gpio_init(TSpin5);
  gpio_init(TSpin6);
  gpio_init(Bupin);
  gpio_set_dir(TSpin1,GPIO_IN);
  gpio_set_dir(TSpin2,GPIO_IN);
  gpio_set_dir(TSpin3,GPIO_IN);
  gpio_set_dir(TSpin4,GPIO_IN);
  gpio_set_dir(TSpin5,GPIO_OUT);
  gpio_set_dir(TSpin6,GPIO_OUT);
  gpio_set_dir(Bupin,GPIO_OUT);
}