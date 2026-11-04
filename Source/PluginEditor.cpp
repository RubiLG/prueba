#include "PluginEditor.h"

PruebaVSTAudioProcessorEditor::PruebaVSTAudioProcessorEditor (PruebaVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (480, 300);

    titleLabel.setText ("PRUEBA", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setFont (juce::Font (28.0f, juce::Font::bold));
    addAndMakeVisible (titleLabel);

    setupSlider (gainSlider, gainLabel, "GAIN");
    setupSlider (bassSlider, bassLabel, "BASS");
    setupSlider (trebleSlider, trebleLabel, "TREBLE");

    gainSlider.setRange (-24.0, 12.0, 0.1);
    bassSlider.setRange (-12.0, 12.0, 0.1);
    trebleSlider.setRange (-12.0, 12.0, 0.1);

    gainAttachment = std::make_unique<SliderAttachment> (audioProcessor.parameters, "gain", gainSlider);
    bassAttachment = std::make_unique<SliderAttachment> (audioProcessor.parameters, "bass", bassSlider);
    trebleAttachment = std::make_unique<SliderAttachment> (audioProcessor.parameters, "treble", trebleSlider);
}

void PruebaVSTAudioProcessorEditor::setupSlider (juce::Slider& slider,
                                                  juce::Label& label,
                                                  const juce::String& text)
{
    label.setText (text, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    label.setFont (juce::Font (15.0f, juce::Font::bold));
    addAndMakeVisible (label);

    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 24);
    slider.setTextValueSuffix (" dB");
    addAndMakeVisible (slider);
}

void PruebaVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff171923));
    g.setColour (juce::Colour (0xff303747));
    g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (12.0f), 18.0f);
    g.setColour (juce::Colour (0xff8be9fd));
    g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (12.0f), 18.0f, 2.0f);
}

void PruebaVSTAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (28);
    titleLabel.setBounds (area.removeFromTop (55));

    auto controls = area.reduced (8, 12);
    const int columnWidth = controls.getWidth() / 3;

    auto gainArea = controls.removeFromLeft (columnWidth);
    auto bassArea = controls.removeFromLeft (columnWidth);
    auto trebleArea = controls;

    gainLabel.setBounds (gainArea.removeFromTop (28));
    gainSlider.setBounds (gainArea);
    bassLabel.setBounds (bassArea.removeFromTop (28));
    bassSlider.setBounds (bassArea);
    trebleLabel.setBounds (trebleArea.removeFromTop (28));
    trebleSlider.setBounds (trebleArea);
}
