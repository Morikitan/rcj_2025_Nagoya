#include "motor.h"
#include "actions.h"

void Brake(){
    MainMotorState(1, 3, 255);
    MainMotorState(2, 3, 255);
    MainMotorState(3, 3, 255);
    MainMotorState(4, 3, 255);
}

void UseMotorDUty(){

}

void CheseBall(float angle){

}