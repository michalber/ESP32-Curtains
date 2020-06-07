#ifndef __DC_MOTOR_CONTROLLER__
#define __DC_MOTOR_CONTROLLER__

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_log.h"
#include <string>

namespace DCMotorController
{
    constexpr int GPIO_PWM0A_OUT = ESP_MOTOR_OUT_A; //Set GPIO 15 as PWM0A
    constexpr int GPIO_PWM0B_OUT = ESP_MOTOR_OUT_B; //Set GPIO 16 as PWM0B
    constexpr int MOTOR_MCPWM_UNIT = MCPWM_UNIT_0;
    constexpr int MOTOR_TIMER_UNIT = MCPWM_TIMER_0;
    constexpr std::string TAG = "DCMotorController";

    class DCMotorController
    {
        public:
            DCMotorController();
            ~DCMotorController();

            forward(float duty_cycle);
            backward(float duty_cycle);
            stop();
            calibrate_forward();
            calibrate_backward();
        private:
    };
} // namespace DCMotorController

#endif //__DC_MOTOR_CONTROLLER__