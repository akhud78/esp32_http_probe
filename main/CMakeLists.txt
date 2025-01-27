idf_component_register(SRC_DIRS "."
                    REQUIRES driver nvs_flash console cmd_system cmd_wifi cmd_nvs http_rest_client led
                    INCLUDE_DIRS ".")
