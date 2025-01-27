#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"

#include "app.h"

extern "C" void app_main(void) 
{
    ESP_LOGI("","%s Build: %s @ %s", __func__ , __DATE__, __TIME__);
    
    setup();
    
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    
    repl_config.prompt = "esp32>";
    repl_config.task_stack_size =  1024 * 6;
    
    
    //esp_log_level_set("*", ESP_LOG_INFO);       // set all components to INFO level
    esp_log_level_set("esp_http_client", ESP_LOG_DEBUG);    // enable DEBUG logs for component foo
    
    
#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
    esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_CDC)
    esp_console_dev_usb_cdc_config_t hw_config = ESP_CONSOLE_DEV_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&hw_config, &repl_config, &repl));

#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));

#endif    
    
    register_commands();
    
    // Start console REPL
    esp_console_start_repl(repl);       
    
    while (1) {
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}




