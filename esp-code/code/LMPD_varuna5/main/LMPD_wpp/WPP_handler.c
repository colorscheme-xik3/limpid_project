#include "WPP_HANDLER.h"
#include <string.h> 

// Define WaterQualityStandards array
WaterQualitySpecs waterQualityStandards[] = {
    {
        {7, 8},         // pH
        {301, 699},     // TDS ppm
        {5.5, 8.5},     // Dissolved oxygen mg/L
        {1, 5}          // NTU
    },
    {
        {7.9, 9},       // SALT water standards
        {700, 2000},
        {7.0, 8.0},
        {1, 80}
    },
    {
        {6.5, 9.5},     // pH
        {50, 300},      // TDS ppm
        {6.5, 8},       // Dissolved oxygen mg/L
        {1, 5}          // NTU
    },
};



// Function to determine the current water type based on measured parameters
// Function to determine the current water type based on measured parameters (FreeRTOS task)
void LMPD_SYSTEM_waterType(void *pvParameters) {

    while (1) {
        // Compare parameters with standard values for each water type
        if (
            LastParams.TDSolids >= waterQualityStandards[WATER_TYPE_FRESH].tds.minValue &&
            LastParams.TDSolids <= waterQualityStandards[WATER_TYPE_FRESH].tds.maxValue 
            ) {

            // Update waterType based on detected type
            strcpy(LastParams.waterType, "FRESH WATER");
        }
        else if (
                 LastParams.TDSolids >= waterQualityStandards[WATER_TYPE_SALT].tds.minValue &&
                 LastParams.TDSolids <= waterQualityStandards[WATER_TYPE_SALT].tds.maxValue 
         ) {

            // Update waterType based on detected type
            strcpy(LastParams.waterType, "SALT WATER");
        }
        else if (
                 LastParams.TDSolids >= waterQualityStandards[WATER_TYPE_TAP].tds.minValue &&
                 LastParams.TDSolids <= waterQualityStandards[WATER_TYPE_TAP].tds.maxValue 
                 ) {

            // Update waterType based on detected type
            strcpy(LastParams.waterType, "TAP WATER");
        }
        else {
            // Default to unknown type
            strcpy(LastParams.waterType, "FLOATING PROBE");
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay task execution for 1000 milliseconds (1 second)
    }
}