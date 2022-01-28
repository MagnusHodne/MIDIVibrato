#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "Utility/Fifo.h"

class VibratoDetector {
public:
    void processMidi(juce::MidiBuffer &midiMessages, int numSamples) {
        amplitude.skip(numSamples);
        juce::MidiBuffer passthrough;
        juce::MidiBuffer vibratoData;

        for (const auto metadata: midiMessages) {
            auto message = metadata.getMessage();
            const auto time = metadata.samplePosition;

            if (message.isControllerOfType(inputController)) {
                vibratoData.addEvent(message, time);
            } else {
                passthrough.addEvent(message, time);
            }
        }

        ringBuffer.push(vibratoData);

        amplitude.setTargetValue(static_cast<float>(ringBuffer.getRms()) * scaling);
        midiMessages.swapWith(passthrough);
    }

    int getAmplitude() const {
        return std::clamp(static_cast<int>(amplitude.getCurrentValue()), 0, 127);
    }

    int getRate() const {
        return std::clamp(static_cast<int>(rate.getCurrentValue()), 0, 127);
    }

    void setInputController(int newCC) {
        inputController = clampCCs(newCC);
    }

    void setAmpController(int newCC) {
        ampController = clampCCs(newCC);
    }

    void setRateController(int newCC) {
        rateController = clampCCs(newCC);
    }

    int clampCCs(int newCC) { return std::clamp(newCC, 1, 127); }

    void resetValues(double sampleRate, int numBuffers, float newScaling) {
        amplitude.reset(sampleRate, rampLengthInSeconds);
        amplitude.setCurrentAndTargetValue(0.f);

        scaling = newScaling;

        ringBuffer.reset(numBuffers);
    }

private:

    int inputController = 1;
    int ampController = 21;
    int rateController = 19;

    Utility::MidiRingBuffer ringBuffer;

    double rampLengthInSeconds = 0.1;
    float scaling = 1.f;

    juce::LinearSmoothedValue<float> amplitude;
    juce::LinearSmoothedValue<float> rate;
};