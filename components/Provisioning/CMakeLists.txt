idf_component_register(SRCS "ProvisioningService.cxx"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES nvs_flash wifi_provisioning esp_wifi esp_event
                       )