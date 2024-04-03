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
#include "../LMPD_rom/ROM_msd.h"

//#include "ds18b20.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t actionT_semaphore;
extern SemaphoreHandle_t actionP_semaphore;
extern SemaphoreHandle_t actionA_semaphore;

typedef struct {
    float Temperature;
    float PHydrogen;
    uint16_t TDSolids;
    float Doxygen;
    float Turbidity;
} WaterParams;

extern WaterParams LastParams;


void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionP(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionS(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionD(esp_spp_cb_param_t *param, bool mode_flag);
void LMPD_SYSTEM_handleActionB(esp_spp_cb_param_t *param);


void LMPD_SYSTEM_handleActionT_sd(onewire_bus_handle_t handle_ds, bool mode_flag);




/*
void LMPD_SYSTEM_handleActionT(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param);
void LMPD_SYSTEM_handleActionA(esp_spp_cb_param_t *param);
void LMPD_SYSTEM_handleActionD(onewire_bus_handle_t handle_ds, esp_spp_cb_param_t *param);*/



#endif /* MSG_HANDLER_H_ */