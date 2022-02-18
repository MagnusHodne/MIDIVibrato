#pragma once

#include "PluginProcessor.h"
#include "Component/MidiMeter.h"
#include "CustomLookAndFeel.h"

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
    Gui::CustomLookAndFeel customLookAndFeel;

    Gui::MidiMeter amplitudeMeter, rateMeter;
    juce::Slider inputCCSlider, ampCCSlider, rateCCSlider, timeSlider, frAtSlider, frRelSlider, rmsAtSlider, rmsRelSlider;
    juce::AudioProcessorValueTreeState::SliderAttachment inputSliderAtt, ampSliderAtt, rateSliderAtt, timeSliderAtt, frAtSliderAtt, frRelSliderAtt, rmsAtSliderAtt, rmsRelSliderAtt;

    Label ampMeterLabel{"", "Amplitude"},
            rateMeterLabel{"", "Rate"},
            timeLabel{"", "Analysis time"},
            frAtLabel{"", "Frequency attack time"},
            frRelLabel{"", "Frequency release time"},
            rmsAtLabel{"", "RMS attack time"},
            rmsRelLabel{"", "RMS release time"},
            inputCCLabel{"", "Input CC"},
            ampCCLabel{"", "Vibrato depth CC"},
            rateCCLabel{"", "Vibrato rate CC"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginEditor)
};
