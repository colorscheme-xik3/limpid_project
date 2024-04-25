#include "MSG_handler.h"
#include "../LMPD_sen/SEN_ds.h"
#include "../LMPD_sen/SEN_tds.h"



WaterParams LastParams;
Date LastDate;

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
    LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.Temperature);
}


void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param, bool mode_flag)
{
    LMPD_SYSTEM_PM(POWER_MODE_ON);
    char sppVoltageT[10] = "";
    LastParams.Temperature = ds18b20_readTemperature(handle_ds);
    ESP_LOGI(TAG_ADS, "T Value: %f", LastParams.Temperature);

    sprintf(sppVoltageT, "%.1fT", LastParams.Temperature);
    esp_spp_write(param->write.handle, strlen(sppVoltageT), (uint8_t *)sppVoltageT);

    if(mode_flag)
    {
        const char *parameter = "Temperature";
        //LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.Temperature);
    }
}

void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppVoltageA0[10] = "";

    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB);
    float raw_A0 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A0, ADS1115_CONFIG_LSB));
    float mvolt_A0 = ((raw_A0* 4.096) / (32767))*1000;
    LastParams.PHydrogen = mvolt_A0;

    sprintf(sppVoltageA0, "%.1fP", LastParams.PHydrogen);


    //float voltage_adc = voltage_A0 * 1000;  
   // LMPD_PH_selector(PH_SENSOR);
    //ESP_LOGI(TAG_ADS, "PH Value: %f", mvolt_A0);

    //ESP_LOGI(TAG_ADS, "PH Value: %.2f", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_A1));

    //sprintf(sppVoltageA1, "%.2fP", LMPD_PH_calibrator(CALIBRATION_POINTS, voltage_adc));

    if(mode_flag)
    {
        const char *parameter = "Potential Hydrogen";
       // LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.PHydrogen);
    }

    esp_spp_write(param->write.handle, strlen(sppVoltageA0), (uint8_t*)sppVoltageA0);

    
}


void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppVoltageA1[10] = "";

    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB);
    float raw_A1 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
    float mvolt_A1 = ((raw_A1* 4.096) / (32767));

    LastParams.TDSolids = (uint16_t)sen0244_processing(mvolt_A1, LastParams.Temperature);
    sprintf(sppVoltageA1, "%dS",  LastParams.TDSolids);

    ESP_LOGI(TAG_ADS, "TDS Value: %f | %d", mvolt_A1, LastParams.TDSolids);

    if(mode_flag)
    {
        const char *parameter = "TDS";
       // LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.TDSolids);
    }

    esp_spp_write(param->write.handle, strlen(sppVoltageA1), (uint8_t*)sppVoltageA1);
}

void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppVoltageA2[10] = "4.5D";

    
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB);
    float raw_A2 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A2, ADS1115_CONFIG_LSB));
    float mvolt_A2 = ((raw_A2* 4.096) / (32767))*1000;
    LastParams.Doxygen = mvolt_A2;
    ESP_LOGI(TAG_ADS, "DO Value: %.1f", LastParams.Doxygen);
    

    if(mode_flag)
    {
        const char *parameter = "Dissolved Oxygen";
        //LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.Doxygen);
    }

    esp_spp_write(param->write.handle, strlen(sppVoltageA2), (uint8_t*)sppVoltageA2);

}


void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppVoltageA3[10] = "";
    LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB);
    float raw_A3 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A3, ADS1115_CONFIG_LSB));
    float mvolt_A3 = ((raw_A3* 4.096) / (32767));

    double ntu = - (2572.2 * mvolt_A3 * mvolt_A3) + (8700.5 * mvolt_A3) - 4352.9;
    //double ntu = - (1300 * mvolt_A3 * mvolt_A3) + (6500 * mvolt_A3) - 4353;

    //uint32_t ntu = (-1659.9 * mvolt_A3) + 7198.4;
    LastParams.Turbidity = ntu;
    sprintf(sppVoltageA3, "%fB", LastParams.Turbidity);
    ESP_LOGI(TAG_ADS, "Turbidity %f V - %f NTU", mvolt_A3, LastParams.Turbidity);

    if(mode_flag)
    {
        const char *parameter = "Turbidity";
        //LMPD_device_writing(MOUNT_POINT"/param.csv", parameter, LastParams.Turbidity);
    }

    esp_spp_write(param->write.handle, strlen(sppVoltageA3), (uint8_t*)sppVoltageA3);
}

void LMPD_SYSTEM_Time(char *date, bool mode_flag)
{      
    char join_date[20]; // Sufficient size for "yyyy-MM-dd HH:mm\0"
    // Extract year, month, day, hour, and minute from the date string
    sscanf(date, "%d-%d-%d %d:%d", &LastDate.year, &LastDate.month, &LastDate.day, &LastDate.hour, &LastDate.min);
    //snprintf(join_date, sizeof(join_date), "%d-%02d-%02d %02d:%02d",
             //LastDate.year, LastDate.month, LastDate.day, LastDate.hour, LastDate.min);

    ESP_LOGI(TAG_ADS, "date %s", join_date);


    if(mode_flag)
    {
        const char *parameter = "Date";
        //LMPD_device_writing_time(MOUNT_POINT"/param.csv", parameter, join_date);
        //LMPD_device_writing_space(MOUNT_POINT"/param.csv");
    }

    LMPD_SYSTEM_PM(POWER_MODE_OFF);
}

void LMPD_SYSTEM_save_parameters(bool mode_flag)
{
    char join_date[20]; // Sufficient size for "yyyy-MM-dd HH:mm\0"

    const char *t_parameter = "Temperature";
    const char *p_parameter = "PH";
    const char *s_parameter = "TDS";
    const char *o_parameter = "DO";
    const char *b_parameter = "Turbidity";
    const char *d_parameter = "Date";

    snprintf(join_date, sizeof(join_date), "%d-%02d-%02d %02d:%02d",
             LastDate.year, LastDate.month, LastDate.day, LastDate.hour, LastDate.min);

    if(mode_flag)
    {
        LMPD_device_writing(MOUNT_POINT"/param.csv", t_parameter, LastParams.Temperature);
        LMPD_device_writing(MOUNT_POINT"/param.csv", p_parameter, LastParams.PHydrogen);
        LMPD_device_writing(MOUNT_POINT"/param.csv", s_parameter, LastParams.TDSolids);
        LMPD_device_writing(MOUNT_POINT"/param.csv", o_parameter, LastParams.Doxygen);
        LMPD_device_writing(MOUNT_POINT"/param.csv", b_parameter, LastParams.Turbidity);
        LMPD_device_writing_time(MOUNT_POINT"/param.csv", d_parameter, join_date);
        LMPD_device_writing_space(MOUNT_POINT"/param.csv");

    }

}



void flushTask(void *pvParameters) {
    char sppStopFlush[10] = "";
    const char *filename = MOUNT_POINT"/param.csv";
    char block_buffer[MAX_LINE_LENGTH];
    int block_count = 0;
    esp_err_t ret;
    esp_spp_cb_param_t *param = (esp_spp_cb_param_t*)pvParameters;

    printf("Stoped to flush\n");


    while (1) {
        // Read a block from the CSV file
        ret = LMPD_device_read_block(filename, block_buffer, sizeof(block_buffer));
        if (ret != ESP_OK) {
            ESP_LOGE(FLUSH_TAG, "Error reading block from file");
            break; // Exit the loop if there's an error
        }

        // Process the read block (print in this example)
        printf("Block %d:\n%s\n", block_count + 1, block_buffer);

        // Simulate delay before reading the next block
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 2 seconds
        block_count++;
    }


    if(esp_spp_write(param->write.handle, strlen(sppStopFlush), (uint8_t*)sppStopFlush))
        printf("Restarted command sending\n");



    // Task completed, delete the task
    vTaskDelete(NULL);
}

void LMPD_SYSTEM_handleActionF(esp_spp_cb_param_t *param, bool mode_flag)
{
    char sppStopFlush[10] = "";
    float test = 1;

    const char *filename = MOUNT_POINT"/param.csv";
    char block_buffer[MAX_LINE_LENGTH];
    int block_count = 0;
    esp_err_t ret;

    printf("Stoped to flush\n");


    while (1) {
        // Read a block from the CSV file
        ret = LMPD_device_read_block(filename, block_buffer, sizeof(block_buffer));
        if (ret != ESP_OK) {
            ESP_LOGE(FLUSH_TAG, "Error reading block from file");
            break; // Exit the loop if there's an error
        }

        // Process the read block (print in this example)
        printf("Block %d:\n%s\n", block_count + 1, block_buffer);

        // Simulate delay before reading the next block
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 2 seconds
        block_count++;
    }

    sprintf(sppStopFlush, "%.1fF", test);


    printf("%s",sppStopFlush);

    esp_spp_write(param->write.handle, strlen(sppStopFlush), (uint8_t*)sppStopFlush);
    
    printf("Restarted command sending\n");

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

        