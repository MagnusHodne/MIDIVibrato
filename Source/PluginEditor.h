#pragma once

#include "PluginProcessor.h"
#include "HorizontalMeter.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (MidiPluginProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiPluginProcessor& processorRef;

    Gui::HorizontalMeter amplitudeMeter, rateMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
