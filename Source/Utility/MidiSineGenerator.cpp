#include "MidiSineGenerator.h"
#include "juce_core/juce_core.h"

namespace Utility
{
    MidiSineGenerator::MidiSineGenerator(int inputController, float frequency, double sampleRate, int samplesPerBlock)
        : freq(
              frequency), currentSampleRate(sampleRate), spb(samplesPerBlock), controller(inputController)
    {
        updateValues(sampleRate, samplesPerBlock); //Make sure we update the value at least once
    }

    void MidiSineGenerator::updateValues(double sampleRate, int samplesPerBlock)
    {
        currentSampleRate = sampleRate;
        spb = samplesPerBlock;

        auto cyclesPerSample = freq / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    }

    void MidiSineGenerator::fillBuffer(juce::MidiBuffer& bufferToFill)
    {
        int currentSample;
        int prevSample = -1;

        for (auto sample = 0; sample < spb; ++sample)
        {
            auto sine = std::sin(currentAngle); //Generates values from -1 to 1
            currentSample = static_cast<int>(juce::jmap(sine, -1., 1., 0., 127.));
            currentAngle += angleDelta;
            if (currentSample != prevSample)
            {
                bufferToFill.addEvent(juce::MidiMessage::controllerEvent(1, controller, currentSample), sample);
            }
            prevSample = currentSample;
        }
    }
}
