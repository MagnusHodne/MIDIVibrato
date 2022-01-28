#pragma once

#include "PluginProcessor.h"
#include "Component/MidiMeter.h"

//==============================================================================
class MidiPluginEditor : public juce::AudioProcessorEditor {
public:
    explicit MidiPluginEditor(MidiPluginProcessor &p);


    ~MidiPluginEditor() override = default;

    //==============================================================================
    void paint(juce::Graphics &g) override;

    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiPluginProcessor &processorRef;

    Gui::MidiMeter amplitudeMeter, rateMeter;
    juce::Slider inputCCSlider, ampCCSlider, rateCCSlider, buffersSlider, scalingSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment bufferSliderAtt, scalingSliderAtt;

    Label ampMeterLabel{"ampLabel","Amplitude"},
    rateMeterLabel{"rateLabel","Rate"},
    bufferLabel {"bufLabel", "Number of buffers"},
    scalingLabel {"scaleLabel", "Scaling/Multiplier"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginEditor)
};
