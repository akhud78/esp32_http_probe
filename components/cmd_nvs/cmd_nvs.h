/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Register NVS functions
void register_nvs(void);

void initialize_nvs(void);
esp_err_t get_str_from_nvs(const char* key, char *dest, size_t len);

#ifdef __cplusplus
}
#endif
