#pragma once

class Detector {
public:
    virtual ~Detector() = default;
    virtual int getFrequency() = 0;
    virtual int getRms() = 0;
    virtual void resetValues(double sampleRate, int blockSize) = 0;
    virtual void setInputController(int newController) = 0;
    virtual void setRmsController(int newController) = 0;
    virtual void setFrequencyController(int newController) = 0;
    virtual void processMidi(juce::MidiBuffer &midiMessages, int blockSize) = 0;
    virtual void setNumSecondsToHold(float numSeconds) = 0;
    virtual void setMinMaxRate(float newMinRate, float newMaxRate) = 0;
    virtual void setFrequencyAttack(float newValue) = 0;
    virtual void setFrequencyRelease(float newValue) = 0;
    virtual void setRmsAttack(float newValue) = 0;
    virtual void setRmsRelease(float newValue) = 0;
};