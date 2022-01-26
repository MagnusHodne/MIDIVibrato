#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"

class VibratoDetector
{
public:
    /// Splits out an incoming MIDI CC message, and splits it out into separate rate/amplitude components.
    /// All processing is done in this class
    /// \param midiMessages The incoming MidiBuffer
    /// \param inputController The controller that is used for interpreting vibrato
    /// \param ampController The controller that will be used to send out amplitude information
    /// \param rateController The controller that will be used to send out rate information
    /// \return
    void processMidi(juce::MidiBuffer& midiMessages, int numSamples) {

        juce::MidiBuffer passthrough;

        int sum = 0;

        for (const auto metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            const auto time = metadata.samplePosition;

            if(message.isControllerOfType(inputController)){
                sum += message.getControllerValue();
            } else {
                passthrough.addEvent(message, time);
            }
        }

        amplitude = sum/numSamples;
        midiMessages.swapWith (passthrough);
    }

    int getAmplitude() const {
        return amplitude;
    }
    int getRate() const {
        return rate;
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

private:

    int inputController = 1;
    int ampController = 21;
    int rateController = 19;

    int amplitude = 0;
    int rate = 0;
};