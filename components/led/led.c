#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "led.h"

// On board LED
#define BLINK_GPIO          (CONFIG_BLINK_GPIO)

#ifdef CONFIG_BLINK_LED_RMT
#include "led_strip.h"
// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ    (10 * 1000 * 1000)

static led_strip_handle_t _configure_led(void)
{
    // LED strip general initialization, according to your led board design
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,   // The GPIO that connected to the LED strip's data line
        .max_leds = 1,       
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // The color component format is G-R-B
        .flags.invert_out = false,                // whether to invert the output signal
    };

    // LED strip backend configuration: RMT
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
    };

    // LED Strip object handle
    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    return led_strip;
}

static led_strip_handle_t s_led_strip = NULL;

void led_configure(void)
{
    // LED strip initialization
    if (s_led_strip == NULL) {
        s_led_strip = _configure_led();
    }
    // Set all LED off to clear all pixels
    ESP_ERROR_CHECK(led_strip_clear(s_led_strip));
}

void led_blink_on(void)
{
    uint8_t value = 16;
    uint32_t led_index = 0;
    /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
    ESP_ERROR_CHECK(led_strip_set_pixel(s_led_strip, led_index, value, value, value));
    /* Refresh the strip to send data */
    ESP_ERROR_CHECK(led_strip_refresh(s_led_strip));
}

void led_blink_off(void)
{
    uint8_t value = 0;
    uint32_t led_index = 0;

    ESP_ERROR_CHECK(led_strip_set_pixel(s_led_strip, led_index, value, value, value));
    ESP_ERROR_CHECK(led_strip_refresh(s_led_strip));
}

#elif CONFIG_BLINK_LED_GPIO

void led_configure(void)
{
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}


void led_blink_on(void)
{
#ifdef CONFIG_BLINK_INVERTED
    gpio_set_level(BLINK_GPIO, 0);
#else
    gpio_set_level(BLINK_GPIO, 1);
#endif
}
void led_blink_off(void)
{
#ifdef CONFIG_BLINK_INVERTED
    gpio_set_level(BLINK_GPIO, 1);
#else
    gpio_set_level(BLINK_GPIO, 0);
#endif
}

#endif





