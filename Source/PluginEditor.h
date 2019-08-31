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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class DistortionAudioProcessorEditor  : public AudioProcessorEditor, FilenameComponentListener, Timer
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&, AudioProcessorValueTreeState&);
    ~DistortionAudioProcessorEditor();

    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
    
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void filenameComponentChanged(FilenameComponent* f) override;
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionAudioProcessor& processor;

    AudioProcessorValueTreeState& valueTreeState;

    Slider pedalGain, ampGain, bass, middle, treble, volume;
    TextButton tightButton;
    FilenameComponent fileBrowser;

    Label tightLabel, pedalGainLabel, ampGainLabel, bassLabel, middleLabel, trebleLabel,
          volumeLabel, titleLabel, speakerLabel;

    std::unique_ptr<SliderAttachment> pedalGainAttachment, ampGainAttachment,
        bassAttachment, middleAttachment, trebleAttachment, volumeAttachment;
    std::unique_ptr<ButtonAttachment> tightAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};
