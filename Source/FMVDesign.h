/* Discretised FMV tonestack based on the paper "DISCRETIZATION OF
 * THE ’59 FENDER BASSMAN TONE STACK" by David T. Yeh and Julius
 * O. Smith
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//arguments are the positions of low, mid and treble controls and the sampling
//frequency
dsp::IIR::Coefficients<float> *designFMV(float l, float m, float t, float Fs);
