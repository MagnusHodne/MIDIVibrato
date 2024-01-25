#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "Detector.h"
#include "Utility/MidiRingBuffer.h"

class VibratoDetector : public Detector {
public:
    ///
    /// \param initialBufferSize the number of buffers that is stored internally for calculating averages on
    explicit VibratoDetector(int initialBufferSize);

    ~VibratoDetector() override = default;

    void processMidi(juce::MidiBuffer &midiMessages, int numSamples) override;

    [[nodiscard]] int getRms() override;

    //Should return rate mapped to the correct values...
    [[nodiscard]] int getFrequency() override;

    void setInputController(int newCC) override;

    void setRmsController(int newCC) override;

    void setFrequencyController(int newCC) override;

    void resetValues(double sampleRate, int blockSize) override;

    void setNumSecondsToHold(float numSeconds) override;

    /// Sets the new min and max rates for the vibrato. These values should correspond to the min/max
    /// of whatever instrument you are playing, so that the output rateCC correctly maps from 0-127
    void setMinMaxRate(float newMinRate, float newMaxRate) override;

    void setFrequencyAttack(float newValue) override;

    void setFrequencyRelease(float newValue) override;

    void setRmsAttack(float newValue) override;

    void setRmsRelease(float newValue) override;

private:

    int inputController = 2;
    int ampController = 21;
    int rateController = 20;

    //These are the min-max Hz rates in Aaron Venture...
    float minRate = 1.68f, maxRate = 7.33f;
    double sr = 48000; //Sample rate
    int spb = 256; //Samples per buffer
    Utility::MidiRingBuffer ringBuffer;
};