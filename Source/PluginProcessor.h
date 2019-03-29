#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Waveshaper.h"
#include "HM2Filter.h"

//==============================================================================
/**
*/
class DistortionAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DistortionAudioProcessor();
    ~DistortionAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setSpeakerIR(File f);

    std::unique_ptr<File> speakerFile;
private:
    AudioProcessorValueTreeState parameters;

    float* tightParameter;
    float* pedalGainParameter;
    float* ampGainParameter;
    float* bassParameter;
    float* middleParameter;
    float* trebleParameter;
    float* volumeParameter;

    dsp::Oversampling<float> pedalOversampling;
    dsp::Oversampling<float> ampOversampling;

    dsp::WaveShaper<float> pedalWaveshaper;
    dsp::WaveShaper<float> ampWaveshaper;

    dsp::Gain<float> gain;
    dsp::Gain<float> finalAttenuation;

    dsp::IIR::Filter<float> pedalInputFilter;
    dsp::IIR::Filter<float> tightFilter;
    dsp::IIR::Filter<float> ampInputFilter;
    HM2Filter hm2Filter;
    dsp::IIR::Filter<float> fmvFilter;
    //these filters remove frequencies above 7000, which are not really
    //reproducable by guitar speakers and sound horrible
    dsp::IIR::Filter<float> pedalLowpassFilter;
    dsp::IIR::Filter<float> ampLowpassFilter;
    dsp::Convolution speakerConvolution;

    float sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessor)
};
