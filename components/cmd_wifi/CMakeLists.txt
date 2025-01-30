idf_component_register(SRCS "cmd_wifi.c" "cmd_ping.c" "cmd_rest.c" "cmd_client.c" "cmd_request.c"
                    INCLUDE_DIRS .
                    REQUIRES console esp_wifi cmd_nvs http_rest_client led esp_timer)
