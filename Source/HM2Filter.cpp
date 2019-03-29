#include "HM2Filter.h"

HM2Filter::HM2Filter()
{

}

void HM2Filter::prepare( const juce::dsp::ProcessSpec& spec) 
{
    //these coefficients were obtained from MATLAB
    namespace IIR = juce::dsp::IIR;
    filter1.coefficients = IIR::Coefficients<float>::Ptr(new IIR::Coefficients<float>(
            1.016224171805848, -1.996244766867333, 0.980170456681741,
            1, -1.996244766867333, 0.996394628487589));
    filter2.coefficients = IIR::Coefficients<float>::Ptr(new IIR::Coefficients<float>(
            1.048098514125369, -1.946884930731663, 0.914902628855116,
            1, -1.946884930731663, 0.963001142980485));
    filter3.coefficients = IIR::Coefficients<float>::Ptr(new IIR::Coefficients<float>(
            1.260798129602192, -1.896438187817481, 0.674002337997260,
            1, -1.896438187817481, 0.934800467599452));
    filter1.prepare(spec);
    filter2.prepare(spec);
    filter3.prepare(spec);
}

void HM2Filter::process(const juce::dsp::ProcessContextReplacing<float>context) {
    filter1.process(context);
    filter2.process(context);
    filter3.process(context);
}
