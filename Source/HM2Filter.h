/* Simulates the Graphic EQ circuit of the HM-2 with all knobs at max settings
 */
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class HM2Filter
{

    public:
    HM2Filter(void);

    void prepare( const juce::dsp::ProcessSpec& spec);

    void process(const juce::dsp::ProcessContextReplacing<float>context);

    private:
        juce::dsp::IIR::Filter<float> filter1, filter2, filter3;
};
