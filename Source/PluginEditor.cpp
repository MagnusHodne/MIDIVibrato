#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiPluginEditor::MidiPluginEditor(MidiPluginProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          amplitudeMeter([&]() { return processorRef.getRms(); }),
          rateMeter([&]() { return processorRef.getFrequency(); }),
          inputSliderAtt(p.getApvts(), "inputCC", inputCCSlider),
          ampSliderAtt(p.getApvts(), "ampCC", ampCCSlider),
          rateSliderAtt(p.getApvts(), "rateCC", rateCCSlider),
          bufferSliderAtt(p.getApvts(), "numBuf", buffersSlider){

    addAndMakeVisible(amplitudeMeter);
    addAndMakeVisible(ampMeterLabel);
    ampMeterLabel.setJustificationType(Justification::right);
    addAndMakeVisible(rateMeter);
    addAndMakeVisible(rateMeterLabel);
    rateMeterLabel.setJustificationType(Justification::right);

    addAndMakeVisible(inputCCSlider);
    inputCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(inputCCLabel);
    inputCCLabel.setJustificationType(Justification::right);

    addAndMakeVisible(ampCCSlider);
    ampCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(ampCCLabel);
    ampCCLabel.setJustificationType(Justification::right);

    addAndMakeVisible(rateCCSlider);
    rateCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(rateCCLabel);
    rateCCLabel.setJustificationType(Justification::right);

    addAndMakeVisible(buffersSlider);
    buffersSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(bufferLabel);
    bufferLabel.setJustificationType(Justification::right);

    setResizable(false, false);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, (15*11)+40);
}

void MidiPluginEditor::paint(juce::Graphics &g) {
    g.fillAll(Colours::darkgrey);
}

void MidiPluginEditor::resized() {

    auto height = 15;
    auto bounds = getLocalBounds().reduced(20);
    auto labelArea = bounds.removeFromLeft(bounds.getWidth()/2);
    auto inputArea = bounds;

    amplitudeMeter.setBounds(inputArea.removeFromTop(height));
    ampMeterLabel.setBounds(labelArea.removeFromTop(height));

    rateMeter.setBounds(inputArea.removeFromTop(height));
    rateMeterLabel.setBounds(labelArea.removeFromTop(height));

    labelArea.removeFromTop(height*2);
    inputArea.removeFromTop(height*2);

    buffersSlider.setBounds(inputArea.removeFromTop(height));
    bufferLabel.setBounds(labelArea.removeFromTop(height));

    inputCCSlider.setBounds(inputArea.removeFromTop(height));
    inputCCLabel.setBounds(labelArea.removeFromTop(height));

    ampCCSlider.setBounds(inputArea.removeFromTop(height));
    ampCCLabel.setBounds(labelArea.removeFromTop(height));

    rateCCSlider.setBounds(inputArea.removeFromTop(height));
    rateCCLabel.setBounds(labelArea.removeFromTop(height));
}

