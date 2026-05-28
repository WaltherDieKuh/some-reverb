/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleGainAudioProcessorEditor::SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor& p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
    , gainAttachment(audioProcessor.apvts, ParameterIDs::gain, gainSlider)
{
    gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);
    gainSlider.setTextValueSuffix(" dB");

    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(gainLabel);
    addAndMakeVisible(gainSlider);

    setSize(240, 180);
}

SimpleGainAudioProcessorEditor::~SimpleGainAudioProcessorEditor() = default;

void SimpleGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void SimpleGainAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto header = area.removeFromTop(24);
    gainLabel.setBounds(header);
    gainSlider.setBounds(area);
}