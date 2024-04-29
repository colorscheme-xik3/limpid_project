/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_system.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "onewire_bus.h"
#include "driver/uart.h"
#include <math.h>
#include "driver/gpio.h"
#include "esp_sleep.h"

#include "LMPD_blt/BLT_spp.h"
#include "LMPD_adc/ADC_ads.h"
#include "LMPD_sen/SEN_ds.h"
#include "LMPD_rom/ROM_msd.h"
#include "LMPD_wpp/WPP_handler.h"


#define GPIO_PIN_NUMBER  GPIO_NUM_4  // Replace XX with the GPIO number you want to configure


const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
const bool esp_spp_enable_l2cap_ertm = true;
struct timeval time_new, time_old;
long data_num = 0;
const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;
bool bluetooth_connected = false;

uint8_t device_num = 0;
uint8_t device_rom_id[EXAMPLE_ONEWIRE_MAX_DEVICES][8];
onewire_bus_handle_t handle_ds;


TaskHandle_t task_A0_handle = NULL;
TaskHandle_t task_A1_handle = NULL;
TaskHandle_t task_temperature_handle = NULL;

SemaphoreHandle_t actionT_semaphore;
SemaphoreHandle_t actionP_semaphore;
SemaphoreHandle_t actionA_semaphore;

SemaphoreHandle_t mutex_A0;
SemaphoreHandle_t mutex_A1;
SemaphoreHandle_t mutex_temperature;

SemaphoreHandle_t configureSemaphore;
SemaphoreHandle_t writeSemaphore;

SemaphoreHandle_t bluetooth_semaphore; // Semaphore to notify offline_task about Bluetooth connection status



void online_task(void *pvParameters)
{
    // Bluetooth initialization and configuration code

   

    char bda_str[18] = {0};
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    if ((ret = esp_bluedroid_init()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    esp_spp_cfg_t bt_spp_cfg = {
        .mode = esp_spp_mode,
        .enable_l2cap_ertm = esp_spp_enable_l2cap_ertm,
        .tx_buffer_size = 0,
    };
    if ((ret = esp_spp_enhanced_init(&bt_spp_cfg)) != ESP_OK) {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        vTaskDelete(NULL);
    }

    
#if (CONFIG_BT_SSP_ENABLED == true)
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));
#endif

    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    esp_bt_pin_code_t pin_code;
    esp_bt_gap_set_pin(pin_type, 0, pin_code);

    ESP_LOGI(SPP_TAG, "Own address:[%s]", bda2str((uint8_t *)esp_bt_dev_get_address(), bda_str, sizeof(bda_str)));

    // Bluetooth loop
    while (1) {
        // Your Bluetooth-related tasks or event loop code goes here

        if (!bluetooth_connected) {
            xSemaphoreGive(bluetooth_semaphore);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // Example: delay for 1 second
    }
}


void offline_task(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(bluetooth_semaphore, portMAX_DELAY) == pdTRUE) { // Wait indefinitely for the semaphore
        // Do offline tasks here
            //LMPD_SYSTEM_handleActionT_sd(handle_ds, bluetooth_connected);
            ESP_LOGI("ADC_ADS", "Performed action, wrote on SD");
            vTaskDelay(pdMS_TO_TICKS(1000));  // Example: delay for 1 second
        }
   
    }
}


    
void app_main(void)
{
    
    bluetooth_semaphore = xSemaphoreCreateBinary();
    if (bluetooth_semaphore == NULL) {
        // Semaphore creation failed
        // Handle error
    }


       gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;        // Disable interrupt
    io_conf.mode = GPIO_MODE_INPUT;                // Set as input mode
    io_conf.pin_bit_mask = (1ULL << GPIO_PIN_NUMBER);  // Bit mask of the pins that you want to set
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;       // Enable pull-up resistor
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;  // Disable pull-down resistor
    gpio_config(&io_conf);
    // ------------------------------------------ DS18B20 INIT SECTION ----------------------------------------//
    
    onewire_rmt_config_t config = {
        .gpio_pin = EXAMPLE_ONEWIRE_GPIO_PIN,
        .max_rx_bytes = 10
    };

    ESP_ERROR_CHECK(onewire_new_bus_rmt(&config, &handle_ds));
    ESP_LOGI(TAG_DS, "1-wire bus installed");

    onewire_rom_search_context_handler_t context_handler;
    ESP_ERROR_CHECK(onewire_rom_search_context_create(handle_ds, &context_handler));

    do {
        esp_err_t search_result = onewire_rom_search(context_handler);

        if (search_result == ESP_ERR_INVALID_CRC) {
            continue;
        } else if (search_result == ESP_FAIL || search_result == ESP_ERR_NOT_FOUND) {
            break;
        }

        ESP_ERROR_CHECK(onewire_rom_get_number(context_handler, device_rom_id[device_num]));
        ESP_LOGI(TAG_DS, "found device with rom id " ONEWIRE_ROM_ID_STR, ONEWIRE_ROM_ID(device_rom_id[device_num]));
        device_num++;
    } while (device_num < EXAMPLE_ONEWIRE_MAX_DEVICES);

    ESP_ERROR_CHECK(onewire_rom_search_context_delete(context_handler));
    ESP_LOGI(TAG_DS, "%d device%s found on 1-wire bus", device_num, device_num > 1 ? "s" : "");

    // ------------------------------------------ I2C INIT SECTION ----------------------------------------//

    ESP_ERROR_CHECK(LMPD_I2C_init());
    ESP_LOGI(TAG_ADS, "I2C initialized successfully");


    // ------------------------------------------ SPI INIT SECTION ----------------------------------------//

    ESP_ERROR_CHECK(sd_card_init());

   
    // ------------------------------------------ BLT INIT SECTION ----------------------------------------//
    xTaskCreate(&online_task, "bluetooth_task", 8192, NULL, 5, NULL);
    xTaskCreate(&offline_task, "offline_task", 4096, NULL, 5, NULL);
    xTaskCreate(&LMPD_SYSTEM_waterType, "water_task", 2048, NULL, 2, NULL);


}



