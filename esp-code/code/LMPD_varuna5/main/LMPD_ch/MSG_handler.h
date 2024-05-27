/**
 * @file MSG_handler.h - LIMPID project
 * @brief .c file to handle system operations
 * @details This driver provides functions to handle responses using bluetooth spp
 * @version 1.0
 * @date 2024-03-24
 * 
 * @author Francisco Duarte
 *         Leonardo Rosa
 *         
 **/
#ifndef MSG_HANDLER_H_
#define MSG_HANDLER_H_

#include <string.h>
#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_bt.h"
#include "esp_system.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "onewire_bus.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/semphr.h"


#include "../LMPD_adc/ADC_ads.h"
#include "../LMPD_sen/SEN_ds.h"
#include "../LMPD_sen/SEN_ph.h"
#include "../LMPD_sen/SEN_tds.h"
#include "../LMPD_sen/SEN_do.h"
#include "../LMPD_sen/SEN_turb.h"
#include "../LMPD_rom/ROM_msd.h"



#define GPIO_PIN_POWER  GPIO_NUM_5   // Breakout power GPIO
#define GPIO_PIN_STAT   GPIO_NUM_13  // Breakout power GPIO

#define ADC_CHANNEL ADC_CHANNEL_6 // GPIO 14 corresponds to ADC1 channel 6

#define POWER_MODE_ON 1             
#define POWER_MODE_OFF 0

#define MAX_LINE_LENGTH 500


static const char *FLUSH_TAG = "Flush_Task"; // Tag for logging
static const char *BAT_TAG = "CHARGE"; // Tag for logging


extern SemaphoreHandle_t actionT_semaphore;
extern SemaphoreHandle_t actionP_semaphore;
extern SemaphoreHandle_t actionA_semaphore;


/*Structure to store last sensor value*/
typedef struct {
    float Temperature;
    float PHydrogen;
    uint16_t TDSolids;
    float Doxygen;
    char* Turbidity;
    char waterType[10]; 
    char date[20];
} WaterParams;

/*Structure to store last timestamp in timestamp format*/
typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
} Date;

extern Date LastDate;
extern WaterParams LastParams;



/**
 * @name LMPD_SYSTEM_handleActionT
 * @brief Recieves Temperature value from the sensor, stores it in a structure and sents the value using spp
 * 
 * @param handle_ds Handler from ds18b20 operation library 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param);


/**
 * @name LMPD_SYSTEM_handleActionP
 * @brief Recieves PH value from the sensor, stores it in a structure and sents the value using spp
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param);
/**
 * @name LMPD_SYSTEM_handleActionS
 * @brief Recieves TDS value from the sensor, stores it in a structure and sents the value using spp
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param);
/**
 * @name LMPD_SYSTEM_handleActionD
 * @brief Recieves DO value from the sensor, stores it in a structure and sents the value using spp
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param);
/**
 * @name LMPD_SYSTEM_handleActionB
 * @brief Recieves Turbidity value from the sensor, stores it in a structure and sents the value using spp
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param);

/**
 * @name LMPD_SYSTEM_handleActionH
 * @brief Gets the water type from the structure which is updated by the WaterType task 
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionH(esp_spp_cb_param_t *param);

/**
 * @name LMPD_SYSTEM_handleActionF
 * @brief Flushes the stored information from the microSD via bluetooth
 * 
 * @param param Pointer to the event parameters structure
 * @return none
 */
void LMPD_SYSTEM_handleActionF(esp_spp_cb_param_t *param);

/**
 * @name LMPD_SYSTEM_Time
 * @brief Recieves the timestamp from the smarthphone via bluetooth
 * 
 * @param date Pointer to "%d-%d-%d %d:%d" type string
 * @return none
 */
void LMPD_SYSTEM_Time(char *date);


/**
 * @name LMPD_SYSTEM_save_parameters
 * @brief Recieves the timestamp from the smarthphone via bluetooth
 * 
 * @param mode_flag 1 -> smartphone is offline store in local storage
 *                  0 -> smartphone is online
 * @return none
 */
void LMPD_SYSTEM_save_parameters(bool mode_flag);


/**
 * @name LMPD_SYSTEM_PM
 * @brief Handles the power to breakout sensor boards
 * 
 * @param mode_flag 1 -> boards are activated
 *                  0 -> boards are deactivated
 * @return none
 */
void LMPD_SYSTEM_PM(bool power_mode);


void LMPD_BATTERY_status(esp_spp_cb_param_t *param);


#endif /* MSG_HANDLER_H_ */