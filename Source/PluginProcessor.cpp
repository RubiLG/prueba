#include "PluginProcessor.h"
#include "PluginEditor.h"

PruebaVSTAudioProcessor::PruebaVSTAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
PruebaVSTAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain", juce::NormalisableRange<float> (-24.0f, 12.0f, 0.1f), 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "bass", "Bass", juce::NormalisableRange<float> (-12.0f, 12.0f, 0.1f), 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>(
        "treble", "Treble", juce::NormalisableRange<float> (-12.0f, 12.0f, 0.1f), 0.0f));

    return { params.begin(), params.end() };
}

void PruebaVSTAudioProcessor::prepareToPlay (double sampleRate, int)
{
    currentSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    lowState[0] = lowState[1] = 0.0f;
    highState[0] = highState[1] = 0.0f;
}

void PruebaVSTAudioProcessor::releaseResources()
{
}

bool PruebaVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();

    return mainIn == mainOut
        && (mainOut == juce::AudioChannelSet::mono()
            || mainOut == juce::AudioChannelSet::stereo());
}

void PruebaVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                            juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const auto channels = juce::jmin (buffer.getNumChannels(), 2);
    const auto samples = buffer.getNumSamples();

    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, samples);

    const float gainDb = parameters.getRawParameterValue ("gain")->load();
    const float bassDb = parameters.getRawParameterValue ("bass")->load();
    const float trebleDb = parameters.getRawParameterValue ("treble")->load();

    const float gain = juce::Decibels::decibelsToGain (gainDb);
    const float bassAmount = juce::Decibels::decibelsToGain (bassDb) - 1.0f;
    const float trebleAmount = juce::Decibels::decibelsToGain (trebleDb) - 1.0f;

    // Simple first-order tone shaping: low shelf-like bass and high shelf-like treble.
    const float lowCutoff = 180.0f;
    const float highCutoff = 3500.0f;
    const float lowAlpha = 1.0f - std::exp (-2.0f * juce::MathConstants<float>::pi
                                           * lowCutoff / static_cast<float> (currentSampleRate));
    const float highAlpha = 1.0f - std::exp (-2.0f * juce::MathConstants<float>::pi
                                            * highCutoff / static_cast<float> (currentSampleRate));

    for (int ch = 0; ch < channels; ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        float low = lowState[ch];
        float high = highState[ch];

        for (int i = 0; i < samples; ++i)
        {
            const float input = data[i];
            low += lowAlpha * (input - low);
            high += highAlpha * (input - high);
            const float highPart = input - high;
            data[i] = (input + bassAmount * low + trebleAmount * highPart) * gain;
        }

        lowState[ch] = low;
        highState[ch] = high;
    }
}

juce::AudioProcessorEditor* PruebaVSTAudioProcessor::createEditor()
{
    return new PruebaVSTAudioProcessorEditor (*this);
}

void PruebaVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PruebaVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (parameters.state.getType()))
        parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PruebaVSTAudioProcessor();
}
