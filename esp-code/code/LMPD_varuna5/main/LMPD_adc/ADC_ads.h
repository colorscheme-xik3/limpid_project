/**
 * @file ads1115_driver.h - LIMPID project
 * @brief Header file for ADS1115 ADC driver
 * @details This driver provides functions to initialize and configure the ADS1115 ADC.
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Francisco Duarte
 *         Leonardo Rosa
 *         
 */
#ifndef ADC_ads_H_
#define ADC_ads_H_

#include <stdio.h>
#include "driver/i2c.h"
#include "esp_err.h"

static const char *TAG_ADS = "ADC_ADS";

#define I2C_MASTER_SCL_IO           22                         /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           21                          /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define ADS1115_SENSOR_ADDR                 0x48        /*!< Slave address of the MPU9250 sensor */
#define ADS1115_CONFIG_REG                  0x01
#define ADS1115_CONV_REG                    0X00
#define ADS1115_CONFIG_MSB_A0               0xC2
#define ADS1115_CONFIG_MSB_A1               0xD2
#define ADS1115_CONFIG_MSB_A2               0xE2
#define ADS1115_CONFIG_MSB_A3               0xF2
#define ADS1115_CONFIG_LSB                  0x83


/*Driver functions initiation*/

/**
 * @brief Initialize the I2C bus for ADS1115 ADC.
 *
 * This function initializes the I2C bus for communication with the ADS1115 ADC.
 *
 * @return
 *         - ESP_OK : I2C initialization success.
 *         - ESP_FAIL : I2C initialization failed.
 */
uint16_t LMPD_I2C_configureADS(uint8_t address, uint8_t reg1, uint8_t reg2);

/**
 * @brief Configure the ADS1115 ADC.
 *
 * This function configures the ADS1115 ADC with the provided configuration registers.
 *
 * @param[in] address : I2C address of the ADS1115 ADC.
 * @param[in] reg1 : Configuration register 1.
 * @param[in] reg2 : Configuration register 2.
 *
 * @return
 *         - ADC value : The ADC conversion result.
 *         - 0 : Configuration of ADS1115 ADC failed.
 */
extern esp_err_t LMPD_I2C_init(void);

#endif /* ADS1115_DRIVER_H_ */