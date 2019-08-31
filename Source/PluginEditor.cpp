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

#include "PluginProcessor.h"
#include "PluginEditor.h"

const Colour ORANGE(243, 122, 44);
const Colour DARK_GREY(20, 20, 20);

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts),
    fileBrowser("fileBrowser", *p.speakerFile, false, false, false, "*.wav", "",
            "Select an IR")
{

    getLookAndFeel().setColour (Slider::thumbColourId, ORANGE);
    getLookAndFeel().setColour (Slider::rotarySliderFillColourId, Colour(0.0f, 0.0f, 0.0f, 0.0f));
    getLookAndFeel().setColour (Slider::rotarySliderOutlineColourId, DARK_GREY);

    getLookAndFeel().setColour (Label::textColourId, ORANGE);

    getLookAndFeel().setColour (TextButton::buttonColourId, DARK_GREY);
    getLookAndFeel().setColour (TextButton::buttonOnColourId, ORANGE);
    getLookAndFeel().setColour (TextButton::textColourOffId, ORANGE);
    getLookAndFeel().setColour (TextButton::textColourOnId, DARK_GREY);

    getLookAndFeel().setColour (ComboBox::backgroundColourId, DARK_GREY);
    getLookAndFeel().setColour (ComboBox::textColourId, ORANGE);
    getLookAndFeel().setColour (ComboBox::arrowColourId, ORANGE);
    getLookAndFeel().setColour (ComboBox::outlineColourId, ORANGE);

    getLookAndFeel().setColour (PopupMenu::backgroundColourId, DARK_GREY);
    getLookAndFeel().setColour (PopupMenu::textColourId, ORANGE);

    tightButton.setClickingTogglesState(true);
    tightAttachment.reset(new ButtonAttachment (valueTreeState, "tight",
                tightButton));

    pedalGain.setSliderStyle(Slider::RotaryVerticalDrag);
    pedalGain.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    pedalGain.setPopupDisplayEnabled(false, false, nullptr);
    pedalGain.setTextValueSuffix(" dB");
    pedalGainAttachment.reset (new SliderAttachment (valueTreeState, "pedalGain",
                pedalGain));

    ampGain.setSliderStyle(Slider::RotaryVerticalDrag);
    ampGain.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    ampGain.setPopupDisplayEnabled(false, false, nullptr);
    ampGain.setTextValueSuffix(" dB");
    ampGainAttachment.reset (new SliderAttachment (valueTreeState, "ampGain",
                ampGain));

    bass.setSliderStyle(Slider::RotaryVerticalDrag);
    bass.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    bass.setPopupDisplayEnabled(false, false, nullptr);
    bassAttachment.reset (new SliderAttachment (valueTreeState, "bass",
                bass));

    //the bass slider should be logarithmic
    //0.05 is roughly halfway between 1 and 0 when looking at the frequency
    //response on a log scale
    bass.setSkewFactorFromMidPoint(0.05);
    bass.setRange(bass.getRange(), 0.00001);

    middle.setSliderStyle(Slider::RotaryVerticalDrag);
    middle.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    middle.setPopupDisplayEnabled(false, false, nullptr);
    middleAttachment.reset (new SliderAttachment (valueTreeState, "middle",
                middle));
    
    treble.setSliderStyle(Slider::RotaryVerticalDrag);
    treble.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    treble.setPopupDisplayEnabled(false, false, nullptr);
    trebleAttachment.reset (new SliderAttachment (valueTreeState, "treble",
                treble));

    volume.setSliderStyle(Slider::RotaryVerticalDrag);
    volume.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
    volume.setPopupDisplayEnabled(false, false, nullptr);
    volume.setTextValueSuffix(" dB");
    volumeAttachment.reset (new SliderAttachment (valueTreeState, "volume",
                volume));

    addAndMakeVisible(&tightButton);
    addAndMakeVisible(&pedalGain);
    addAndMakeVisible(&ampGain);
    addAndMakeVisible(&bass);
    addAndMakeVisible(&middle);
    addAndMakeVisible(&treble);
    addAndMakeVisible(&volume);

    titleLabel    .setText("SWEDISH CHAINSAW", dontSendNotification);
    titleLabel    .setJustificationType(Justification::centred);
    titleLabel    .setFont(Font(20, Font::bold));
    tightLabel    .setText("TIGHT", dontSendNotification);
    tightLabel    .setJustificationType(Justification::centred);
    pedalGainLabel.setText("HM-2 GAIN", dontSendNotification);
    pedalGainLabel.setJustificationType(Justification::centred);
    ampGainLabel  .setText("AMP GAIN", dontSendNotification);
    ampGainLabel  .setJustificationType(Justification::centred);
    bassLabel     .setText("BASS", dontSendNotification);
    bassLabel     .setJustificationType(Justification::centred);
    middleLabel   .setText("MIDDLE", dontSendNotification);
    middleLabel   .setJustificationType(Justification::centred);
    trebleLabel   .setText("TREBLE", dontSendNotification);
    trebleLabel   .setJustificationType(Justification::centred);
    volumeLabel   .setText("VOLUME", dontSendNotification);
    volumeLabel   .setJustificationType(Justification::centred);
    speakerLabel  .setText("SPEAKER", dontSendNotification);
    speakerLabel  .setJustificationType(Justification::centred);

    addAndMakeVisible (titleLabel);
    addAndMakeVisible (tightLabel);
    addAndMakeVisible (pedalGainLabel);
    addAndMakeVisible (ampGainLabel);
    addAndMakeVisible (bassLabel);
    addAndMakeVisible (middleLabel);
    addAndMakeVisible (trebleLabel);
    addAndMakeVisible (volumeLabel);
    addAndMakeVisible (speakerLabel);

    fileBrowser.addListener(this);
    addAndMakeVisible (fileBrowser);

    setSize (7*75, 50+75+20+50);
    startTimer(50);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

void DistortionAudioProcessorEditor::filenameComponentChanged (FilenameComponent* f)
{
    processor.speakerFile.reset(new File(f->getCurrentFile()));
    processor.setSpeakerIR(*processor.speakerFile);
}

//make sure the editor shows the correct file name after a preset load
void DistortionAudioProcessorEditor::timerCallback()
{
    fileBrowser.setCurrentFile(*processor.speakerFile, true, dontSendNotification);
}
//==============================================================================
void DistortionAudioProcessorEditor::paint (Graphics& g)
{
    auto b = g.getClipBounds();
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

    //draw a bar separating the knobs from the file picker
    g.setColour(DARK_GREY);
    const float x = 10.f;
    const float y = 50.f + 75.f + 15.f + 8.5f;
    g.fillRect(Rectangle<float>(x, y, b.getWidth() - 2*x, 2));
}

void DistortionAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto titleHeight = 50;
    titleLabel.setBounds (area.removeFromTop (titleHeight));

    auto controlWidth = 75;
    auto controlHeight = 75;
    auto labelHeight = 15;

    auto controlArea = area.removeFromTop(controlHeight);
    auto b = controlArea.removeFromLeft(controlWidth);
    auto margin = 10;
    b.removeFromTop(margin);
    b.removeFromBottom(margin);
    b.removeFromLeft(margin);
    b.removeFromRight(margin);
    tightButton.setBounds(b);
    pedalGain.setBounds(controlArea.removeFromLeft(controlWidth));
    ampGain.setBounds(controlArea.removeFromLeft(controlWidth));
    bass.setBounds(controlArea.removeFromLeft(controlWidth));
    middle.setBounds(controlArea.removeFromLeft(controlWidth));
    treble.setBounds(controlArea.removeFromLeft(controlWidth));
    volume.setBounds(controlArea.removeFromLeft(controlWidth));

    auto labelArea = area.removeFromTop(labelHeight);
    tightLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    pedalGainLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    ampGainLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    bassLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    middleLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    trebleLabel.setBounds(labelArea.removeFromLeft(controlWidth));
    volumeLabel.setBounds(labelArea.removeFromLeft(controlWidth));

    //a little margin around the file picker
    area.removeFromTop(2*margin);
    area.removeFromBottom(margin);
    area.removeFromRight(margin);

    speakerLabel.setBounds(area.removeFromLeft(controlWidth));
    fileBrowser.setBounds(area);
}
