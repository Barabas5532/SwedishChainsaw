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
    using AudioProcessor::processBlock;

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

    std::atomic<float>* tightParameter;
    std::atomic<float>* pedalGainParameter;
    std::atomic<float>* ampGainParameter;
    std::atomic<float>* bassParameter;
    std::atomic<float>* middleParameter;
    std::atomic<float>* trebleParameter;
    std::atomic<float>* volumeParameter;

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
