#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "VibratoDetector.h"

class MidiPluginProcessor : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener {
public:
    MidiPluginProcessor();

    ~MidiPluginProcessor() override;

    //================Stuff you care about============================

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

    void releaseResources() override;

    //===================Boilerplate==================================

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    juce::AudioProcessorEditor *createEditor() override;

    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String &newName) override;

    void getStateInformation(juce::MemoryBlock &destData) override;

    void setStateInformation(const void *data, int sizeInBytes) override;

    //========================Custom functions============================

    void parameterChanged(const juce::String &parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState &getApvts() { return parameters; }

    int getAmplitude() const {
        return detector.getAmplitude();
    }

    int getRate() const {
        return detector.getRate();
    }

private:
    VibratoDetector detector;
    juce::AudioProcessorValueTreeState parameters;

    int numBuffers = 5;
    float multiplier = 2.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginProcessor)
};
