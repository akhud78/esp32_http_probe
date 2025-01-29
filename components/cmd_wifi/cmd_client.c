// https://github.com/espressif/esp-idf/blob/v5.4/examples/protocols/esp_http_client/main/esp_http_client_example.c
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "esp_console.h"
#include "esp_log.h"
#include "cmd_nvs.h" 
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_http_client.h"
#include "argtable3/argtable3.h"
#include "cmd_client.h"

#define CLIENT_URL_MAX_LEN      120
#define CLIENT_BUFFER_SIZE      (1024 * 40)

static const char *TAG = "cmd_client";

static struct {
    struct arg_str *url;
    struct arg_int *laps;
    struct arg_end *end;
} client_args;

/** 'client' command  */
/*
    esp32> nvs_set url str -v  http://httpbin.org/stream/6
    esp32> nvs_set url str -v  http://esp32-fs.local/image.jpeg
    esp32> nvs_list nvs
    esp32> client http://httpbin.org/get
    nvs_get url str
*/

static int _read(char *url, bool show)
{
    char *buffer = malloc(CLIENT_BUFFER_SIZE + 1);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Cannot malloc http receive buffer");
        return -1;
    }
    
    esp_http_client_config_t config = {
        .auth_type = HTTP_AUTH_TYPE_BASIC,
        .max_authorization_retries = -1,
        //.buffer_size = 1024*4,
    };
    
    //ESP_LOGI(TAG, "url='%s'", url);
    config.url = url;
    
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        free(buffer);
        return -1; 
    }  
    
    esp_err_t err;    
    if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        return -1;
    }
    int read_len = 0;
    //bool chunked = false;
        
    int content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0) {
        ESP_LOGE(TAG, "HTTP client fetch headers failed");
        free(buffer);
        return -1;
    } else if (content_length == 0 && esp_http_client_is_chunked_response(client)) { 
        //chunked = true;
        
        read_len = esp_http_client_read_response(client, buffer, CLIENT_BUFFER_SIZE);
        /*
        while (read_len < CLIENT_BUFFER_SIZE) {
            //int data_read = esp_http_client_read(client, buffer + read_len, CLIENT_BUFFER_SIZE - read_len);
            int data_read = esp_http_client_read(client, buffer + read_len, 1024*4);
            ESP_LOGI(TAG, "data_read=%d", data_read);
            if (data_read <= 0) {
                break;
            }
            read_len += data_read;
        } // while
        */
    } else if (content_length <= CLIENT_BUFFER_SIZE) {
        read_len = esp_http_client_read(client, buffer, content_length);
    }
    int status_code = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG, "status_code=%d bytes=%d", status_code, read_len);

    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    free(buffer);

    if (read_len < 0) {
        ESP_LOGE(TAG, "Error read data");
    }
    return read_len;
}


// https://github.com/espressif/esp-idf/blob/v5.4/examples/protocols/esp_http_client/main/esp_http_client_example.c#L589
static int client_get(int argc, char **argv)
{    
    int nerrors = arg_parse(argc, argv, (void **) &client_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, client_args.end, argv[0]);
        return 1;
    }
    
    char url[CLIENT_URL_MAX_LEN] = "";
    strlcpy(url, client_args.url->sval[0],  sizeof(url));
    
    if (!strlen(url)) {
        if (get_str_from_nvs("url", url, sizeof(url)) != ESP_OK) {
            ESP_LOGE(TAG, "No such entry was found");
            return -1;
        }
    }
    
    int laps = 1;
    if (client_args.laps->count > 0) {
        laps = (uint32_t)(client_args.laps->ival[0]);
    }
    
    int run_time_total_ms = 0;
    int bytes_total = 0;
    for (int i=0; i<laps; i++) {
        
        uint64_t start = esp_timer_get_time();
        int bytes = _read(url, false);
        uint64_t end = esp_timer_get_time();
        
        if (bytes <= 0) {
            break;
        }
        int run_time_ms = (end - start) / 1000;
        run_time_total_ms += run_time_ms;
        bytes_total += bytes;
        
    }
    int average_time = run_time_total_ms / laps;
    int average_bps = bytes_total /  run_time_total_ms * 1000 * 8;
    ESP_LOGI(TAG, "laps=%d run_time_total_ms=%d average_time=%d average_bps=%d", 
        laps, run_time_total_ms, average_time, average_bps);
    
    return 0;
}


void register_client(void)
{
    client_args.url = arg_str0(NULL, NULL, "<url>", "URL base");
    client_args.laps = arg_int0("l", "laps", "<n>", "number of laps");
    client_args.end = arg_end(1);
    
    const esp_console_cmd_t client_cmd = {
        .command = "client",
        .help = "HTTP Client",
        .hint = NULL,
        .func = &client_get,
        .argtable = &client_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&client_cmd) );
}

