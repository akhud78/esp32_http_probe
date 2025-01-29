// https://github.com/espressif/esp-idf/blob/v5.4/examples/protocols/http_request/main/http_request_example_main.c
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "sdkconfig.h"
#include "esp_console.h"
#include "esp_log.h"
#include "cmd_nvs.h" 
#include "nvs_flash.h"
#include "esp_timer.h"
#include "argtable3/argtable3.h"
#include "cmd_request.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "spb.local" // "example.com"
#define WEB_PORT "9000" // "80"
#define WEB_PATH "/image.jpeg" // "/"

#define REQUEST_URL_MAX_LEN      120
#define REQUEST_BUFFER_SIZE      (1024 * 40)

static const char *TAG = "cmd_request";

static struct {
    struct arg_str *url;
    struct arg_int *laps;
    struct arg_end *end;
} request_args;

static const char *REQUEST = "GET " WEB_PATH " HTTP/1.0\r\n"
    "Host: "WEB_SERVER":"WEB_PORT"\r\n"
    "User-Agent: esp-idf/1.0 esp32\r\n"
    "\r\n";

static void http_get_task(void *pvParameters)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s; // r;
    //char recv_buf[64];

    while(1) {
        uint64_t start = esp_timer_get_time();
        
        char *buffer = malloc(REQUEST_BUFFER_SIZE + 1);
        if (buffer == NULL) {
            ESP_LOGE(TAG, "Cannot malloc http receive buffer");
            vTaskDelete(NULL);
        }
        
        int err = getaddrinfo(WEB_SERVER, WEB_PORT, &hints, &res);

        if(err != 0 || res == NULL) {
            ESP_LOGE(TAG, "DNS lookup failed err=%d res=%p", err, res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        /* Code to print the resolved IP.

           Note: inet_ntoa is non-reentrant, look at ipaddr_ntoa_r for "real" code */
        addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        ESP_LOGI(TAG, "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));

        s = socket(res->ai_family, res->ai_socktype, 0);
        if(s < 0) {
            ESP_LOGE(TAG, "... Failed to allocate socket.");
            freeaddrinfo(res);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... allocated socket");

        if(connect(s, res->ai_addr, res->ai_addrlen) != 0) {
            ESP_LOGE(TAG, "... socket connect failed errno=%d", errno);
            close(s);
            freeaddrinfo(res);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(TAG, "... connected");
        freeaddrinfo(res);

        if (write(s, REQUEST, strlen(REQUEST)) < 0) {
            ESP_LOGE(TAG, "... socket send failed");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... socket send success");

        struct timeval receiving_timeout;
        receiving_timeout.tv_sec = 5;
        receiving_timeout.tv_usec = 0;
        if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &receiving_timeout,
                sizeof(receiving_timeout)) < 0) {
            ESP_LOGE(TAG, "... failed to set socket receiving timeout");
            close(s);
            vTaskDelay(4000 / portTICK_PERIOD_MS);
            continue;
        }
        ESP_LOGI(TAG, "... set socket receiving timeout success");

        bzero(buffer, REQUEST_BUFFER_SIZE);
        int read_len = 0;
        /* Read HTTP response */
        while (read_len < REQUEST_BUFFER_SIZE) {
            int data_read = read(s, buffer + read_len, REQUEST_BUFFER_SIZE - read_len);
            //ESP_LOGI(TAG, "data_read=%d", data_read);
            if (data_read <= 0) {
                break;
            }
            read_len += data_read;
        } // while

        ESP_LOGI(TAG, "... done reading from socket. Last read return=%d errno=%d.", read_len, errno);
        close(s);
        
        buffer[read_len] = '\0';
        //printf("--- buffer ---\n%s\n", buffer);
        
        
        /*
        for(int countdown = 10; countdown >= 0; countdown--) {
            ESP_LOGI(TAG, "%d... ", countdown);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
        */
        free(buffer);
        uint64_t end = esp_timer_get_time();
        int run_time_ms = (end - start) / 1000;
        //int bps = read_len * 1000 * 8 / run_time_ms;
        //ESP_LOGI(TAG, "status=%d bytes=%d run_time_ms=%d bps=%d", status_code, read_len, run_time_ms, bps);
        ESP_LOGI(TAG, "run_time_ms=%d", run_time_ms);
        
        break;
    }
    vTaskDelete(NULL);
}



static int _read(char *url, bool show)
{

    xTaskCreate(&http_get_task, "http_get_task", 4096, NULL, 5, NULL);
    
    return 0;
}
    

static int request_get(int argc, char **argv)
{    
    int nerrors = arg_parse(argc, argv, (void **) &request_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, request_args.end, argv[0]);
        return 1;
    }
    
    char url[REQUEST_URL_MAX_LEN] = "";
    strlcpy(url, request_args.url->sval[0],  sizeof(url));
    
    if (!strlen(url)) {
        if (get_str_from_nvs("url", url, sizeof(url)) != ESP_OK) {
            ESP_LOGE(TAG, "No such entry was found");
            return -1;
        }
    }
    
    int laps = 1;
    if (request_args.laps->count > 0) {
        laps = (uint32_t)(request_args.laps->ival[0]);
    }
    
    for (int i=0; i<laps; i++) {
        
        if (_read(url, false)) {
            break;
        }
    }
    
    return 0;
}

void register_request(void)
{
    request_args.url = arg_str0(NULL, NULL, "<url>", "URL base");
    request_args.laps = arg_int0("l", "laps", "<n>", "number of laps");
    request_args.end = arg_end(1);
    
    const esp_console_cmd_t request_cmd = {
        .command = "request",
        .help = "HTTP request via socket",
        .hint = NULL,
        .func = &request_get,
        .argtable = &request_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&request_cmd) );
}
