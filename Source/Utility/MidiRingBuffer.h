#pragma once

#include <tuple>
#include <vector>
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiRingBuffer {
    public:
        explicit MidiRingBuffer(int samplesPerBuffer, int numSamplesToHold) : data(numSamplesToHold),
                                                                              samplesPerBlock(samplesPerBuffer) {}

        void push(juce::MidiBuffer &buffer) {

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
                value = metadata.getMessage().getControllerValue();
                prevTime = time;
            }
            //Make sure we write the remaining values in the buffer
            if (prevTime < samplesPerBlock) {
                write(samplesPerBlock - prevTime);
            }
        }

        int getSum() {
            return sum;
        }

        float getAverage() {
            return (float) sum / (float) data.size();
        }

        void shouldCenterBe63() {

        }

    private:
        void write(int numSamplesToWrite) {
            if (numSamplesToWrite == 0) return;
            for (int i = 0; i < numSamplesToWrite; i++) {
                sum -= data[writeHead]; //Subtract the "oldest" value (the one immediately after the write head)
                sum += value;

                data[writeHead] = value;
                moveWritePos(1);
            }
        }

        void iterateOver() {
            auto startPos = writeHead;
            for (int i = 0; i < data.size(); i++) {
                moveWritePos(1);
            }
            jassert(startPos == writeHead); //If this is false, it means we haven't cycled through the buffer correctly!
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

        std::vector<juce::uint16> data; //Holds MIDI msg value
        int writeHead = 0;
        juce::uint16 value = 0;
        int samplesPerBlock;
        int sum = 0;
    };
}