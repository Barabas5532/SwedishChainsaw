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
