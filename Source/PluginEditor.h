#pragma once

#include "PluginProcessor.h"
#include "MidiMeter.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    explicit AudioPluginAudioProcessorEditor (MidiPluginProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
    {
        juce::ignoreUnused (processorRef);
        addAndMakeVisible(amplitudeMeter);
        addAndMakeVisible(rateMeter);

        addAndMakeVisible(inputCCSlider);
        addAndMakeVisible(ampCCSlider);
        addAndMakeVisible(rateCCSlider);


        // Make sure that before the constructor has finished, you've set the
        // editor's size to whatever you need it to be.
        setSize (400, 300);
        startTimerHz(30); //Runs meters in 30fps
    }

    ~AudioPluginAudioProcessorEditor() override = default;

    //==============================================================================
    void paint (juce::Graphics& g) override {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }

    void resized() override {
        amplitudeMeter.setBounds(100, 100, 200, 15);
        rateMeter.setBounds(100, 120, 200, 15);
    }

    void timerCallback() override {
        auto detector = processorRef.getDetector();
        amplitudeMeter.setValue(detector.getAmplitude());
        rateMeter.setValue(detector.getRate());

        amplitudeMeter.repaint();
        rateMeter.repaint();
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiPluginProcessor& processorRef;

    Gui::MidiMeter amplitudeMeter, rateMeter;
    juce::Slider inputCCSlider, ampCCSlider, rateCCSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
