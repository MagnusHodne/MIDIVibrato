#pragma once

class Detector {
public:
    virtual ~Detector() = default;
    virtual int getFrequency() const = 0;
    virtual int getRms() const = 0;

    virtual void setAmpScaling(float newScale) = 0;
    virtual void resetValues(double sampleRate, int blockSize) = 0;
    virtual void setInputController(int newController) = 0;
    virtual void setRmsController(int newController) = 0;
    virtual void setFrequencyController(int newController) = 0;
    virtual void processMidi(juce::MidiBuffer &midiMessages, int blockSize) = 0;
    virtual void setInternalBufferSize(int newSize) = 0;
    virtual void setMinMaxRate(float newMinRate, float newMaxRate) = 0;
};