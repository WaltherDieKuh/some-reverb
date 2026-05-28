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
    , sizeAttachment(audioProcessor.apvts, ParameterIDs::size, sizeSlider)
    , shapeAttachment(audioProcessor.apvts, ParameterIDs::shape, shapeSlider)
    , dampingAttachment(audioProcessor.apvts, ParameterIDs::damping, dampingSlider)
    , wetnessAttachment(audioProcessor.apvts, ParameterIDs::wetness, wetnessSlider)

{
    sizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);

    sizeLabel.setText("Size", juce::dontSendNotification);
    sizeLabel.setJustificationType(juce::Justification::topLeft);

    addAndMakeVisible(sizeLabel);
    addAndMakeVisible(sizeSlider);

    shapeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    shapeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);
    shapeSlider.setTextValueSuffix(" edges");

    shapeLabel.setText("Shape", juce::dontSendNotification);
    shapeLabel.setJustificationType(juce::Justification::centredTop);

    addAndMakeVisible(shapeLabel);
    addAndMakeVisible(shapeSlider);

    dampingSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);

    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.setJustificationType(juce::Justification::topRight);

    addAndMakeVisible(dampingLabel);
    addAndMakeVisible(dampingSlider);

    wetnessSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    wetnessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);
    wetnessSlider.setTextValueSuffix(" %");

    wetnessLabel.setText("Wet Level", juce::dontSendNotification);
    wetnessLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(wetnessLabel);
    addAndMakeVisible(wetnessSlider);


    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 20);

    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(mixLabel);
    addAndMakeVisible(mixSlider);

    setSize(400, 400);
}

SomeReverbAudioProcessorEditor::~SomeReverbAudioProcessorEditor() = default;

void SomeReverbAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void SomeReverbAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    auto topRow = bounds.removeFromTop(bounds.getHeight() / 2);
    auto bottomRow = bounds;
    constexpr int labelHeight = 20;

    auto layoutKnob = [labelHeight](juce::Rectangle<int> area,
                                    juce::Label& label,
                                    juce::Slider& slider)
    {
        area = area.reduced(10);
        label.setBounds(area.removeFromTop(labelHeight));
        slider.setBounds(area);
    };

    const int topCellWidth = topRow.getWidth() / 3;
    layoutKnob(topRow.removeFromLeft(topCellWidth), sizeLabel, sizeSlider);
    layoutKnob(topRow.removeFromLeft(topCellWidth), shapeLabel, shapeSlider);
    layoutKnob(topRow, dampingLabel, dampingSlider);

    const int bottomCellWidth = bottomRow.getWidth() / 2;
    layoutKnob(bottomRow.removeFromLeft(bottomCellWidth), wetnessLabel, wetnessSlider);
    layoutKnob(bottomRow, mixLabel, mixSlider);
}