#pragma once

#include <tuple>
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiRingBuffer {
    public:
        explicit MidiRingBuffer(float numSecondsToHold, double sampleRate, int blockSize)
                : data((int)(sampleRate * numSecondsToHold), 0),
                  spb(blockSize),
                  sr(sampleRate) {
            reset(sampleRate, blockSize);
        }

        void push(juce::MidiBuffer &buffer) {
            amplitude.skip(spb);
            frequency.skip(spb);

            //Write whatever the previous value was, up until the next point in the buffer (or the buffer end)

            //First, write whatever the previous value was, up until the first sample in the buffer
            //Then, write until the next sample in the buffer
            //Finally, write until the end of the buffer
            int prevTime = 0;

            for (auto metadata: buffer) {
                auto time = metadata.samplePosition;

                /*
                 * It might seem counter-intuitive to write the value before we update it, but the reasoning is that we
                 * want to write whatever the previous value was up until the current sample position first
                 */
                write(time - prevTime);
                value = juce::jmap(metadata.getMessage().getControllerValue(), 0, 127, -63, 64); //Map so that MIDI 63 = 0;
                prevTime = time;
            }
            //Make sure we write the remaining values in the buffer
            if (prevTime < spb) {
                write(spb - prevTime);
            }
            auto rms = std::sqrt(rmsSum / static_cast<float>(data.size()));
            amplitude.setTargetValue(rms);
        }

        float getRawFrequency() {
            /*float numSecondsInBuffer = (float) data.size() / (float) sr;
            float numCycles = (float) numCrossings / 2;
            float freq = numCycles/numSecondsInBuffer;
            return freq;*/
            return frequency.getCurrentValue();
        }

        float getRawRms() {
            //The max rms of a sine wave with
            return amplitude.getCurrentValue();
        }

        int getRms() {
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
                //sum -= data[writeHead]; //Subtract the "oldest" value (the one immediately after the write head)
                //sum += value;

                auto oldestValue = data[writeHead];
                rmsSum -= std::powf(static_cast<float>(oldestValue), 2.0);
                rmsSum += std::powf(static_cast<float>(value), 2.0);

                data[writeHead] = value;
                moveWritePos(1);
            }
        }

        void calculate() {
            auto startPos = writeHead;
            numCrossings = 0;

            for(size_t i = 0; i < data.size(); i++){
                auto current = data[writeHead];

                moveWritePos(1);
                auto next = data[writeHead];

                if (current > 0 && next <= 0 || current < 0 && next >= 0) {
                    numCrossings++;
                }
            }
            float numSecondsRecorded = (float)data.size()/(float)sr;
            float numCycles = numCrossings/2;
            frequency.setTargetValue(numCycles/numSecondsRecorded);

            jassert(startPos == writeHead);
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
        int writeHead = 0;
        int value = 0;
        int spb; //Block size
        double sr; //Sample rate

        float rmsSum = 0.f;
        int numCrossings = 0;

        double rampLengthInSeconds = 0.5;
        juce::LinearSmoothedValue<float> amplitude, frequency;
    };
}