/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SomeReverbAudioProcessorEditor::SomeReverbAudioProcessorEditor(SomeReverbAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
    , mixAttachment(audioProcessor.apvts, ParameterIDs::mix, mixSlider)
{
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);
    mixSlider.setTextValueSuffix(" %");

    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(mixLabel);
    addAndMakeVisible(mixSlider);

    setSize(240, 180);
}

SomeReverbAudioProcessorEditor::~SomeReverbAudioProcessorEditor() = default;

void SomeReverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void SomeReverbAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto header = area.removeFromTop(24);
    mixLabel.setBounds(header);
    mixSlider.setBounds(area);
}