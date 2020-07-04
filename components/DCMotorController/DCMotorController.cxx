#include "DCMotorController.hxx"

namespace controller 
{
    DCMotorController::DCMotorController()
    {
        gpio_config_t io_conf;
        //disable interrupt
        io_conf.intr_type = (gpio_int_type_t)GPIO_PIN_INTR_DISABLE;
        //set as output mode
        io_conf.mode = GPIO_MODE_OUTPUT;
        //bit mask of the pins that you want to set,e.g.GPIO18/19
        io_conf.pin_bit_mask = (1ULL<<GPIO_MOTOR_EN);
        //disable pull-down mode
        io_conf.pull_down_en = (gpio_pulldown_t)0;
        //disable pull-up mode
        io_conf.pull_up_en = (gpio_pullup_t)0;
        //configure GPIO with the given settings
        gpio_config(&io_conf);

        //1. mcpwm gpio initialization
        printf("initializing mcpwm gpio...\n");
        mcpwm_gpio_init((mcpwm_unit_t)MOTOR_MCPWM_UNIT, MCPWM0A, GPIO_PWM0A_OUT);
        mcpwm_gpio_init((mcpwm_unit_t)MOTOR_MCPWM_UNIT, MCPWM0B, GPIO_PWM0B_OUT);

        //2. initial mcpwm configuration
        ESP_LOGI(TAG, "Configuring Initial Parameters of mcpwm...");
        mcpwm_config_t pwm_config;
        pwm_config.frequency = 30000; //frequency = 500Hz,
        pwm_config.cmpr_a = 0;       //duty cycle of PWMxA = 0
        pwm_config.cmpr_b = 0;       //duty cycle of PWMxb = 0s
        pwm_config.counter_mode = MCPWM_UP_COUNTER;
        pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
        mcpwm_init((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, &pwm_config); //Configure PWM0A & PWM0B with above settings

        for (int i = 0; i < numOfSteps; i++)
        {
            motorMapSoftStart[i] = maxPWM * (1.0f - exp(i * -dXVal));
            motorMapSoftStop[i] = maxPWM * (-exp((i + 1) * dXVal - 3.0f) + 1.0f);
        }

        // Activate motor
        this->enableMotor(true);
    }

    DCMotorController::~DCMotorController()
    {
    }

    void DCMotorController::forward(float duty_cycle = 100.0f)
    {
        ESP_LOGI(TAG, "Forward");
        esp_err_t err = ESP_OK;
        err = mcpwm_set_signal_low((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_B);
        // err = mcpwm_set_signal_low((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_A);
        err = mcpwm_set_duty((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_A, duty_cycle);
        err = mcpwm_set_duty_type((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void DCMotorController::backward(float duty_cycle = 100.0f)
    {
        ESP_LOGI(TAG, "Backward");
        mcpwm_set_signal_low((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_A);
        mcpwm_set_duty((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_B, duty_cycle);
        mcpwm_set_duty_type((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state
    }

    void DCMotorController::stop()
    {
        mcpwm_set_signal_low((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_A);
        mcpwm_set_signal_low((mcpwm_unit_t)MOTOR_MCPWM_UNIT, (mcpwm_timer_t)MOTOR_TIMER_UNIT, MCPWM_OPR_B);
    }

    void DCMotorController::softStart(bool backward = false)
    {
        for (int i = 0; i < numOfSteps; i++)
        {
            if(backward)
            {
                this->backward(motorMapSoftStart[i]);
            }
            else
            {                        
                this->forward(motorMapSoftStart[i]);
            }
            calculateDistance(motorMapSoftStart[i]);
            vTaskDelay((timeStep));
        }
    }
    
    void DCMotorController::softStop(bool backward = false)
    {
        for (int i = 0; i < numOfSteps; i++)
        {
            if(backward)
            {
                this->backward(motorMapSoftStop[i]);
            }
            else
            {                        
                this->forward(motorMapSoftStop[i]);
            }
            calculateDistance(motorMapSoftStop[i]);
            vTaskDelay((timeStep));
        }
    }

    float DCMotorController::calculateDistance(float PWM)
    {
        distance += PWM / 100.0f * linearMaxSpeed * (timeStep / 1000.0f);
        return distance;
    }

    void DCMotorController::open()
    {
        softStart();
        while (1)
        {
            forward(maxPWM);
            if (distance > 1.0f)
            {
                break;
            }
            else
            {
                vTaskDelay((timeStep));
            }
        }
        softStop();
        stop();
        distance = 0.0f;
    }

    void DCMotorController::close()
    {
        softStop(true);
        while (1)
        {
            backward(maxPWM);
            if (distance > 1.0f)
            {
                break;
            }
            else
            {
                vTaskDelay((timeStep));
            }
        }
        softStop(true);
        stop();
        distance = 0.0f;
    }

    void DCMotorController::enableMotor(bool enable = true)
    {
        if(enable)
        {
            gpio_set_level((gpio_num_t)GPIO_MOTOR_EN, 1);
        }
        else
        {
            gpio_set_level((gpio_num_t)GPIO_MOTOR_EN, 0);
        }
        
    }

} // namespace DCMotorController