#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unity.h"
#include "esp_log.h"

static void print_banner(const char* text)
{
    printf("\n### %s ####\n\n", text);
}

extern "C" void app_main(void)
{    
    printf("--- %s Build: %s @ %s ---", __func__ ,  __DATE__, __TIME__);
    
    esp_log_level_set("*", ESP_LOG_INFO);       // set all components to INFO level
    esp_log_level_set("http_client", ESP_LOG_DEBUG);    // enable DEBUG logs for component foo
    
    print_banner("Running tests with [smoke] tag"); 
    UNITY_BEGIN();
    unity_run_tests_by_tag("[smoke]", false);
    UNITY_END();
        
    print_banner("Starting interactive test menu");
    /* This function will not return, and will be busy waiting for UART input.
     * Make sure that task watchdog is disabled if you use this function.
     */
    unity_run_menu();
}
