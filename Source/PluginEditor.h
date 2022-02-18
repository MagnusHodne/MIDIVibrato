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
    juce::Slider inputCCSlider, ampCCSlider, rateCCSlider, buffersSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment bufferSliderAtt, inputSliderAtt, ampSliderAtt, rateSliderAtt;

    Label ampMeterLabel{"", "Amplitude"},
            rateMeterLabel{"", "Rate"},
            inputCCLabel{"", "Input CC"},
            ampCCLabel{"", "Vibrato depth CC"},
            rateCCLabel{"", "Vibrato rate CC"},
            bufferLabel{"", "Number of buffers"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginEditor)
};
