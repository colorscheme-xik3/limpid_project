#include "SEN_tds.h"

float sen0244_processing(float voltage, float temperature)
{    
    // Calculate the temperature compensation coefficient
    float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);   
    
    // Apply the compensation coefficient to the raw voltage
    float compensationVoltage = voltage / compensationCoefficient;  
    
    // Use a polynomial equation to calculate TDS concentration from the compensated voltage
    return (133.42 * compensationVoltage * compensationVoltage * compensationVoltage) 
           - (255.86 * compensationVoltage * compensationVoltage) 
           + (857.39 * compensationVoltage) * 0.5;
}
