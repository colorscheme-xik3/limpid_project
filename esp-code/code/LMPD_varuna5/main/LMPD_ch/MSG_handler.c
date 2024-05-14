/**
 * @file MSG_handler.c - LIMPID project
 * @brief .c file to handle system operations
 * @details This driver provides functions to handle responses using bluetooth spp
 * @version 1.0
 * @date 2024-03-24
 * 
 * @author Francisco Duarte
 *         Leonardo Rosa
 *         
 **/

#include "MSG_handler.h"
#include "../LMPD_sen/SEN_ds.h"
#include "../LMPD_sen/SEN_tds.h"

WaterParams LastParams;
Date LastDate;
static esp_adc_cal_characteristics_t adc2_chars;



void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param)
{
    LMPD_SYSTEM_PM(POWER_MODE_ON);
    char sppVoltageT[4] = "";
    LastParams.Temperature = ds18b20_readTemperature(handle_ds);
    ESP_LOGI(TAG_ADS, "T Value: %f", LastParams.Temperature);

    sprintf(sppVoltageT, "%.1fT", LastParams.Temperature);
    esp_spp_write(param->write.handle, strlen(sppVoltageT), (uint8_t *)sppVoltageT);
}

void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param)
{
    char sppVoltageA0[10] = "";

    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB);
    float raw_A0 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB));
    float mvolt_A0 = ((raw_A0* 4.096) / (32767))*1000;
    LastParams.PHydrogen = mvolt_A0;

    sprintf(sppVoltageA0, "%.1fP", LastParams.PHydrogen);
    esp_spp_write(param->write.handle, strlen(sppVoltageA0), (uint8_t*)sppVoltageA0);
}


void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param)
{
    char sppVoltageA1[10] = "";

    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB);
    float raw_A1 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
    float mvolt_A1 = ((raw_A1* 4.096) / (32767));

    LastParams.TDSolids = (uint16_t)sen0244_processing(mvolt_A1, 34.0);
    sprintf(sppVoltageA1, "%dS",  LastParams.TDSolids);

    ESP_LOGI(TAG_ADS, "TDS Value: %f | %d", mvolt_A1, LastParams.TDSolids);

    esp_spp_write(param->write.handle, strlen(sppVoltageA1), (uint8_t*)sppVoltageA1);
}

void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param)
{
    char sppVoltageA2[10] = "4.5O";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB);
    float raw_A2 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB));
    float mvolt_A2 = ((raw_A2* 4.096) / (32767))*1000;
    LastParams.Doxygen = mvolt_A2;
    ESP_LOGI(TAG_ADS, "DO Value: %.1f", LastParams.Doxygen);

    esp_spp_write(param->write.handle, strlen(sppVoltageA2), (uint8_t*)sppVoltageA2);

}


void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param)
{
    char sppVoltageA3[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB);
    float raw_A3 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB));
    float mvolt_A3 = ((raw_A3* 4.096) / (32767));
    LastParams.Turbidity = sen0189_processing(mvolt_A3);
    sprintf(sppVoltageA3, "%sB", LastParams.Turbidity);
    ESP_LOGI(TAG_ADS, "Turbidity-  %f raw - %f V - %s NTU", raw_A3,  mvolt_A3, LastParams.Turbidity);

    esp_spp_write(param->write.handle, strlen(sppVoltageA3), (uint8_t*)sppVoltageA3);
}


void LMPD_SYSTEM_handleActionH(esp_spp_cb_param_t *param) {
    char water_type[10] = ""; 

    sprintf(water_type, "%sH", LastParams.waterType);
    esp_spp_write(param->write.handle, strlen(water_type), (uint8_t*)water_type);
    
}


void LMPD_SYSTEM_Time(char *date)
{      
    char join_date[20]; 
    
    sscanf(date, "%d-%d-%d %d:%d", &LastDate.year, &LastDate.month, &LastDate.day, &LastDate.hour, &LastDate.min);
    
    /*Last response sent -> Power off breakout boards*/
    LMPD_SYSTEM_PM(POWER_MODE_OFF);
}

void LMPD_SYSTEM_save_parameters(bool mode_flag)
{
    char join_date[20]; // Sufficient size for "yyyy-MM-dd HH:mm\0"

    const char *t_parameter = "Temperature";
    const char *p_parameter = "Potential Hydrogen";
    const char *s_parameter = "Total Dissolved Solids";
    const char *o_parameter = "Dissolved Oxygen";
    const char *b_parameter = "Turbidity";
    const char *d_parameter = "Timestamp";
    const char *w_parameter = "Water Type";

    snprintf(join_date, sizeof(join_date), "%d-%02d-%02d %02d:%02d",
             LastDate.year, LastDate.month, LastDate.day, LastDate.hour, LastDate.min);

    if(mode_flag)
    {
        printf("Guardei");
        LMPD_device_writing(MOUNT_POINT"/param.csv", t_parameter, LastParams.Temperature);
        LMPD_device_writing(MOUNT_POINT"/param.csv", p_parameter, LastParams.PHydrogen);
        LMPD_device_writing(MOUNT_POINT"/param.csv", s_parameter, LastParams.TDSolids);
        LMPD_device_writing(MOUNT_POINT"/param.csv", o_parameter, LastParams.Doxygen);
        LMPD_device_writing_string(MOUNT_POINT"/param.csv", b_parameter, LastParams.Turbidity);
        LMPD_device_writing_string(MOUNT_POINT"/param.csv", w_parameter, LastParams.waterType);
        LMPD_device_writing_string(MOUNT_POINT"/param.csv", d_parameter, join_date);
        LMPD_device_writing_space(MOUNT_POINT"/param.csv");

    }

}



void LMPD_SYSTEM_handleActionF(esp_spp_cb_param_t *param)
{
    static int current_block_index = 0;  // Static variable to track current block index
    const char *filename = MOUNT_POINT"/param.csv";
    char block_buffer[MAX_LINE_LENGTH];
    esp_err_t ret;

    char sppStopFlush[10];  // Adjust buffer size as needed
    float test = 1.5;

    printf("Stoped to flush\n");

    // Read the next block of data from the CSV file
    ret = LMPD_device_read_block(filename, block_buffer, sizeof(block_buffer));
    if (ret != ESP_OK) {
        sprintf(sppStopFlush, "%.1fZ", test);
        esp_spp_write(param->write.handle, strlen(sppStopFlush), (uint8_t*)sppStopFlush);
        ESP_LOGE(FLUSH_TAG, "No more blocks to read", current_block_index);
        return;
    }

    // Process the read block
    printf("Block %d:\n%s\n", current_block_index + 1, block_buffer);
    esp_spp_write(param->write.handle, strlen(block_buffer), (uint8_t*)block_buffer);

    // Increment the current block index for the next function call
    current_block_index++;
}



void LMPD_SYSTEM_PM(bool power_mode)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;      // Disable interrupt
    io_conf.mode = GPIO_MODE_OUTPUT;            // Set as output mode
    io_conf.pin_bit_mask = (1ULL << GPIO_PIN_POWER); // Set the GPIO pin bit mask
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;   // Disable pull-up resistor
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;// Disable pull-down resistor

    gpio_config(&io_conf); // Configure GPIO pin settings

    if (power_mode) {
        // Activate (set high) the GPIO pin
        gpio_set_level(GPIO_PIN_POWER, 1);
        printf("GPIO pin %d activated\n", GPIO_PIN_POWER);
    } else {
        // Deactivate (set low) the GPIO pin
        gpio_set_level(GPIO_PIN_POWER, 0);
        printf("GPIO pin %d deactivated\n", GPIO_PIN_POWER);
    }
}

void LMPD_BATTERY_status(esp_spp_cb_param_t *param)
{
    // Configure ADC to read from GPIO 14 (ADC1 channel 6)
    char status_charge[10] = ""; 

    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_6, ADC_ATTEN_DB_11));

    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc2_chars);

    int raw_value;
        
    esp_err_t err = adc2_get_raw(ADC2_CHANNEL_6, ADC_WIDTH_BIT_12, &raw_value);
    if (err != ESP_OK) {
        ESP_LOGE(BAT_TAG, "ADC2 read error: %d", err);
        return; // Skip current iteration and try again
    }

    uint16_t voltage_mv = esp_adc_cal_raw_to_voltage(raw_value, &adc2_chars);

    sprintf(status_charge, "%luC", voltage_mv);
    esp_spp_write(param->write.handle, strlen(status_charge), (uint8_t*)status_charge);

    // Print the voltage using ESP_LOGI
    ESP_LOGI(BAT_TAG, "Input Pin Voltage: %s - %lu mV", status_charge, voltage_mv);
}
