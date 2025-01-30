#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"

#include "cmd_system.h"
#include "cmd_wifi.h"
#include "cmd_rest.h"
#include "cmd_client.h"
#include "cmd_request.h"
#include "cmd_ping.h"
#include "cmd_nvs.h"

#include "led.h"
#include "app.h"

static const char *TAG = "app";


void setup(void)
{
    ESP_LOGI(TAG, "%s", __func__); 
    
    initialize_nvs();
    led_configure();
        
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_blink_on();
    vTaskDelay(pdMS_TO_TICKS(1000));
    led_blink_off();
    
    //foo_init();
}

/*  
  esp32> nvs_set ssid str -v Hotspot-5413
  esp32> nvs_set pass str -v fHMUja9a
  esp32> nvs_list nvs
  ...
  namespace 'storage', key 'ssid', type 'str' 
  namespace 'storage', key 'pass', type 'str'
*/

void register_commands(void)
{
    register_system();
    register_wifi();
    register_ping();
    register_nvs();
    register_rest();
    register_client();
    register_request();
}
