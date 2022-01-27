#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiPluginEditor::MidiPluginEditor(MidiPluginProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          amplitudeMeter([&]() { return processorRef.getAmplitude(); }),
          rateMeter([&]() { return processorRef.getRate(); }) {
    juce::ignoreUnused(processorRef);
    addAndMakeVisible(amplitudeMeter);
    addAndMakeVisible(rateMeter);

    addAndMakeVisible(inputCCSlider);
    inputCCSlider.setMinAndMaxValues(1, 127);
    inputCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(ampCCSlider);
    ampCCSlider.setMinAndMaxValues(1, 127);
    ampCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(rateCCSlider);
    rateCCSlider.setMinAndMaxValues(1, 127);
    rateCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
}

void MidiPluginEditor::paint(juce::Graphics &g) {
    g.fillAll(Colours::darkgrey);
}

void MidiPluginEditor::resized() {
    amplitudeMeter.setBounds(100, 100, 200, 15);
    rateMeter.setBounds(100, 120, 200, 15);
}

