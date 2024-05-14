#include "SEN_tds.h"

float sen0244_processing(float voltage, float temperature)
{    
    float compensationCoefficient=1.0+0.02*(temperature-25.0);   
    float compensationVolatge = voltage / compensationCoefficient;  

    return  (133.42 * compensationVolatge * compensationVolatge * compensationVolatge) - (255.86 * compensationVolatge * compensationVolatge) + (857.39 * compensationVolatge) * 0.5;

}

//falta numero de samples


/*
float sen0244_read_sensor(int numSamples, float sampleDelay){
    // Take n sensor readings every p millseconds where n is numSamples, and p is sampleDelay.
    // Return the average sample value.
    uint32_t runningSampleValue = 0;

    for(int i = 0; i < numSamples; i++) {
        // Read analogue value
        int raw_A1 = (LMPD_I2C_configureADS(ADS1115_SENSOR_ADDR, ADS1115_CONFIG_MSB_A1, ADS1115_CONFIG_LSB));
        runningSampleValue = runningSampleValue + raw_A1;        
    }
    
    float tdsAverage = runningSampleValue / TDS_NUM_SAMPLES;
    return tdsAverage;
}*/
