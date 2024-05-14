#include "SEN_turb.h"

char* sen0189_processing(float voltage)
{    
    float ntu = - (2572.2 * voltage * voltage) + (8700.5 * voltage) - 3850.9;
    if(ntu < 0)
    {
        ntu = 0;
        return "clear";
    }
    else if(ntu > 0 && ntu < 60)
    {
        return "cloudy";
    }
    else if(ntu > 60 && ntu < 400)
    {
        return "moderate";

    }
    else if(ntu > 400)
    {
        ntu = 3000;
        return "dirty";
    }
    else{
        return "error";
    }
}