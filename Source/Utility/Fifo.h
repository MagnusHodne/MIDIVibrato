#pragma once

#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"


//Code from https://github.com/Thrifleganger/level-meter-demo/blob/master/Source/Utility/Fifo.h
namespace Utility {
    using namespace juce;

    class MidiBufferFifo {
    public:
        int getMidiRms() {
            jassertfalse; //You haven't implemented this yet, smartass
            return 0;
        }

        void reset(int numBuffersToHold) {
            buffers.clear();
            buffers.reserve(numBuffersToHold);


        }

    private:
        std::vector<MidiBuffer> buffers;

        /*
         * prepareToPlay() should set numBuffersToHold and clear any prev. buffers
         *
         */
    };
}