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


#include "../LMPD_adc/ADC_ads.h"
#include "../LMPD_sen/SEN_ds.h"
#include "../LMPD_sen/SEN_tds.h"
#include "../LMPD_rom/ROM_msd.h"

//#include "ds18b20.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define GPIO_PIN_POWER  GPIO_NUM_5  // Replace XX with the GPIO number you want to configure
#define POWER_MODE_ON 1
#define POWER_MODE_OFF 0

#define MAX_LINE_LENGTH 256


static const char *FLUSH_TAG = "Flush_Task"; // Tag for logging

static FILE *csv_file = NULL;

extern SemaphoreHandle_t actionT_semaphore;
extern SemaphoreHandle_t actionP_semaphore;
extern SemaphoreHandle_t actionA_semaphore;

typedef struct {
    float Temperature;
    float PHydrogen;
    uint16_t TDSolids;
    float Doxygen;
    float Turbidity;
    char date[20];
} WaterParams;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
} Date;

extern Date LastDate;
extern WaterParams LastParams;



void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param, bool mode_flag);

void LMPD_SYSTEM_Time(char *date, bool mode_flag);
void LMPD_SYSTEM_handleActionF(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_save_parameters(bool mode_flag);



void LMPD_SYSTEM_handleActionT_sd(onewire_bus_handle_t handle_ds, bool mode_flag);

void LMPD_SYSTEM_PM(bool power_mode);


void flushTask(void *pvParameters);



/*
void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param);
void LMPD_SYSTEM_handleActionA(esp_spp_cb_param_t *param);
void LMPD_SYSTEM_handleActionD(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param);*/



#endif /* MSG_HANDLER_H_ */