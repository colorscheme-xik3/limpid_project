#ifndef WPP_HANDLER_H_
#define WPP_HANDLER_H_

#include "../LMPD_ch/MSG_handler.h" 

#include <stdint.h>  


typedef enum {
    WATER_TYPE_FRESH,
    WATER_TYPE_SALT,
    WATER_TYPE_TAP,
} WaterType;

typedef struct {
    float minValue;
    float maxValue;
} WaterParameter;

typedef struct {
    WaterParameter temperature;
    WaterParameter pH;
    WaterParameter tds;
    WaterParameter dissolvedOxygen;
    WaterParameter turbidity;

} WaterQualitySpecs;

extern const WaterQualitySpecs waterQualityStandards[] = {
    {
        {7, 8},    // pH
        {301, 699},     // TDS ppm
        {5.5, 8.5},      // Dissolved oxygen mg/L
        {1, 5}         // NTU
    },
    {
        {7.9, 9},      // SALT water standards
        {700, 2000},
        {7.0, 8.0},
        {1, 80}
    },
    {
        {6.5, 9.5},    // pH
        {50, 300},     // TDS ppm
        {6.5, 8},      // Dissolved oxygen mg/L
        {1, 5}         //    NTU
    },
};


// Function to determine the current water type
char LMPD_SYSTEM_waterType(void);

#endif /* WPP_HANDLER_H_ */
