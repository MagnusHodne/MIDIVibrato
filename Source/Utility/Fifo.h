#pragma once

#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"


//Code from https://github.com/Thrifleganger/level-meter-demo/blob/master/Source/Utility/Fifo.h
namespace Utility {
    using namespace juce;

    class MidiRingBuffer {
    public:

        MidiRingBuffer(){
            ringBuffer = std::vector<int>(size, 0);
        }

        void reset(int numBuffersToHold) {
            size = numBuffersToHold;
            ringBuffer = std::vector<int>(size, 0);
        }

        /// You should call this method on each processBlock, passing in a buffer containing
        /// only the CC you wish to calculate RMS on
        /// \param buffer a MidiBuffer containing only the CC values you wish to analyze
        void push(const MidiBuffer &buffer) {
            ringBuffer[writeHead] = calculateRmsOfSingleBuffer(buffer);
            writeHead = writeHead == size-1 ? 0 : writeHead + 1;
        }

        int getRms() {
            int sum = 0;
            for(int i = 0; i < size; i++){
                sum += ringBuffer[i];
            }
            return sum/size;
        }

    private:
        int calculateRmsOfSingleBuffer(const MidiBuffer &buffer) {
            if (buffer.isEmpty()) return 0;

            int sum = 0;

            for (auto metadata: buffer) {
                const auto message = metadata.getMessage();
                sum += message.getControllerValue();
            }
            return sum / buffer.getNumEvents();
        }

        std::vector<int> ringBuffer;

        int writeHead = 0;
        int size = 5;
    };
}