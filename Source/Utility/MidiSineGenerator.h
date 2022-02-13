#pragma once
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

class MidiSineGenerator {
public:
    MidiSineGenerator(int inputController, float frequency, double sampleRate, int samplesPerBlock) : freq(frequency), currentSampleRate(sampleRate), spb(samplesPerBlock), controller(inputController){
        updateValues(sampleRate, samplesPerBlock); //Make sure we update the value at least once
    }

    /// Changes the sample rate and frequency of the plug-in. Call this on each prepareToPlay!
    void updateValues(double sampleRate, int samplesPerBlock){
        currentSampleRate = sampleRate;
        spb = samplesPerBlock;

        auto cyclesPerSample = freq / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }
    void fillBuffer(juce::MidiBuffer& bufferToFill){
        int currentSample;
        for (auto sample = 0; sample < spb; ++sample){
            currentSample = static_cast<int>(juce::jmap(std::sin(currentAngle), 0., 127.));
            //auto currentSample = std::sin(currentAngle);
            currentAngle += angleDelta;
            bufferToFill.addEvent(juce::MidiMessage::controllerEvent(1, controller, currentSample), sample);
        }
    }
private:
    float freq;
    double currentSampleRate, currentAngle = 0.0, angleDelta = 0.0;
    int spb, controller;
};