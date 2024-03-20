/**
 * @file ads1115_driver.c - LIMPID project
 * @brief .c file for ADS1115 ADC driver
 * @details This driver provides functions to initialize and configure the ADS1115 ADC.
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Francisco Duarte
 *         Leonardo Rosa
 *         
 **/

// Include necessary header files
#include "ADC_ads.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"

// Initialize the I2C bus
extern esp_err_t LMPD_I2C_init(void) {
    // Define the I2C master port to use
    int i2c_master_port = I2C_MASTER_NUM;

    // Configure the I2C bus settings
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .clk_flags = 0,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    // Set the I2C configuration
    i2c_param_config(i2c_master_port, &conf);

    // Install the I2C driver
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

// Configure the ADS1115 ADC with the given settings
uint16_t LMPD_I2C_configureADS(uint8_t address, uint8_t reg1, uint8_t reg2) {
    uint16_t adc_value = 0;

    ESP_LOGI(TAG_ADS, "*re ADC value: %d", adc_value);

    // Set up the configuration bytes for the ADC
    uint8_t config[3] = {0};
    config[0] = ADS1115_CONFIG_REG;
    config[1] = reg1;
    config[2] = reg2;

    // Create an I2C command handle
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // Send the I2C start bit
    i2c_master_start(cmd);

    // Send the I2C address of the ADC and the write bit
    i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, true);

    // Send the ADC configuration bytes
    i2c_master_write(cmd, config, sizeof(config), true);

    // Send the I2C stop bit
    i2c_master_stop(cmd);

    // Execute the I2C command
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    // Delete the I2C command handle
    i2c_cmd_link_delete(cmd);

    // Check if the I2C command was successful
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_ADS, "Could not configure ADS1115 ADC");
        return 0;
    }

    // Delay for a settling time (adjust this delay as needed based on the device requirements)
    //vTaskDelay(pdMS_TO_TICKS(2000));
    
    // Create a new I2C command handle
    cmd = i2c_cmd_link_create();

    // Send the I2C start bit
    i2c_master_start(cmd);

    // Send the I2C address of the ADC and the write bit
    i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, true);

    // Send the address pointer byte to select the conversion register
    i2c_master_write_byte(cmd, ADS1115_CONV_REG, true);

    // Send the I2C stop bit
    i2c_master_stop(cmd);


    // Execute the I2C command
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    // Delete the I2C command handle
    i2c_cmd_link_delete(cmd);

    // Check if the I2C command was successful
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_ADS, "Could not select conversion register for ADS1115 ADC");
        return 0;
    }

    vTaskDelay(pdMS_TO_TICKS(200)); // Adjust the delay time as needed

    // Create a new I2C command handle
    cmd = i2c_cmd_link_create();

    // Send the I2C start bit
    i2c_master_start(cmd);

    // Send the I2C address of the ADC and the read bit
    i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_READ, true);

    // Read the conversion register bytes from the ADC
    uint8_t data[2];
    i2c_master_read(cmd, data, sizeof(data), I2C_MASTER_LAST_NACK);

    // Send the I2C stop bit
    i2c_master_stop(cmd);

    // Execute the I2C command
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    // Delete the I2C command handle
    i2c_cmd_link_delete(cmd);

    // Check if the I2C command was successful
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_ADS, "Could not read ADS1115 ADC");
        return 0;
    }

    adc_value = (data[0] << 8) | data[1];

    ESP_LOGI(TAG_ADS, "ADC value: %d", adc_value);
    // Combine the read values into the ADC result
    return adc_value;
}
