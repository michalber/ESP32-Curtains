#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "DCMotorController.hxx"


extern "C" {
    void app_main(void);
}

void app_main(void)
{
    controller::DCMotorController motorController;
    int i = 0;    
    while (1) {
        motorController.softStart(false);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.softStop(false);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.forward(30.0);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.stop();

        motorController.softStart(true);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.softStop(true);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.backward(30.0);
        vTaskDelay(2000 / portTICK_RATE_MS);
        motorController.stop();
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}
