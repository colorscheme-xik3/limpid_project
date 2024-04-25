#include "WPP_HANDLER.h"
#include <string.h> 

// Function to determine the current water type based on measured parameters
char LMPD_SYSTEM_waterType(void) {
    // Retrieve the last gathered water parameters (assumed to be in LastParams)
    WaterParams lastParams = LastParams;

    // Compare pH, TDS, dissolved oxygen, and total bacteria with standard values for each water type
    if (lastParams.PHydrogen >= waterQualityStandards[WATER_TYPE_FRESH].pH.minValue &&
        lastParams.PHydrogen <= waterQualityStandards[WATER_TYPE_FRESH].pH.maxValue &&
        lastParams.TDSolids >= waterQualityStandards[WATER_TYPE_FRESH].tds.minValue &&
        lastParams.TDSolids <= waterQualityStandards[WATER_TYPE_FRESH].tds.maxValue &&
        lastParams.Doxygen >= waterQualityStandards[WATER_TYPE_FRESH].dissolvedOxygen.minValue &&
        lastParams.Doxygen <= waterQualityStandards[WATER_TYPE_FRESH].dissolvedOxygen.maxValue &&
        lastParams.Turbidity >= waterQualityStandards[WATER_TYPE_FRESH].turbidity.minValue &&
        lastParams.Turbidity <= waterQualityStandards[WATER_TYPE_FRESH].turbidity.maxValue) {
        return 'F';  // FRESH water type
    }
    else if (lastParams.PHydrogen >= waterQualityStandards[WATER_TYPE_SALT].pH.minValue &&
             lastParams.PHydrogen <= waterQualityStandards[WATER_TYPE_SALT].pH.maxValue &&
             lastParams.TDSolids >= waterQualityStandards[WATER_TYPE_SALT].tds.minValue &&
             lastParams.TDSolids <= waterQualityStandards[WATER_TYPE_SALT].tds.maxValue &&
             lastParams.Doxygen >= waterQualityStandards[WATER_TYPE_SALT].dissolvedOxygen.minValue &&
             lastParams.Doxygen <= waterQualityStandards[WATER_TYPE_SALT].dissolvedOxygen.maxValue &&
             lastParams.Turbidity >= waterQualityStandards[WATER_TYPE_SALT].turbidity.minValue &&
             lastParams.Turbidity <= waterQualityStandards[WATER_TYPE_SALT].turbidity.maxValue) {
        return 'S';  // SALT water type
    }
    else if (lastParams.PHydrogen >= waterQualityStandards[WATER_TYPE_TAP].pH.minValue &&
             lastParams.PHydrogen <= waterQualityStandards[WATER_TYPE_TAP].pH.maxValue &&
             lastParams.TDSolids >= waterQualityStandards[WATER_TYPE_TAP].tds.minValue &&
             lastParams.TDSolids <= waterQualityStandards[WATER_TYPE_TAP].tds.maxValue &&
             lastParams.Doxygen >= waterQualityStandards[WATER_TYPE_TAP].dissolvedOxygen.minValue &&
             lastParams.Doxygen <= waterQualityStandards[WATER_TYPE_TAP].dissolvedOxygen.maxValue &&
             lastParams.Turbidity >= waterQualityStandards[WATER_TYPE_TAP].turbidity.minValue &&
             lastParams.Turbidity <= waterQualityStandards[WATER_TYPE_TAP].turbidity.maxValue) {
        return 'T';  // TAP water type
    }
    else {
        return 'U';  // Unknown water type
    }
}
