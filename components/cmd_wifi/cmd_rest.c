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
    struct arg_str *url_base;
    struct arg_end *end;
} rest_args;

/** 'rest' command  */
// https://github.com/parabuzzle/idf_http_rest_client
/*
    esp32> nvs_set url_base str -v  http://httpbin.org/stream/6
    esp32> nvs_set url_base str -v  http://esp32-fs.local/image.jpeg
    esp32> nvs_list nvs
    esp32> rest http://httpbin.org/get
    nvs_get url_base str
*/

static int rest(int argc, char **argv)
{    
    int nerrors = arg_parse(argc, argv, (void **) &rest_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, rest_args.end, argv[0]);
        return 1;
    }

    char url_base[REST_URL_MAX_LEN] = "";
    strlcpy(url_base, rest_args.url_base->sval[0],  sizeof(url_base));
    
    if (!strlen(url_base)) {

        if (get_str_from_nvs("url_base", url_base, sizeof(url_base)) != ESP_OK) {
            ESP_LOGE(TAG, "No such entry was found");
            return -1;
        }
    }
    
    ESP_LOGI(__func__, "url_base='%s'", url_base);

    http_rest_recv_buffer_t response_buffer = {0};

    uint64_t start = esp_timer_get_time();
    esp_err_t  err = http_rest_client_get(url_base, &response_buffer);
    uint64_t end = esp_timer_get_time();

    ESP_ERROR_CHECK(err);
    int run_time_ms = (end - start) / 1000;
    int bytes = response_buffer.buffer_len;
    int bps = bytes * 1000 * 8 / run_time_ms;
    ESP_LOGI(TAG, "bytes=%d run_time_ms=%d bps=%d", bytes, run_time_ms, bps);

    printf("status_code=%d\n", response_buffer.status_code);
    if (bytes < 1024*2) {  //check limit
        printf("--- buffer ---\n%s\n", response_buffer.buffer);
    }
    // clean up
    http_rest_client_cleanup(&response_buffer);
    
    return 0;
}

void register_rest(void)
{
    rest_args.url_base = arg_str0(NULL, NULL, "<url_base>", "URL base");
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


