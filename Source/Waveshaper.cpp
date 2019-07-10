#include "Waveshaper.h"

float waveshape(float x) {
    float y;
    /*
    if(x >= -1 && x < -0.08905) {
        y = -3.0/4.0 * ( 1 - pow( 1 - (fabs(x) - 0.032847), 12) +1.0/3.0*(fabs(x) - 0.032847)) + 0.01;
    } else if(x >= -0.08905 && x < 0.320018) {
        y = -6.153 * pow(x, 2) + 3.9375 * x;
    }
    else {
        y = 0.630035;
    }
    */

    y = std::tanh(x);
    return y;
}
