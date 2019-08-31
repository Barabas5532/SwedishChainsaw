/*
 *  This file is part of Swedish Chainsaw.
 *  Copyright (C) 2019  Barabas Raffai
 *
 *  Swedish Chainsaw is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Swedish Chanisaw is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Swedish Chanisaw.  If not, see <https://www.gnu.org/licenses/>.
 */

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
