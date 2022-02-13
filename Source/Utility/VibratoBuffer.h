#pragma once

#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    using namespace juce;

    ///Simple structure for storing average values that you can then get the total average from
    ///Works as a "write-only" ring buffer, so you cannot read values from it (only the average)
    class AverageRingBuffer {
    public:

        explicit AverageRingBuffer(int initialBufferSize) {
            initialBufferSize = std::clamp(initialBufferSize, 1, 256);
            buffer = std::vector<int>(initialBufferSize, 0);
        }

        void reset(int newBufferSize) {
            newBufferSize = std::clamp(newBufferSize, 1, 256);
            buffer = std::vector<int>(newBufferSize, 0);
        }

        void push(int averageVal) {
            buffer[writeHead] = averageVal;
            writeHead = (writeHead == buffer.size() - 1) ? 0 : writeHead + 1; //This is how we get a ring buffer...
        }

        float getAverage() {
            float sum = 0.f;
            for (auto val: buffer) {
                sum += static_cast<float>(val);
            }
            return sum / static_cast<float>(buffer.size());
        }

        int getSize(){
            return buffer.size();
        }

    private:
        std::vector<int> buffer;
        int writeHead = 0;
    };

    class VibratoBuffer {
    public:

        explicit VibratoBuffer(int initialBufferSize) : rmsBuffer(initialBufferSize), rateBuffer(initialBufferSize) {
        }

        void reset(int numBuffersToHold) {
            rmsBuffer.reset(numBuffersToHold);
            rateBuffer.reset(numBuffersToHold);
        }

        /// You should call this method on each processBlock, passing in a buffer containing
        /// only the CC you wish to calculate RMS on
        /// \param buffer a MidiBuffer containing only the CC values you wish to analyze
        void calculateValues(const MidiBuffer &buffer) {
            rmsBuffer.push(calculateRmsOfSingleBuffer(buffer));
            rateBuffer.push(calculateNumCrossings(buffer));
        }

        int getRms() {
            return static_cast<int>(rmsBuffer.getAverage());
        }

        /// Gets the average number of crossings per buffer, based upon all the buffers stored. Remember that num crossings is double the Hz!
        /// \return The average rate of all the buffers stored
        float getAvgNumCrossings() {
            return rateBuffer.getAverage();
        }

    private:
        static int calculateRmsOfSingleBuffer(const MidiBuffer &buffer) {
            if (buffer.isEmpty()) return 0;

            float sum = 0.f;

            for (auto metadata: buffer) {
                const auto value = metadata.getMessage().getControllerValue();
                sum += std::powf(static_cast<float>(value - halfMidi), 2.0);
            }
            return static_cast<int>(std::sqrt(sum / static_cast<float>(buffer.getNumEvents())));
        }

        /// Calculates how many zero crossings there are in the supplied buffer (zero-crossing defined as crossing a MIDI value of 63
        static int calculateNumCrossings(const MidiBuffer &buffer) {
            if (buffer.isEmpty()) return 0;

            int sum = 0;
            int prev = -1;

            for (auto metadata: buffer) {
                const auto value = metadata.getMessage().getControllerValue();
                if(prev < 0) {
                    if(value == halfMidi){
                        sum++;
                    }
                    prev = value;
                    continue;
                } else if(prev != value) {
                    if(prev < halfMidi && value >= halfMidi) sum++;
                    if(prev > halfMidi && value <= halfMidi) sum++;
                }
                prev = value;
            }

            return sum;
        }

        AverageRingBuffer rmsBuffer;
        AverageRingBuffer rateBuffer;

        //Since we are calculating amplitude from "center", we have to define that as our
        //baseline value
        static constexpr int halfMidi = 128 / 2 - 1; //= 63
    };
}