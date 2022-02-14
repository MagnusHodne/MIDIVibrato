#pragma once
#include <tuple>
#include <vector>
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiRingBuffer {
    public:
        explicit MidiRingBuffer(int samplesPerBuffer, int numSamplesToHold) : data(numSamplesToHold), numSamples(samplesPerBuffer) {}

        void push(juce::MidiBuffer& buffer){
            int prevTime = 0;
            for(auto metadata : buffer){
                auto time = metadata.samplePosition;
                value = metadata.getMessage().getControllerValue();
                write(time - prevTime);
                prevTime = time;
            }
            //Make sure we write the remaining values in the buffer
            if(prevTime < numSamples){
                write(numSamples - prevTime);
            }
        }
    private:
        void write(int numSamplesToWrite){
            for(int i = 0; i < numSamplesToWrite; i++){
                data[writeHead] = value;
                moveWritePos(1);
            }
        }

        void iterateOver(){
            auto startPos = writeHead;
            for(int i = 0; i < data.size(); i++){
                moveWritePos(1);
            }
            jassert(startPos == writeHead); //If this is false, it means we haven't cycled through the buffer correctly!
        }

        //Method for arbitrarily moving the writeHead any number of positions
        void moveWritePos(int increment){
            //If we aren't out of bounds when incrementing by the desired amount, simply increment
            if(writeHead + increment < data.size()) writeHead += increment;
            //Wrap around
            else {
                auto delta = (int)data.size() - writeHead;
                writeHead = 0 + increment - delta;
            }
        }

        std::vector<juce::uint16> data; //Holds MIDI msg value
        int writeHead = 0;
        juce::uint16 value = -1;
        int numSamples;
    };
}