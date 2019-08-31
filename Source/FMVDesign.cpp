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

#include "FMVDesign.h"

dsp::IIR::Coefficients<float> *designFMV(float l, float m, float t, float Fs)
{
    //JCM800 values
    float R1 = 220e3;
    float R2 = 1e6;
    float R3 = 22e3;
    float R4 = 33e3;

    float C1 = 470e-12;
    float C2 = 22e-9;
    float C3 = 22e-9;

    float b1 = t * C1 * R1 + m * C3 * R3 + l * (C1 * R2 + C2 * R2) + 
        (C1 * R3 + C2 * R3);

    float b2 = t * (C1 * C2 * R1 * R4 + C1 * C3 * R1 * R4 ) 
        - m * m * (C1 * C3 * R3 * R3  + C2 * C3 * R3 * R3 )
        + m * (C1 * C3 * R1 * R3 + C1 * C3 * R3 * R3 + C2 * C3 * R3 * R3)
        + l * (C1 * C2 * R1 * R2 + C1 * C2 * R2 * R4 + C1 * C3 * R2 * R4)
        + l * m * (C1 * C3 * R2 * R3 + C2 * C3 * R2 * R3)
        + (C1 * C2 * R1 * R3 + C1 * C2 * R3 * R4 + C1 * C3 * R3 * R4);

    float b3 = l*m*(C1 * C2 * C3 * R1 * R2 * R3 + C1 * C2 * C3 * R2 * R3 * R4 )
        - m * m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4 )
        + m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4 )
        + t * C1 * C2 * C3 * R1 * R3 * R4 - t * m * C1 * C2 * C3 * R1 * R3 * R4
        + t * l * C1 * C2 * C3 * R1 * R2 * R4;

    float a0 = 1;

    float a1 = (C1 * R1 + C1 * R3 + C2 * R3 + C2 * R4 + C3 * R4 )
        + m * C3 * R3 + l * (C1 * R2 + C2 * R2);

    float a2 = m * (C1 * C3 * R1 * R3 - C2 * C3 * R3 * R4 + C1 * C3 * R3 * R3
        + C2 * C3 * R3 * R3 ) + l * m * (C1 * C3 * R2 * R3 + C2 * C3 * R2 * R3 )
        - m*m * (C1 * C3 * R3 * R3 + C2 * C3 * R3 * R3 ) + l*(C1 * C2 * R2 * R4
        + C1 * C2 * R1 * R2 + C1 * C3 * R2 * R4 + C2 * C3 * R2 * R4 )
        + (C1 * C2 * R1 * R4 + C1 * C3 * R1 * R4 + C1 * C2 * R3 * R4
        + C1 * C2 * R1 * R3 + C1 * C3 * R3 * R4 + C2 * C3 * R3 * R4 );

    float a3 = l*m*(C1 * C2 * C3 * R1 * R2 * R3 + C1 * C2 * C3 * R2 * R3 * R4 )
        - m * m * (C1 * C2 * C3 * R1 * R3 * R3 + C1 * C2 * C3 * R3 * R3 * R4)
        + m * (C1 * C2 * C3 * R3 * R3 * R4 + C1 * C2 * C3 * R1 * R3 * R3 
                - C1 * C2 * C3 * R1 * R3 * R4 ) 
        + l * C1 * C2 * C3 * R1 * R2 * R4 + C1 * C2 * C3 * R1 * R3 * R4;

    //bilinear transform
    float c = 2*Fs;
    float c2 = c*c;
    float c3 = c*c*c;

    float  B0 = -b1 * c- b2 * c2 - b3 * c3;
    float  B1 = -b1 * c+ b2 * c2 + 3 * b3 * c3;
    float  B2 = b1 * c+ b2 * c2 - 3 * b3 * c3;
    float  B3 = b1 * c- b2 * c2 + b3 * c3;

    float  A0 = -a0 - a1 * c- a2 * c2 - a3 * c3;
    float  A1 = -3 * a0 - a1 * c+ a2 * c2 + 3 * a3 * c3;
    float  A2 = -3 * a0 + a1 * c+ a2 * c2 - 3 * a3 * c3;
    float  A3 = -a0 + a1 * c- a2 * c2 + a3 * c3;

    return new dsp::IIR::Coefficients<float>(B0, B1, B2, B3, A0, A1, A2, A3);
}
