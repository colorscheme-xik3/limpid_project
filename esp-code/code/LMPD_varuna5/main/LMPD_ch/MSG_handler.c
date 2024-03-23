#include "MSG_handler.h"
#include "../LMPD_sen/SEN_ds.h"


WaterParams LastParams;

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

void LMPD_SYSTEM_handleActionT_sd(onewire_bus_handle_t handle_ds, bool mode_flag)
{
    const char *parameter = "Temperature";
    float test = 10.54;
    //LastParams.Temperature = ds18b20_readTemperature(handle_ds);
    LastParams.Temperature = test;
    ESP_LOGI(TAG_ADS, "PH Value: %f", LastParams.Temperature); 
    LMPD_device_writing(MOUNT_POINT"/param.txt", parameter, LastParams.Temperature);
}


void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppVoltageT[10] = "";
    //LastParams.Temperature = ds18b20_readTemperature(handle_ds);
    LastParams.Temperature = ds18b20_readTemperature(handle_ds);
    ESP_LOGI(TAG_ADS, "PH Value: %f", LastParams.Temperature);

    sprintf(sppVoltageT, "%.2fT", LastParams.Temperature);
    printf("%.2fT",sppVoltageT);
    esp_spp_write(param->write.handle, strlen(sppVoltageT), (uint8_t *)sppVoltageT);
}

void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param)
{
    char sppVoltageA0[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB);
    float raw_A0 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB));
    float mvolt_A0 = ((raw_A0* 4.096) / (32767))*1000;

    //float voltage_adc = voltage_A0 * 1000;  
   // LMPD_PH_selector(PH_SENSOR);
    ESP_LOGI(TAG_ADS, "PH Value: %f", mvolt_A0);

    //ESP_LOGI(TAG_ADS, "PH Value: %.2f", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_A1));

    //sprintf(sppVoltageA1, "%.2fP", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_adc));
    //esp_spp_write(param->write.handle, strlen(sppVoltageA1), (uint8_t*)sppVoltageA1);
}


void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param)
{
    char sppVoltageA1[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB);
    float raw_A1 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
    float mvolt_A1 = ((raw_A1* 4.096) / (32767))*1000;

    ESP_LOGI(TAG_ADS, "TDS Value: %f", mvolt_A1);
}

void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param)
{
    char sppVoltageA2[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB);
    float raw_A2 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB));
    float mvolt_A2 = ((raw_A2* 4.096) / (32767))*1000;

    ESP_LOGI(TAG_ADS, "DO Value: %f", mvolt_A2);
}


void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param)
{
    char sppVoltageA3[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB);
    float raw_A3 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB));
    float mvolt_A3 = ((raw_A3* 4.096) / (32767))*1000;

    ESP_LOGI(TAG_ADS, "B Value: %f", mvolt_A3);
}
/*



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

        