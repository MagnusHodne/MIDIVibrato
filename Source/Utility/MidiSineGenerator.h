#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiSineGenerator {
    public:
        MidiSineGenerator(int inputController, float frequency, double sampleRate, int samplesPerBlock);

        /// Changes the sample rate and frequency of the plug-in. Call this on each prepareToPlay!
        void updateValues(double sampleRate, int samplesPerBlock);

        void fillBuffer(juce::MidiBuffer &bufferToFill);

    private:
        float freq;
        double currentSampleRate, currentAngle = 0.0, angleDelta = 0.0;
        int spb, controller;
    };
}