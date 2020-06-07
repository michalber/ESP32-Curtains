#include "DCMotorController.hxx"

namespace DCMotorController
{
    DCMotorController::DCMotorController()
    {
        //1. mcpwm gpio initialization
        mcpwm_example_gpio_initialize();

        //2. initial mcpwm configuration
        ESP_LOGI(TAG, "Configuring Initial Parameters of mcpwm...");
        mcpwm_config_t pwm_config;
        pwm_config.frequency = 1000; //frequency = 500Hz,
        pwm_config.cmpr_a = 0;       //duty cycle of PWMxA = 0
        pwm_config.cmpr_b = 0;       //duty cycle of PWMxb = 0
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        mcpwm_init(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, &pwm_config); //Configure PWM0A & PWM0B with above settings
    }

    DCMotorController::~DCMotorController()
    {
    }

    DCMotorController::forward(float duty_cycle = 100.0f)
    {
        mcpwm_set_signal_low(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_B);
        mcpwm_set_duty(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_A, duty_cycle);
        mcpwm_set_duty_type(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    DCMotorController::backward(float duty_cycle = 100.0f)
    {
        mcpwm_set_signal_low(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_A);
        mcpwm_set_duty(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_B, duty_cycle);
        mcpwm_set_duty_type(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    DCMotorController::stop()
    {
        mcpwm_set_signal_low(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_A);
        mcpwm_set_signal_low(MOTOR_MCPWM_UNIT, MOTOR_TIMER_UNIT, MCPWM_OPR_B);
    }

} // namespace DCMotorController