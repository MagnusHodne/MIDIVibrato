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
          timeSliderAtt(p.getApvts(), "numSec", timeSlider),
          frAtSliderAtt(p.getApvts(), "freqAttack", frAtSlider),
          frRelSliderAtt(p.getApvts(), "freqRelease", frRelSlider),
          rmsAtSliderAtt(p.getApvts(), "rmsAttack", rmsAtSlider),
          rmsRelSliderAtt(p.getApvts(), "rmsRelease", rmsRelSlider)
{

    auto aamv = [&](juce::Label& l, juce::Component& c) {
        addAndMakeVisible(l);
        addAndMakeVisible(c);
        l.setJustificationType(juce::Justification::right);
    };
    aamv(ampMeterLabel, amplitudeMeter);
    aamv(rateMeterLabel, rateMeter);
    aamv(timeLabel, timeSlider);
    aamv(frAtLabel, frAtSlider);
    aamv(frRelLabel, frRelSlider);
    aamv(rmsAtLabel, rmsAtSlider);
    aamv(rmsRelLabel, rmsRelSlider);

    aamv(inputCCLabel, inputCCSlider);
    inputCCSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    aamv(ampCCLabel, ampCCSlider);
    ampCCSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);
    aamv(rateCCLabel, rateCCSlider);
    rateCCSlider.setSliderStyle(juce::Slider::SliderStyle::IncDecButtons);

    setResizable(false, false);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, (15*15)+40);
}

void MidiPluginEditor::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::darkgrey);
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

    auto place = [&](juce::Label& l, juce::Slider& s) {
        s.setBounds(inputArea.removeFromTop(height));
        l.setBounds(labelArea.removeFromTop(height));
    };

    place(timeLabel, timeSlider);
    place(frAtLabel, frAtSlider);
    place(frRelLabel, frRelSlider);
    place(rmsAtLabel, rmsAtSlider);
    place(rmsRelLabel, rmsRelSlider);
    place(inputCCLabel, inputCCSlider);
    place(ampCCLabel, ampCCSlider);
    place(rateCCLabel, rateCCSlider);
}

