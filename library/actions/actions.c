#include "motor.h"
#include "actions.h"

int MotorDuty[4];

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