#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"
#include "Utility/VibratoBuffer.h"

class VibratoDetector {
public:
    ///
    /// \param initialBufferSize the number of buffers that is stored internally for calculating averages on
    explicit VibratoDetector(int initialBufferSize)
            : vibratoBuffer(initialBufferSize) {
    }

    void processMidi(juce::MidiBuffer &midiMessages, int numSamples) {
        amplitude.skip(numSamples);
        rate.skip(numSamples);
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
        amplitude.setTargetValue(static_cast<float>(vibratoBuffer.getRms()) * ampScaling);

        //Should be a Hz value times scaling
        rate.setTargetValue(vibratoBuffer.getRate() * getNumBlocksPerSecond() * rateScaling);

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

    //Should return rate mapped to the correct values...
    [[nodiscard]] int getRate() const {
        //These are the min-max Hz rates in Aaron Venture...
        float minRate = 1.68f;
        float maxRate = 7.33f;
        return std::clamp(static_cast<int>(rate.getCurrentValue()), 0, 127);
    }

    [[nodiscard]] float getTargetRate() const {
        return rate.getTargetValue();
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

    void resetValues(int numBuffers, float newScaling) {
        amplitude.reset(sr, rampLengthInSeconds);
        rate.reset(sr, rampLengthInSeconds);
        amplitude.setCurrentAndTargetValue(0.f);
        rate.setCurrentAndTargetValue(0.f);

        ampScaling = rateScaling = newScaling;

        vibratoBuffer.reset(numBuffers);
    }

    void setAmpScaling(float newScale) {
        ampScaling = newScale;
    }

    void setRateScaling(float newScale){
        rateScaling = newScale;
    }

    void setMetadata(double sampleRate, int samplesPerBlock) {
        sr = sampleRate;
        spb = samplesPerBlock;
    }

private:
    [[nodiscard]] float getNumBlocksPerSecond() const{
        return static_cast<float>(sr)/spb;
    }

    int inputController = 2;
    int ampController = 21;
    int rateController = 20;

    Utility::VibratoBuffer vibratoBuffer;

    double rampLengthInSeconds = 0.5;
    float ampScaling, rateScaling = 1.f;

    double sr;
    int spb;

    juce::LinearSmoothedValue<float> amplitude, rate;
};