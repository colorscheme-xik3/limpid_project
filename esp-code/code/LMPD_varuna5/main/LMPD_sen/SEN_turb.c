#include "SEN_turb.h"

char* sen0189_processing(float voltage)
{    
    float ntu = - (2572.2 * voltage * voltage) + (8700.5 * voltage) - 3850.9;
    if(ntu < NTU_CLEAR_THRESHOLD)
    {
        ntu = 0;
        return "clear";
    }
    else if(ntu <= NTU_CLOUDY_THRESHOLD)
    {
        return "cloudy";
    }
    else if(ntu <= NTU_MODERATE_THRESHOLD)
    {
        return "moderate";
    }
    else
    {
        ntu = MAX_NTU_VALUE;
        return "dirty";
    }
}
