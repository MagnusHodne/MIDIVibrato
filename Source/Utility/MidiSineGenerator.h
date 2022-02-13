#pragma once
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
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
            angleDelta = cyclesPerSample * 2.0 * MathConstants<double>::pi;
        }
        void fillBuffer(MidiBuffer& bufferToFill){
            int currentSample;
            int prevSample = -1;

            for (auto sample = 0; sample < spb; ++sample){
                currentSample = static_cast<int>(jmap(sin(currentAngle), 0., 127.));
                //auto currentSample = std::sin(currentAngle);
                currentAngle += angleDelta;
                if(currentSample != prevSample){
                    bufferToFill.addEvent(MidiMessage::controllerEvent(1, controller, currentSample), sample);
                }
                prevSample = currentSample;
            }
        }
    private:
        float freq;
        double currentSampleRate, currentAngle = 0.0, angleDelta = 0.0;
        int spb, controller;
    };
}