#include "BlynkController.hxx"

namespace controller
{
    BlynkController::BlynkController()
    {
        client = malloc(sizeof(blynk_client_t));        
        blynk_init(client);

        blynk_options_t opt = {
		    .token = BLYNK_TOKEN,
		    .server = BLYNK_SERVER,
		    /* Use default timeouts */
	    };

	    blynk_set_options(client, &opt);

	    /* Subscribe to state changes and errors */
	    blynk_set_state_handler(client, BlynkController::state_handler, NULL);

	    /* blynk_set_handler sets hardware (BLYNK_CMD_HARDWARE) command handler */
	    blynk_set_handler(client, "vw", BlynkController::vw_handler, NULL);
	    blynk_set_handler(client, "vr", BlynkController::vr_handler, NULL);

	    /* Start Blynk client task */
	    blynk_start(client);
    }

    BlynkController::~BlynkController()
    {
        free(client);
    }

    /* Blynk client state handler */
static void state_handler(blynk_client_t *c, const blynk_state_evt_t *ev, void *data) {
	ESP_LOGI(tag, "state: %d\n", ev->state);
}

/* Virtual write handler */
static void vw_handler(blynk_client_t *c, uint16_t id, const char *cmd, int argc, char **argv, void *data) {
	if (argc > 1 && atoi(argv[0]) == VP_PWM) {
		uint32_t value = atoi(argv[1]);

		/* Update PWM channel */
		ledc_set_duty(PWM_MODE, PWM_CHANNEL, value);
		ledc_update_duty(PWM_MODE, PWM_CHANNEL);
	}
}

/* Virtual read handler */
static void BlynkController::vr_handler(blynk_client_t *c, uint16_t id, const char *cmd, int argc, char **argv, void *data) {
	if (!argc) {
		return;
	}

	int pin = atoi(argv[0]);

	switch (pin) {
		case VP_ADC:
		{
			/* Get ADC value */
			int value = adc1_get_voltage(ADC_CHANNEL);

			/* Respond with `virtual write' command */
			blynk_send(c, BLYNK_CMD_HARDWARE, 0, "sii", "vw", VP_ADC, value);
			break;
		}

		case VP_UPTIME:
		{
			unsigned long value = (unsigned long long)xTaskGetTickCount() * portTICK_RATE_MS / 1000;

			/* Respond with `virtual write' command */
			blynk_send(c, BLYNK_CMD_HARDWARE, 0, "siL", "vw", VP_UPTIME, value);
			break;
		}
	}
}
}