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

    int getRms() const {
        return detector->getRms();
    }

    int getFrequency() {
        return detector->getFrequency();
    }

private:
    juce::AudioProcessorValueTreeState parameters;

    std::unique_ptr<Detector> detector;
    double sr;
    int spb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPluginProcessor)
};
