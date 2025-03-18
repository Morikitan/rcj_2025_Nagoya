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

uint32_t PreTime = 0;

int main()
{
    stdio_init_all();
    
    VariableSetup();
    PinSetup();
    gpio_put(Bupin,1);
    
    CameraSetup();
    //GyroSetup();
    //int ReturnData = LineSetup();
    //printf("i2c : %d\n",ReturnData);
    //MotorSetup();
    //Nano33IoTSetup();
    gpio_put(Bupin,0);
    while (true) {
        if(SerialWatch == 't'){
            printf("経過時間%fミリ秒\n",(time_us_32()-PreTime)/1000.0);
            PreTime = time_us_32();
        }
        if(mode = 0){
            if(gpio_get(TSpin1 == 1)){
                mode = 1;
                //割り込み処理を行う
                gpio_set_irq_enabled_with_callback(TSpin6,GPIO_IRQ_EDGE_RISE,true,&Attack);

                DribblerMotorState(0,255);
            }else if(gpio_get(TSpin2 == 1)){
                mode = 2;
                //割り込み処理を行う
                gpio_set_irq_enabled_with_callback(TSpin6,GPIO_IRQ_EDGE_RISE,true,&Attack);
                
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
        }else if(mode == 7 || mode == 8){
            //ディフェンスからアタッカーに替わるときの初期化
            float PreTime = time_us_32() / 1000000.0;
            float Time = 0;
            while(Time < 0.1){
                UseLineSensor();
                UseGyroSensor();
                MotorDuty[0] = DefaultSpeed;
                MotorDuty[1] = DefaultSpeed;
                MotorDuty[2] = DefaultSpeed;
                MotorDuty[3] = DefaultSpeed;
                Turn();
                UseMotorDuty();
                if(AllLineSensor <= ErorrLineSensor){
                    Time += time_us_32() / 1000000.0 - PreTime;
                }
                PreTime = time_us_32() / 1000000.0;
            }
            mode -= 6;
            //割り込み処理を行う
            gpio_set_irq_enabled_with_callback(TSpin6,GPIO_IRQ_EDGE_RISE,true,&LineMove);    
        }else{
            UseCamera();
        }
    }
}