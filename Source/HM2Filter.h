/* Simulates the Graphic EQ circuit of the HM-2 with all knobs at max settings
 *
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
