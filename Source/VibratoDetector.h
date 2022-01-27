#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"

class VibratoDetector
{
public:
    void processMidi(juce::MidiBuffer& midiMessages, int numSamples) {
        juce::MidiBuffer passthrough;
        juce::MidiBuffer vibratoData;
        double sum = 0.0;

        for (const auto metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            const auto time = metadata.samplePosition;

            if(message.isControllerOfType(inputController)){
                vibratoData.addEvent(message, time);
            } else {
                passthrough.addEvent(message, time);
            }
        }

        if(!vibratoData.isEmpty()){
            auto numEvents = vibratoData.getNumEvents();
            for(const auto metadata : vibratoData){
                auto message = metadata.getMessage();
                sum += message.getControllerValue() * message.getControllerValue();
            }
            amplitude = static_cast<int>(std::sqrt(sum/numEvents));
        }
        midiMessages.swapWith (passthrough);
    }

    int getAmplitude() const {
        return std::clamp(amplitude, 0, 127);
    }
    int getRate() const {
        return std::clamp(rate, 0, 127);
    }

    void setInputController(int newCC){
        inputController = clampCCs(newCC);
    }
    void setAmpController(int newCC){
        ampController = clampCCs(newCC);
    }
    void setRateController(int newCC){
        rateController = clampCCs(newCC);
    }

    int clampCCs(int newCC) { return std::clamp(newCC, 1, 127); }

    void resetValues(double sampleRate) {
        //amplitude.reset(sampleRate, rampLengthInSeconds);
        //amplitude.setCurrentAndTargetValue(0.f);
    }

private:

    int inputController = 1;
    int ampController = 21;
    int rateController = 19;

    double rampLengthInSeconds = 0.001f;

    int amplitude = 0;
    int rate = 0;
};