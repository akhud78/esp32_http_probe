#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "esp_console.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "cmd_nvs.h" 
#include "nvs_flash.h"
#include "argtable3/argtable3.h"
#include "http_rest_client.h"
#include "cmd_rest.h"

#define REST_URL_MAX_LEN     120

static const char *TAG = "cmd_rest";

static struct {
    struct arg_str *url;
    struct arg_int *laps;
    struct arg_end *end;
} rest_args;

/** 'rest' command  */
static int _read(char *url, bool show)
{
    http_rest_recv_buffer_t response_buffer = {0};
    if (http_rest_client_get(url, &response_buffer) != ESP_OK)
        return -1;
    if (show) {
        printf("--- buffer ---\n%s\n", response_buffer.buffer);
    }
    int bytes = response_buffer.buffer_len;
    http_rest_client_cleanup(&response_buffer);
    ESP_LOGI(TAG, "status_code=%d bytes=%d", response_buffer.status_code, bytes);
    return bytes;
}

static int rest(int argc, char **argv)
{    
    int nerrors = arg_parse(argc, argv, (void **) &rest_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, rest_args.end, argv[0]);
        return 1;
    }

    char url[REST_URL_MAX_LEN] = "";
    strlcpy(url, rest_args.url->sval[0],  sizeof(url));
    
    if (!strlen(url)) {

        if (get_str_from_nvs("url", url, sizeof(url)) != ESP_OK) {
            ESP_LOGE(TAG, "No 'url' entry was found");
            return -1;
        }
    }
    
    int laps = 1;
    if (rest_args.laps->count > 0) {
        laps = (uint32_t)(rest_args.laps->ival[0]);
    }
    
    int bytes_total = 0;
    uint64_t start = esp_timer_get_time();
    for (int i=0; i<laps; i++) {
        int bytes = _read(url, false);
        if (bytes <= 0) {
            break;
        }
        bytes_total += bytes;
    }
    uint64_t end = esp_timer_get_time();
    int run_time_ms = (end - start) / 1000;
    int average_time_ms = run_time_ms / laps;
    int average_bps = (float)bytes_total /  run_time_ms * 1000 * 8;
    ESP_LOGI(TAG, "laps=%d run_time_ms=%d average_time_ms=%d average_bps=%d", 
        laps, run_time_ms, average_time_ms, average_bps);

    return 0;
}

void register_rest(void)
{
    rest_args.url = arg_str0(NULL, NULL, "<url>", "server URL");
    rest_args.laps = arg_int0("l", "laps", "<n>", "number of laps");
    rest_args.end = arg_end(1);
    
    const esp_console_cmd_t rest_cmd = {
        .command = "rest",
        .help = "HTTP REST Client",
        .hint = NULL,
        .func = &rest,
        .argtable = &rest_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&rest_cmd) );
}


