#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Waveshaper.h"
#include "FMVDesign.h"

#define N_WAVESHAPE_ITERATIONS 10 //not used

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor() :
      parameters (*this, nullptr, Identifier ("parameters"), 
              {
              std::make_unique<AudioParameterBool> ("tight",
                                                     "Tight",
                                                     false),
              std::make_unique<AudioParameterFloat> ("pedalGain",
                                                     "HM-2 Gain",
                                                     20,
                                                     40,
                                                     30),
              std::make_unique<AudioParameterFloat> ("ampGain",
                                                     "Amplifier Gain",
                                                     0,
                                                     30,
                                                     15),
              //the bass parameter needs more precision, because most of the
              //change happens between 0 and 0.1
              std::make_unique<AudioParameterFloat> ("bass",
                                                     "Bass",
                                                     NormalisableRange<float>(
                                                         0, 1, 0.00001),
                                                     0.05),
              std::make_unique<AudioParameterFloat> ("middle",
                                                     "Middle",
                                                     0,
                                                     1,
                                                     0.5),
              std::make_unique<AudioParameterFloat> ("treble",
                                                     "Treble",
                                                     0,
                                                     1,
                                                     0.5),
              std::make_unique<AudioParameterFloat> ("volume",
                                                     "Volume",
                                                     10,
                                                     40,
                                                     25)
              }),
      pedalOversampling (4, 4, dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR),
      ampOversampling   (4, 4, dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR)

#ifndef JucePlugin_PreferredChannelConfigurations
      ,AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    pedalWaveshaper.functionToUse = waveshape;
    ampWaveshaper.functionToUse = waveshape;

    tightParameter     = parameters.getRawParameterValue("tight");
    pedalGainParameter = parameters.getRawParameterValue("pedalGain");
    ampGainParameter   = parameters.getRawParameterValue("ampGain");
    bassParameter      = parameters.getRawParameterValue("bass");
    middleParameter    = parameters.getRawParameterValue("middle");
    trebleParameter    = parameters.getRawParameterValue("treble");
    volumeParameter    = parameters.getRawParameterValue("volume");

    speakerFile.reset(new File());
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
}

//==============================================================================
const String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    //NB: some hosts don't cope very well if you tell them there are 0
    //programs, so this should be at least 1, even if you're not really
    //implementing programs.
    return 1;   
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;

    dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    pedalOversampling.initProcessing(samplesPerBlock);
    ampOversampling.initProcessing(samplesPerBlock);

    pedalWaveshaper.prepare(spec);
    ampWaveshaper.prepare(spec);

    //pedal input filter, measured from circuit simulation at the first opamp input
    //rollof is 6dB/octave which means it is a first order filter
    pedalInputFilter.prepare(spec);
    pedalInputFilter.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(sampleRate, 60);

    //tighter filter which cuts more bass before the signal is clipped by the
    //pedal waveshaper
    tightFilter.prepare(spec);
    tightFilter.coefficients = dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(sampleRate, 700);

    pedalLowpassFilter.prepare(spec);
    pedalLowpassFilter.coefficients = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 7000);

    ampInputFilter.prepare(spec);
    //amp input filter, basicaly the high-pass filter from a tube screamer,
    //which is also used in the Marshall valvestate 8100
    ampInputFilter.coefficients =
        dsp::IIR::Coefficients<float>::makeFirstOrderHighPass(sampleRate, 600);
    hm2Filter.prepare(spec);
    fmvFilter.prepare(spec);

    ampLowpassFilter.prepare(spec);
    ampInputFilter.coefficients = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 7000);

    speakerConvolution.prepare(spec);
}

void DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DistortionAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    dsp::AudioBlock<float> block (buffer);
    dsp::ProcessContextReplacing<float> normalContext (block);

    //Change the signal level on the way in to control the amount of distortion
    gain.setGainDecibels(*pedalGainParameter);
    gain.process(normalContext);

    pedalInputFilter.process(normalContext);

    if(*tightParameter > 0.5)
        tightFilter.process(normalContext);

    //oversample the input signal to reduce aliasing from waveshaper
    dsp::AudioBlock<float> oversampledBlock = pedalOversampling.processSamplesUp(block);
    dsp::ProcessContextReplacing<float> oversampledContext (oversampledBlock);

    //apply distortion then downsample back to original sample rate
    pedalWaveshaper.process(oversampledContext);
    pedalOversampling.processSamplesDown(block);

    //apply HM2 filter
    hm2Filter.process(normalContext);
    pedalLowpassFilter.process(normalContext);

    //amp's input filter
    ampInputFilter.process(normalContext);

    //attenuate before amp's waveshaper
    gain.setGainDecibels(*ampGainParameter);
    gain.process(normalContext);

    //oversample again to apply the amp's distortion
    oversampledBlock = ampOversampling.processSamplesUp(block);
    dsp::ProcessContextReplacing<float> oversampledContext2 (oversampledBlock);
    ampWaveshaper.process(oversampledContext2);
    ampOversampling.processSamplesDown(block);

    //apply amp's FMV tonestack filter
    fmvFilter.coefficients = designFMV(*bassParameter, *middleParameter,
            *trebleParameter, sampleRate);
    fmvFilter.process(normalContext);

    //removes some harshness
    ampLowpassFilter.process(normalContext);

    speakerConvolution.process(normalContext);

    //attenuate the output signal
    finalAttenuation.setGainDecibels(*volumeParameter - 30);
    finalAttenuation.process(normalContext);
}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this, parameters);
    //return new GenericAudioProcessorEditor (this);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());

    xml->setAttribute("speakerFile", speakerFile->getFullPathName());
    
    copyXmlToBinary (*xml, destData);
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    String filename;
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        filename = xmlState->getStringAttribute("speakerFile");
        speakerFile.reset(new File(filename));
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (ValueTree::fromXml (*xmlState));

        setSpeakerIR(*speakerFile);
    }
}

//I know this argument is ignored but I'm not going to change it with less than
//1 hour left to the deadline
//The editor sets speakerFile to the new file so this still works
void DistortionAudioProcessor::setSpeakerIR(File f)
{
    speakerConvolution.loadImpulseResponse(*speakerFile, false, false, 0);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}
