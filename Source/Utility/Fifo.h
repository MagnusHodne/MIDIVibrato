#pragma once

#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    using namespace juce;

    class MidiRingBuffer {
    public:

        explicit MidiRingBuffer(int initialBufferSize){
            initialBufferSize = std::clamp(initialBufferSize, 1, 128);
            ringBuffer = std::vector<int>(initialBufferSize, 0);
        }

        void reset(int numBuffersToHold) {
            ringBuffer = std::vector<int>(numBuffersToHold, 0);
        }

        /// You should call this method on each processBlock, passing in a buffer containing
        /// only the CC you wish to calculate RMS on
        /// \param buffer a MidiBuffer containing only the CC values you wish to analyze
        void push(const MidiBuffer &buffer) {
            ringBuffer[writeHead] = calculateRmsOfSingleBuffer(buffer);
            writeHead = writeHead == ringBuffer.size()-1 ? 0 : writeHead + 1;
        }

        int getRms() {
            int sum = 0;
            for(auto rms : ringBuffer){
                sum += rms;
            }
            return sum/static_cast<int>(ringBuffer.size());
        }

    private:
        static int calculateRmsOfSingleBuffer(const MidiBuffer &buffer) {
            if (buffer.isEmpty()) return 0;

            float sum = 0.f;

            for (auto metadata: buffer) {
                const auto value = metadata.getMessage().getControllerValue();
                sum += std::powf(static_cast<float>(value - halfMidi), 2.0);
            }
            return static_cast<int>(std::sqrt(sum/static_cast<float>(buffer.getNumEvents())));
        }

        std::vector<int> ringBuffer;

        int writeHead = 0;
        //int size;

        //Since we are calculating amplitude from "center", we have to define that as our
        //baseline value
        static constexpr int halfMidi = 128/2-1;
    };
}