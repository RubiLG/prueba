#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class PruebaVSTAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit PruebaVSTAudioProcessorEditor (PruebaVSTAudioProcessor&);
    ~PruebaVSTAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    PruebaVSTAudioProcessor& audioProcessor;

    juce::Slider gainSlider, bassSlider, trebleSlider;
    juce::Label titleLabel, gainLabel, bassLabel, trebleLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment, bassAttachment, trebleAttachment;

    void setupSlider (juce::Slider& slider, juce::Label& label, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PruebaVSTAudioProcessorEditor)
};
