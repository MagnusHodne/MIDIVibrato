#include "VibratoDetector.h"
#include "juce_core/juce_core.h"

VibratoDetector::VibratoDetector(int initialBufferSize)
    : sr(48000), spb(256), ringBuffer((float)initialBufferSize, sr, spb)
{
}

void VibratoDetector::processMidi(juce::MidiBuffer& midiMessages, int numSamples)
{
    juce::ignoreUnused(numSamples);
    juce::MidiBuffer passthrough;
    juce::MidiBuffer vibratoData;

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();
        const auto time = metadata.samplePosition;

        if (message.isControllerOfType(inputController))
        {
            vibratoData.addEvent(message, time);
        }
        else
        {
            passthrough.addEvent(message, time);
        }
    }

    ringBuffer.push(vibratoData);

    passthrough.addEvent(
        juce::MidiMessage::controllerEvent(1, ampController, getRms()),
        1);

    passthrough.addEvent(
        juce::MidiMessage::controllerEvent(1, rateController, getFrequency()),
        2);

    midiMessages.swapWith(passthrough);
}

int VibratoDetector::getRms()
{
    return ringBuffer.getRms();
}

int VibratoDetector::getFrequency()
{
    return ringBuffer.getFrequency(minRate, maxRate);
}

void VibratoDetector::setInputController(int newCC)
{
    inputController = clampCCs(newCC);
}

void VibratoDetector::setRmsController(int newCC)
{
    ampController = clampCCs(newCC);
}

void VibratoDetector::setFrequencyController(int newCC)
{
    rateController = clampCCs(newCC);
}

int VibratoDetector::clampCCs(int newCC)
{
    return std::clamp(newCC, 1, 127);
}

void VibratoDetector::resetValues(double sampleRate, int blockSize)
{
    sr = sampleRate;
    spb = blockSize;
    ringBuffer.reset(sr, spb);
}

void VibratoDetector::setNumSecondsToHold(float numSeconds)
{
    ringBuffer.setSecondsToHold(numSeconds);
}

void VibratoDetector::setMinMaxRate(float newMinRate, float newMaxRate)
{
    minRate = newMinRate;
    maxRate = newMaxRate;
}

void VibratoDetector::setFrequencyAttack(float newValue)
{
    ringBuffer.setFrequencyAttack(newValue);
}

void VibratoDetector::setFrequencyRelease(float newValue)
{
    ringBuffer.setFrequencyRelease(newValue);
}

void VibratoDetector::setRmsAttack(float newValue)
{
    ringBuffer.setRmsAttack(newValue);
}

void VibratoDetector::setRmsRelease(float newValue)
{
    ringBuffer.setRmsRelease(newValue);
}
