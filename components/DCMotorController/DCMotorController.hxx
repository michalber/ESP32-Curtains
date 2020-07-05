#ifndef __DC_MOTOR_CONTROLLER_HXX__
#define __DC_MOTOR_CONTROLLER_HXX__

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_log.h"
#include <string>
#include <map>
#include <math.h>
#include "driver/gpio.h"

namespace controller
{
    constexpr int GPIO_PWM0A_OUT = CONFIG_ESP_MOTOR_OUT_A; //Set GPIO 15 as PWM0A
    constexpr int GPIO_PWM0B_OUT = CONFIG_ESP_MOTOR_OUT_B; //Set GPIO 16 as PWM0B
    constexpr int GPIO_MOTOR_EN = CONFIG_ESP_MOTOR_EN;
    constexpr int MOTOR_MCPWM_UNIT = MCPWM_UNIT_0;
    constexpr int MOTOR_TIMER_UNIT = MCPWM_TIMER_0;
    constexpr char TAG[] = "DCMotorController";

    class DCMotorController
    {
        std::map<int, float> motorMapSoftStart;
        std::map<int, float> motorMapSoftStop;
        int timeOn = 1000; // ms
        int timeStep = 10; // ms
        int numOfSteps = timeOn / timeStep;
        int maxPWM = 50;
        float dXVal = 3.0f / numOfSteps;
        const int maxRPMSpeed = 80;
        const float radius = 0.0325;
        const float linearMaxSpeed = radius * 2 * 3.14f * maxRPMSpeed / 60;
        float distance = 0.0f;

    public:
        DCMotorController();
        ~DCMotorController();

        void open();
        void close();

        void forward(float duty_cycle);
        void backward(float duty_cycle);
        void stop();
        void calibrate_forward();
        void calibrate_backward();
        void softStart(bool backward);
        void softStop(bool backward);
        float calculateDistance(float PWM);
        void enableMotor(bool enable);
    };
} // namespace DCMotorController

#endif //__DC_MOTOR_CONTROLLER_HXX__