#include "MSG_handler.h"


/*
void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param)
{
    char sppVoltageA0[10] = "pa";
    float voltage_A0 = readTemperature(handle_ds);
    sprintf(sppVoltageA0, "%.2fT", voltage_A0);
    printf("%.2fT",sppVoltageA0);
    esp_spp_write(param->write.handle, strlen(sppVoltageA0), (uint8_t *)sppVoltageA0);

    //xSemaphoreGive(actionT_semaphore); // Give the semaphore to indicate completion

}*/


void LMPD_SYSTEM_handleActionT(esp_spp_cb_param_t *param)
{
    char sppVoltageA0[10] = "pa";
    esp_spp_write(param->write.handle, strlen(sppVoltageA0), (uint8_t *)sppVoltageA0);

}

/*
void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param)
{
    char sppVoltageA1[10] = "";

    float voltage_A1 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
    float voltage_adc = voltage_A1 * 1000;  
   // LMPD_PH_selector(PH_SENSOR);
    
    ESP_LOGI(TAG_ADS, "PH Value: %.2f", voltage_A1);

    //ESP_LOGI(TAG_ADS, "PH Value: %.2f", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_A1));

    //sprintf(sppVoltageA1, "%.2fP", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_adc));
    esp_spp_write(param->write.handle, strlen(sppVoltageA1), (uint8_t*)sppVoltageA1);

    xSemaphoreGive(actionP_semaphore); // Give the semaphore to indicate completion

}





void LMPD_SYSTEM_handleActionD(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param)
{
    char sppVoltageA3[20] = "";

    float raw_A3 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
    float mvolt_A3 = ((raw_A3* 4.096) / (32767 * 1))*1000;
    ESP_LOGI(TAG_ADS, "DO raw value: %.2f", raw_A3);
    ESP_LOGI(TAG_ADS, "DO mv value: %.2f", mvolt_A3);

    float result_A3 = readDO(mvolt_A3, readTemperature(handle_ds));

    sprintf(sppVoltageA3, "%.2f", result_A3);

    //sprintf(sppVoltageA1, "%.2fP", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_adc));
    esp_spp_write(param->write.handle, strlen(sppVoltageA3), (uint8_t*)sppVoltageA3);

    //xSemaphoreGive(actionP_semaphore); // Give the semaphore to indicate completion

}*/

        