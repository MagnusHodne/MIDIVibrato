#include "VibratoDetector.h"
#include "Utility/MidiUtils.h"
#include "juce_core/juce_core.h"

VibratoDetector::VibratoDetector(int initialBufferSize)
    : ringBuffer((float)initialBufferSize, sr, spb)
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

void VibratoDetector::setInputController(const int newCC)
{
    inputController = Utility::clamp7Bit(newCC);
}

void VibratoDetector::setRmsController(const int newCC)
{
    ampController = Utility::clamp7Bit(newCC);
}

void VibratoDetector::setFrequencyController(const int newCC)
{
    rateController = Utility::clamp7Bit(newCC);
}

void VibratoDetector::resetValues(const double sampleRate, const int blockSize)
{
    sr = sampleRate;
    spb = blockSize;
    ringBuffer.reset(sr, spb);
}

void VibratoDetector::setNumSecondsToHold(const float numSeconds)
{
    ringBuffer.setSecondsToHold(numSeconds);
}

void VibratoDetector::setMinMaxRate(const float newMinRate, const float newMaxRate)
{
    minRate = newMinRate;
    maxRate = newMaxRate;
}

void VibratoDetector::setFrequencyAttack(const float newValue)
{
    ringBuffer.setFrequencyAttack(newValue);
}

void VibratoDetector::setFrequencyRelease(const float newValue)
{
    ringBuffer.setFrequencyRelease(newValue);
}

void VibratoDetector::setRmsAttack(const float newValue)
{
    ringBuffer.setRmsAttack(newValue);
}

void VibratoDetector::setRmsRelease(const float newValue)
{
    ringBuffer.setRmsRelease(newValue);
}
