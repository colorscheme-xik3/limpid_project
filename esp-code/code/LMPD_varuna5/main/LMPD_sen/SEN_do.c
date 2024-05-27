#include "SEN_do.h"

const uint16_t saturation_table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};

float sen0237_processing(float voltage_mv, float temperature_c)
{
    #if TWO_POINT_CALIBRATION == 0
        // Single-point calibration
        float V_saturation = CAL05_V + (TEMPERATURE_COEFFICIENT * temperature_c) - (CAL05_T * TEMPERATURE_COEFFICIENT);
        return (voltage_mv * saturation_table[(int)temperature_c] / V_saturation) / 1000;
    #else
        // Two-point calibration
        float V_saturation = (temperature_c - CAL2_T) * ((CAL1_V - CAL2_V) / (CAL1_T - CAL2_T)) + CAL2_V;
        return (voltage_mv * saturation_table[(int)temperature_c] / V_saturation) / 1000;
    #endif
}
