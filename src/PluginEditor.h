/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SomeReverbAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SomeReverbAudioProcessorEditor(SomeReverbAudioProcessor&);
    ~SomeReverbAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SomeReverbAudioProcessor& audioProcessor;

    juce::Slider mixSlider;
    juce::Label mixLabel;

    juce::AudioProcessorValueTreeState::SliderAttachment mixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SomeReverbAudioProcessorEditor)
};
