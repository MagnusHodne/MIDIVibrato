#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "Utility/VibratoBuffer.h"

class VibratoDetector {
public:
    explicit VibratoDetector(int initialBufferSize)
            : vibratoBuffer(initialBufferSize) {
    }

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

        vibratoBuffer.calculateValues(vibratoData);
        amplitude.setTargetValue(static_cast<float>(vibratoBuffer.getRms()) * scaling);
        rate.setTargetValue(vibratoBuffer.getRate()); //TODO - Scale this based upon samplesPerBlock and sampleRate...

        passthrough.addEvent(
                juce::MidiMessage::controllerEvent(1, ampController, getAmplitude()),
                1);

        passthrough.addEvent(
                juce::MidiMessage::controllerEvent(1, rateController, getRate()),
                2);

        midiMessages.swapWith(passthrough);
    }

    [[nodiscard]] int getAmplitude() const {
        return std::clamp(static_cast<int>(amplitude.getCurrentValue()), 0, 127);
    }

    [[nodiscard]] int getRate() const {
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

    static int clampCCs(int newCC) { return std::clamp(newCC, 1, 127); }

    void resetValues(double sampleRate, int numBuffers, float newScaling) {
        amplitude.reset(sampleRate, rampLengthInSeconds);
        amplitude.setCurrentAndTargetValue(0.f);

        scaling = newScaling;

        vibratoBuffer.reset(numBuffers);
    }

    void setScaling(float newScale) {
        scaling = newScale;
    }

private:

    int inputController = 2;
    int ampController = 21;
    int rateController = 19;

    Utility::VibratoBuffer vibratoBuffer;

    double rampLengthInSeconds = 0.5;
    float scaling = 1.f;

    juce::LinearSmoothedValue<float> amplitude;
    juce::LinearSmoothedValue<float> rate;
    float prevAmplitude, prevRate = 0.f;
};