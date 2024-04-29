#ifndef WPP_HANDLER_H_
#define WPP_HANDLER_H_

#include "../LMPD_ch/MSG_handler.h" 



#include <stdint.h>  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

typedef struct {
    struct {
        float minValue;
        float maxValue;
    } pH;
    struct {
        uint16_t minValue;
        uint16_t maxValue;
    } tds;
    struct {
        float minValue;
        float maxValue;
    } dissolvedOxygen;
    struct {
        uint16_t minValue;
        uint16_t maxValue;
    } turbidity;
} WaterQualitySpecs;

// Define WATER_TYPE enum
typedef enum {
    WATER_TYPE_FRESH,
    WATER_TYPE_SALT,
    WATER_TYPE_TAP
} WATER_TYPE;


// Function to determine the current water type
void LMPD_SYSTEM_waterType(void *pvParameters);

#endif /* WPP_HANDLER_H_ */
