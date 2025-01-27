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
    struct arg_str *url_base;
    struct arg_int *laps;
    struct arg_end *end;
} client_args;

/** 'client' command  */
/*
    esp32> nvs_set url_base str -v  http://httpbin.org/stream/6
    esp32> nvs_set url_base str -v  http://esp32-fs.local/image.jpeg
    esp32> nvs_list nvs
    esp32> client http://httpbin.org/get
    nvs_get url_base str
*/

static int _read(esp_http_client_handle_t client, char *buffer, int len)
{
    esp_err_t err;    
    if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        return -1;
    }
    int read_len = 0;
        
    uint64_t start = esp_timer_get_time();
    
    int content_length = esp_http_client_fetch_headers(client);
    if (content_length < 0) {
        ESP_LOGE(TAG, "HTTP client fetch headers failed");
    } else if (content_length == 0 && esp_http_client_is_chunked_response(client)) { 
        ESP_LOGI(TAG, "Chunked encoding stream");
        
        read_len = esp_http_client_read_response(client, buffer, len);
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
    } else if (content_length <= len) {
        read_len = esp_http_client_read(client, buffer, content_length);
    }
    ESP_LOGI(TAG, "Status = %d", esp_http_client_get_status_code(client));

    uint64_t end = esp_timer_get_time();
    
    if (read_len < 0) {
        ESP_LOGE(TAG, "Error read data");
    } else {
        buffer[read_len] = 0;
        ESP_LOGI(TAG, "read_len = %d", read_len);
        
        int run_time_ms = (end - start) / 1000;
        int bps = read_len * 1000 * 8 / run_time_ms;
        ESP_LOGI(TAG, "bytes=%d run_time_ms=%d bps=%d", read_len, run_time_ms, bps);        
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
    
    char url_base[CLIENT_URL_MAX_LEN] = "";
    strlcpy(url_base, client_args.url_base->sval[0],  sizeof(url_base));
    
    if (!strlen(url_base)) {
        if (get_str_from_nvs("url_base", url_base, sizeof(url_base)) != ESP_OK) {
            ESP_LOGE(TAG, "No such entry was found");
            return -1;
        }
    }
    
    int laps = 1;
    if (client_args.laps->count > 0) {
        laps = (uint32_t)(client_args.laps->ival[0]);
    }
    
    char *buffer = malloc(CLIENT_BUFFER_SIZE + 1);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Cannot malloc http receive buffer");
        return -1;
    }
    
    esp_http_client_config_t config = {
        .auth_type = HTTP_AUTH_TYPE_BASIC,
        .max_authorization_retries = -1,
        .buffer_size = 1024*4,
    };
    
    ESP_LOGI(TAG, "url_base='%s'", url_base);
    config.url = url_base;
    
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        return -1; 
    }  
    
    for (int i=0; i<laps; i++) {
        _read(client, buffer, CLIENT_BUFFER_SIZE);
    }

    esp_http_client_close(client);
    esp_http_client_cleanup(client);
    free(buffer);
    
    return 0;
}


void register_client(void)
{
    client_args.url_base = arg_str0(NULL, NULL, "<url_base>", "URL base");
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

