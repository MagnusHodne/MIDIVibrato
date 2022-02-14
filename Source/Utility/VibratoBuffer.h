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
            initialBufferSize = std::clamp(initialBufferSize, 128, maxNumBuffers);
            buffer = std::vector<int>(initialBufferSize, 0);
        }

        void reset(int newBufferSize) {
            newBufferSize = std::clamp(newBufferSize, 128, maxNumBuffers);
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

        int getSum(){
            int sum = 0;
            for(auto val : buffer){
                sum += val;
            }
            return sum;
        }

        int getSize(){
            return buffer.capacity();
        }

    private:
        std::vector<int> buffer;
        int writeHead = 0;
        int maxNumBuffers = 2048; //The maximum number of buffers to hold. It takes 187,5 buffers to detect a 1Hz frequency at 48 KHz
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

        float getRate(double sampleRate, int samplesPerBlock){
            float secondsRecorded = (float)samplesPerBlock * (float)rateBuffer.getSize()/ (float)sampleRate;
            float numCycles = (float)rateBuffer.getSum()/2;
            float frequency = numCycles/secondsRecorded;
            return frequency;
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

            int numEvents = buffer.getNumEvents();
            int numCrossings = 0;
            auto it = buffer.begin();

            for(int i = 0; i < numEvents -1; i++){
                auto current = it.operator*().getMessage().getControllerValue();
                auto next = it.operator++().operator*().getMessage().getControllerValue();
                if(current > halfMidi && next <= halfMidi || current < halfMidi && next >= halfMidi){
                    numCrossings++;
                }
            }

            return numCrossings;
        }

        AverageRingBuffer rmsBuffer;
        AverageRingBuffer rateBuffer;

        //Since we are calculating amplitude from "center", we have to define that as our
        //baseline value
        static constexpr int halfMidi = 128 / 2 - 1; //= 63
    };
}