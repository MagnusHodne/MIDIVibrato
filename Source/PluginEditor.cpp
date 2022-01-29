#include "PluginProcessor.h"
#include "PluginEditor.h"

MidiPluginEditor::MidiPluginEditor(MidiPluginProcessor &p)
        : AudioProcessorEditor(&p),
          processorRef(p),
          amplitudeMeter([&]() { return processorRef.getAmplitude(); }),
          rateMeter([&]() { return processorRef.getRate(); }),
          inputSliderAtt(p.getApvts(), "inputCC", inputCCSlider),
          ampSliderAtt(p.getApvts(), "ampCC", ampCCSlider),
          rateSliderAtt(p.getApvts(), "rateCC", rateCCSlider),
          bufferSliderAtt(p.getApvts(), "numBuf", buffersSlider),
          scalingSliderAtt(p.getApvts(), "scaling", scalingSlider) {

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
    addAndMakeVisible(amptCCLabel);
    amptCCLabel.setJustificationType(Justification::right);

    addAndMakeVisible(rateCCSlider);
    rateCCSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(rateCCLabel);
    rateCCLabel.setJustificationType(Justification::right);

    addAndMakeVisible(buffersSlider);
    buffersSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    addAndMakeVisible(bufferLabel);
    bufferLabel.setJustificationType(Justification::right);

    addAndMakeVisible(scalingSlider);
    scalingSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    scalingSlider.setNumDecimalPlacesToDisplay(1);
    addAndMakeVisible(scalingLabel);
    scalingLabel.setJustificationType(Justification::right);

    setResizable(false, false);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, (15*10)+40);
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

    scalingSlider.setBounds(inputArea.removeFromTop(height));
    scalingLabel.setBounds(labelArea.removeFromTop(height));

    buffersSlider.setBounds(inputArea.removeFromTop(height));
    bufferLabel.setBounds(labelArea.removeFromTop(height));

    inputCCSlider.setBounds(inputArea.removeFromTop(height));
    inputCCLabel.setBounds(labelArea.removeFromTop(height));

    ampCCSlider.setBounds(inputArea.removeFromTop(height));
    amptCCLabel.setBounds(labelArea.removeFromTop(height));

    rateCCSlider.setBounds(inputArea.removeFromTop(height));
    rateCCLabel.setBounds(labelArea.removeFromTop(height));
}

