#include "SEN_turb.h"

char* sen0189_processing(float voltage)
{    
    float ntu = - (2572.2 * voltage * voltage) + (8700.5 * voltage) - 4100.9;
    //float ntu =  (- 519.5 * voltage * voltage) + (2500.48 * voltage) - 3000;
    //float ntu =  -(6351.747 * voltage * voltage) + (13672.136 * voltage) - 4352.9;

    printf("%f", ntu );
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
