#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

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
    void processMidi(juce::MidiBuffer& midiMessages, int inputController, int ampController, int rateController) {

        juce::MidiBuffer processedMidi;

        for (const auto metadata : midiMessages)
        {
            auto message = metadata.getMessage();
            const auto time = metadata.samplePosition;

            if (message.isControllerOfType(inputController)){
                //Interpret the message here
            } else {
                processedMidi.addEvent (message, time); //Pass through rest
            }
        }

        midiMessages.swapWith (processedMidi);
    }

    int getAmplitude() const {
        return amplitude;
    }
    int getRate() const {
        return rate;
    }
private:
    int amplitude = 0;
    int rate = 0;

};