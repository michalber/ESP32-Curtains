#ifndef __BLYNK_CONTROLLER_HXX__
#define __BLYNK_CONTROLLER_HXX__

#include "blynk.h"

namespace controller
{
    constexpr char BLYNK_TOKEN[] = CONFIG_BLYNK_TOKEN;
    constexpr char BLYNK_SERVER[] =  CONFIG_BLYNK_SERVER;
    class BlynkController
    {
        blynk_client_t *client;
    public:
        BlynkController();
        ~BlynkController();
    private:
        static void state_handler(blynk_client_t *c, const blynk_state_evt_t *ev, void *data);
        static void vw_handler(blynk_client_t *c, uint16_t id, const char *cmd, int argc, char **argv, void *data);
        static void vr_handler(blynk_client_t *c, uint16_t id, const char *cmd, int argc, char **argv, void *data);        
    };
} // namespace controller

#endif //__BLYNK_CONTROLLER_HXX__