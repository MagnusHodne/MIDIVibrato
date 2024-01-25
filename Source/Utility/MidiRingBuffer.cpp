#include "MidiRingBuffer.h"

namespace Utility
{
    MidiRingBuffer::MidiRingBuffer(const float numSecondsToHold, const double sampleRate, const int blockSize)
        : data((int)(sampleRate * numSecondsToHold), 0),
          crossingPositions(0),
          spb(blockSize),
          sr(sampleRate)
    {
        reset(sampleRate, blockSize);
    }

    void MidiRingBuffer::push(const juce::MidiBuffer& buffer)
    {
        amplitude.skip(spb);
        frequency.skip(spb);

        int currentSamplePos = 0;

        for (auto metadata : buffer)
        {
            const auto time = metadata.samplePosition;

            //First we write whatever the previous value was, up until the value should actually change
            write(time - currentSamplePos);
            const auto currentValue = value;
            //Map our values so that we deal with 0 as our center value (lets us process MIDI similarly to how we process audio)
            value = juce::jmap(metadata.getMessage().getControllerValue(), 0, 127, -63, 64);

            if (currentValue >= 0 && value < 0 || currentValue <= 0 && value > 0)
            {
                crossingPositions.emplace_back(writeHead);
            }
            currentSamplePos = time;
        }
        //Make sure we write the remaining values in the buffer
        if (currentSamplePos < spb)
        {
            write(spb - currentSamplePos);
        }

        auto rms = std::sqrt(rmsSum / static_cast<float>(data.size()));
        amplitude.setTargetValue(rms);

        const float numSecondsInBuffer = (float)data.size() / (float)sr;
        const float numCycles = (float)crossingPositions.size() / 2;
        const float freq = numCycles / numSecondsInBuffer;
        frequency.setTargetValue(freq);
    }

    float MidiRingBuffer::getRawFrequency()
    {
        return frequency.getCurrentValue();
    }

    int MidiRingBuffer::getFrequency(float minFrequency, float maxFrequency)
    {
        const auto clamped = std::clamp(getRawFrequency(), minFrequency, maxFrequency);
        return static_cast<int>(juce::jmap(clamped, minFrequency, maxFrequency, 0.f, 127.f));
    }

    float MidiRingBuffer::getRawRms()
    {
        return amplitude.getCurrentValue();
    }

    int MidiRingBuffer::getRms()
    {
        //A sine wave should have an RMS of 0.707 times the max value (which in our case is 64 * 0.707);
        const auto clamped = std::clamp(getRawRms(), 0.f, 64.f * 0.707f);
        const auto mapped = juce::jmap(clamped, 0.f, 64.f * 0.707f, 0.f, 127.f);
        return static_cast<int>(std::clamp(mapped, 0.f, 127.f));
    }

    void MidiRingBuffer::setSmoothingRampLength(double newLength)
    {
        amplitude.reset(sr, newLength);
        frequency.reset(sr, newLength);
    }

    void MidiRingBuffer::reset(double sampleRate, int blockSize)
    {
        sr = sampleRate;
        spb = blockSize;
        amplitude.reset(sampleRate, ampAttack);
        amplitude.setCurrentAndTargetValue(0.f);
        frequency.reset(sampleRate, freqAttack);
        frequency.setCurrentAndTargetValue(0.f);
    }

    void MidiRingBuffer::setSecondsToHold(float newTime)
    {
        data.assign((int)(sr * newTime), 0);
    }

    void MidiRingBuffer::setFrequencyAttack(float newValue)
    {
        freqAttack = newValue;
        frequency.reset(sr, newValue);
    }

    void MidiRingBuffer::setFrequencyRelease(float newValue)
    {
        juce::ignoreUnused(newValue);
    }

    void MidiRingBuffer::setRmsAttack(float newValue)
    {
        ampAttack = newValue;
        amplitude.reset(sr, newValue);
    }

    void MidiRingBuffer::setRmsRelease(float newValue)
    {
        juce::ignoreUnused(newValue);
    }

    void MidiRingBuffer::write(const int numSamplesToWrite)
    {
        if (numSamplesToWrite == 0) return;
        for (int i = 0; i < numSamplesToWrite; i++)
        {
            {
                //Calculate RMS
                const auto oldestValue = data[writeHead];
                rmsSum -= std::powf(static_cast<float>(oldestValue), 2.0);
                rmsSum += std::powf(static_cast<float>(value), 2.0);
            }

            data[writeHead] = value;
            moveWritePos(1);
            if (!crossingPositions.empty() && crossingPositions.front() == writeHead)
            {
                crossingPositions.pop_front();
            }
        }
    }

    void MidiRingBuffer::moveWritePos(int increment)
    {
        //If we aren't out of bounds when incrementing by the desired amount, simply increment
        if (writeHead + increment < data.size()) writeHead += increment;
        //Wrap around
        else
        {
            const auto delta = (int)data.size() - writeHead;
            writeHead = 0 + increment - delta;
        }
    }
}
