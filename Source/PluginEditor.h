/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimpleGainAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor&);
    ~SimpleGainAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SimpleGainAudioProcessor& audioProcessor;

    juce::Slider gainSlider;
    juce::Label gainLabel;

    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainAudioProcessorEditor)
};
