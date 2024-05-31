#include "SEN_ph.h"

// Definition of calibration variables
float pHValues[] = {4.01, 6.86, 9.18};
float voltageReadings[] = {0, 0, 0};
//float voltageReadings_SEN0161[] = {890, 1195, 1454};
float voltageReadings_SEN0161[] = {7035, 9520, 11552};
//float voltageReadings_SEN0169[] = {2029, 1547, 1146};
float voltageReadings_SEN0169[] = {16355, 12408, 9300};


// pH sensor selection
void LMPD_PH_selector(int sensor_ph) {
    if (PH_SENSOR) {
        // Copy the content of voltageReadings_SEN0169 to voltageReadings
        for (int i = 0; i < sizeof(voltageReadings_SEN0169) / sizeof(int); i++) {
            voltageReadings[i] = voltageReadings_SEN0169[i];
        }
    } else {
        // Copy the content of voltageReadings_SEN0161 to voltageReadings
        for (int i = 0; i < sizeof(voltageReadings_SEN0161) / sizeof(int); i++) {
            voltageReadings[i] = voltageReadings_SEN0161[i];
        }
    }
}

// Calibration type selection
float LMPD_PH_calibrator(int calib_points, int voltage, float temperature) {
    float ph_Value;
   
    if (CALIBRATION_POINTS) {
        // Use 3 points calibration method
        ph_Value = LMPD_PH_calibrate3p(voltage, temperature, PH_SENSOR);
    } else {
        // Use 2 points calibration method
        ph_Value = LMPD_PH_calibrate2p(voltage);
    }

    return ph_Value;
}

// Calibration function with 3 points calibration
float LMPD_PH_calibrate3p(int voltage, float temperature, int sensor_ph) {
    
    float temperature_coefficient = 0.03; // General temperature coefficient for pH
    float temperature_compensation = (25 - temperature) * temperature_coefficient;


    // Calculate the slopes (m1, m2) and intercepts (b1, b2) between the three calibration points
    float m1 = (pHValues[1] - pHValues[0]) / (voltageReadings[1] - voltageReadings[0]);
    float b1 = pHValues[0] - (m1 * voltageReadings[0]);

    float m2 = (pHValues[2] - pHValues[1]) / (voltageReadings[2] - voltageReadings[1]);
    float b2 = pHValues[1] - (m2 * voltageReadings[1]);

    // Print the original slopes for debugging
    printf("Original m1: %f, m2: %f\n", m1, m2);
   
    // Calculate the calibrated pH value using the piecewise linear equation
    float calibratepH_3p_c;
    if (sensor_ph) {
        calibratepH_3p_c = (voltage >= voltageReadings[1]) ? (voltage * m1) + b1 : (voltage * m2) + b2;
    } else {
        calibratepH_3p_c = (voltage <= voltageReadings[1]) ? (voltage * m1) + b1 : (voltage * m2) + b2;
    }

    if(calibratepH_3p_c <= 4.01)
    {
        // Apply temperature compensation
        calibratepH_3p_c += temperature_compensation;
    }
          
    printf("Updated m1: %f, m2: %f\n", m1, m2);
        
    return calibratepH_3p_c;
}


// Calibration function with 2 points calibration
float LMPD_PH_calibrate2p(int voltage) {
    float slope = (pHValues[1] - pHValues[0]) / (voltageReadings[1] - voltageReadings[0]);
    float intercept = pHValues[0] - slope * voltageReadings[0];
    float calibratedpH = slope * voltage + intercept;
    return calibratedpH;
}
