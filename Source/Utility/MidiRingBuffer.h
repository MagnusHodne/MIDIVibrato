#pragma once

#include <deque>
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiRingBuffer {
    public:
        explicit MidiRingBuffer(float numSecondsToHold, double sampleRate, int blockSize);

        void push(juce::MidiBuffer &buffer);

        float getRawFrequency();

        int getFrequency(float minFrequency, float maxFrequency);

        float getRawRms();

        int getRms();

        void setSmoothingRampLength(double newLength);

        void reset(double sampleRate, int blockSize);

        void setSecondsToHold(float newTime);

        void setFrequencyAttack(float newValue);

        void setFrequencyRelease(float newValue);

        void setRmsAttack(float newValue);

        void setRmsRelease(float newValue);

    private:
        void write(int numSamplesToWrite);

        //Method for arbitrarily moving the writeHead any number of positions
        void moveWritePos(int increment);

        std::vector<int> data; //Holds MIDI msg value
        std::deque<int> crossingPositions; //TODO - implement a lock-free FIFO instead
        int writeHead = 0;
        int value = 0;
        int spb; //Block size
        double sr; //Sample rate

        float rmsSum = 0.f;

        double ampAttack{}, freqAttack = 0.5;
        juce::LinearSmoothedValue<float> amplitude, frequency;
    };
}