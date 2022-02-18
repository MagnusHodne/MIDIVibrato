#pragma once

#include <deque>
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiRingBuffer {
    public:
        explicit MidiRingBuffer(float numSecondsToHold, double sampleRate, int blockSize)
                : data((int)(sampleRate * numSecondsToHold), 0),
                  crossingPositions(0),
                  spb(blockSize),
                  sr(sampleRate) {
            reset(sampleRate, blockSize);
        }

        void push(juce::MidiBuffer &buffer) {
            amplitude.skip(spb);
            frequency.skip(spb);

            int currentSamplePos = 0;

            for (auto metadata: buffer) {
                auto time = metadata.samplePosition;

                //First we write whatever the previous value was, up until the value should actually change
                write(time - currentSamplePos);
                auto currentValue = value;
                //Map our values so that we deal with 0 as our center value (lets us process MIDI similarly to how we process audio)
                value = juce::jmap(metadata.getMessage().getControllerValue(), 0, 127, -63, 64);

                if (currentValue >= 0 && value < 0 || currentValue <= 0 && value > 0) {
                    crossingPositions.emplace_back(writeHead);
                }
                currentSamplePos = time;
            }
            //Make sure we write the remaining values in the buffer
            if (currentSamplePos < spb) {
                write(spb - currentSamplePos);
            }

            auto rms = std::sqrt(rmsSum / static_cast<float>(data.size()));
            amplitude.setTargetValue(rms);

            float numSecondsInBuffer = (float) data.size() / (float) sr;
            float numCycles = (float) crossingPositions.size() / 2;
            float freq = numCycles/numSecondsInBuffer;
            frequency.setTargetValue(freq);
        }

        float getRawFrequency() {
            return frequency.getCurrentValue();
        }

        float getRawRms() {
            return amplitude.getCurrentValue();
        }

        int getRms() {
            //A sine wave should have an RMS of 0.707 times the max value (which in our case is 64 * 0.707);
            return static_cast<int>(juce::jmap(getRawRms(), 0.f, 64.f * 0.707f, 0.f, 127.f));
        }

        void setSmoothingRampLength(double newLength) {
            rampLengthInSeconds = newLength;
            amplitude.reset(sr, rampLengthInSeconds);
            frequency.reset(sr, rampLengthInSeconds);
        }

        void reset(double sampleRate, int blockSize) {
            sr = sampleRate;
            spb = blockSize;
            amplitude.reset(sampleRate, rampLengthInSeconds);
            amplitude.setCurrentAndTargetValue(0.f);
            frequency.reset(sampleRate, rampLengthInSeconds);
            frequency.setCurrentAndTargetValue(0.f);
        }

    private:
        void write(int numSamplesToWrite) {
            if (numSamplesToWrite == 0) return;
            for (int i = 0; i < numSamplesToWrite; i++) {
                auto oldestValue = data[writeHead];
                {
                    //Calculate RMS
                    rmsSum -= std::powf(static_cast<float>(oldestValue), 2.0);
                    rmsSum += std::powf(static_cast<float>(value), 2.0);
                }

                data[writeHead] = value;
                moveWritePos(1);
                if(!crossingPositions.empty() && crossingPositions.front() == writeHead){
                    crossingPositions.pop_front();
                }
            }
        }

        //Method for arbitrarily moving the writeHead any number of positions
        void moveWritePos(int increment) {
            //If we aren't out of bounds when incrementing by the desired amount, simply increment
            if (writeHead + increment < data.size()) writeHead += increment;
                //Wrap around
            else {
                auto delta = (int) data.size() - writeHead;
                writeHead = 0 + increment - delta;
            }
        }

        std::vector<int> data; //Holds MIDI msg value
        std::deque<int> crossingPositions; //TODO - implement a lock-free FIFO instead
        int writeHead = 0;
        int value = 0;
        int spb; //Block size
        double sr; //Sample rate

        float rmsSum = 0.f;

        double rampLengthInSeconds = 0.5;
        juce::LinearSmoothedValue<float> amplitude, frequency;
    };
}