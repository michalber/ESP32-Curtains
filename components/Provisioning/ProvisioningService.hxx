#ifndef __PROVISIONING_SERVICE_HXX__
#define __PROVISIONING_SERVICE_HXX__

#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include "nvs_flash.h"

#include <wifi_provisioning/manager.h>

#ifdef CONFIG_EXAMPLE_PROV_TRANSPORT_BLE
#include <wifi_provisioning/scheme_ble.h>
#endif /* CONFIG_EXAMPLE_PROV_TRANSPORT_BLE */

#ifdef CONFIG_EXAMPLE_PROV_TRANSPORT_SOFTAP
#include <wifi_provisioning/scheme_softap.h>
#endif /* CONFIG_EXAMPLE_PROV_TRANSPORT_SOFTAP */

namespace service
{

constexpr const char TAG[] = "provisioning";

class ProvisioningService
{

public:
    ProvisioningService();
    ~ProvisioningService();
    void startProvisioning();
private:
    void setup();
    static void event_handler(void* arg, esp_event_base_t event_base,
                          int event_id, void* event_data);
    static void wifi_init_sta(void);
    static void get_device_service_name(char *service_name, size_t max);                          
};

} // namespace provisioning

#endif //__PROVISIONING_SERVICE_HXX__