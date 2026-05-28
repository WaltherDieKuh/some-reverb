/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

SomeReverbAudioProcessor::SomeReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#else
    : AudioProcessor()
#endif
    , apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

SomeReverbAudioProcessor::~SomeReverbAudioProcessor() = default;

const juce::String SomeReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SomeReverbAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SomeReverbAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool SomeReverbAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double SomeReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SomeReverbAudioProcessor::getNumPrograms()
{
    return 1;
}

int SomeReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SomeReverbAudioProcessor::setCurrentProgram(int)
{
}

const juce::String SomeReverbAudioProcessor::getProgramName(int)
{
    return {};
}

void SomeReverbAudioProcessor::changeProgramName(int, const juce::String&)
{
}

void SomeReverbAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
}

void SomeReverbAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SomeReverbAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void SomeReverbAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto sizeValue = apvts.getRawParameterValue(ParameterIDs::size)->load();
    auto shapeValue = apvts.getRawParameterValue(ParameterIDs::shape)->load();
    auto dampingValue = apvts.getRawParameterValue(ParameterIDs::damping)->load();
    auto wetnessValue = apvts.getRawParameterValue(ParameterIDs::wetness)->load();
    auto mixValue = apvts.getRawParameterValue(ParameterIDs::mix)->load();

    // ===== DRY SIGNAL SPEICHERN =====
    juce::AudioBuffer<float> dryBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
        dryBuffer.copyFrom(channel, 0, buffer, channel, 0, buffer.getNumSamples());

    // ===== REVERB-PARAMETER SETZEN =====
    juce::Reverb::Parameters reverbParams;
    reverbParams.roomSize = sizeValue;
    reverbParams.damping = dampingValue;
    reverbParams.wetLevel = wetnessValue;
    reverbParams.dryLevel = 0.0f;                // 0.0: Wir mixen selbst
    reverbParams.width = 1.0f;
    reverb.setParameters(reverbParams);

    // ===== WET SIGNAL MIT JUCE::REVERB GENERIEREN =====
    juce::AudioBuffer<float> wetBuffer = buffer;
    reverb.processStereo(wetBuffer.getWritePointer(0), wetBuffer.getWritePointer(1), 
                         buffer.getNumSamples());

    // ===== MIX: DRY + WET KOMBINIEREN =====
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        auto* dryData = dryBuffer.getReadPointer(channel);
        auto* wetData = wetBuffer.getReadPointer(channel);
        auto* outputData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Mix-Formel: (dry * (1 - mix)) + (wet * mix)
            outputData[sample] = (dryData[sample] * (1.0f - mixValue)) + 
                                 (wetData[sample] * mixValue);
        }
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
}

bool SomeReverbAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SomeReverbAudioProcessor::createEditor()
{
    return new SomeReverbAudioProcessorEditor(*this);
}

void SomeReverbAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SomeReverbAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout
SomeReverbAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    juce::NormalisableRange<float> range(0.0f, 1.0f, 0.001f);
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::mix,
        "Mix",
        range,
        0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::size,
        "Size",
        range,
        1.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::shape,
        "Shape",
        range,
        4.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::damping,
        "Damping",
        range,
        0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        ParameterIDs::wetness,
        "Wetness",
        range,
        1.0f));
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SomeReverbAudioProcessor();
}