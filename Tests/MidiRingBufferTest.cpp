#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("Test sum and average") {
    int numSamplesToHold = GENERATE(16, 32, 64);
    int samplesPerBuffer = 16;

    Utility::MidiRingBuffer ringBuffer(16, numSamplesToHold, 48000);
    juce::MidiBuffer midiBuffer;

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 0);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), samplesPerBuffer/2);
    ringBuffer.push(midiBuffer);
    auto expectedSum = (0 * 8 + 127 * 8);
    CHECK(ringBuffer.getSum() == expectedSum);
    CHECK(ringBuffer.getAverage() == (float) expectedSum / (float) numSamplesToHold);
}

TEST_CASE("Test sine wave") {
    double sampleRate = 48000;
    int blockSize = 256;
    float frequency = 4.f;

    //Effectively the number of samples in a period of the given freq
    auto minimumWindowSize = static_cast<int>(sampleRate/frequency);
    int controllerType = 1;

    Utility::MidiSineGenerator sineGenerator(controllerType, frequency, sampleRate, blockSize);
    Utility::MidiRingBuffer ringBuffer(blockSize, minimumWindowSize, sampleRate);

    juce::MidiBuffer midiBuffer;

    auto numBuffersToGenerate = minimumWindowSize / blockSize + 5;
    for(int i = 0; i < numBuffersToGenerate; i++){
        sineGenerator.fillBuffer(midiBuffer);
        ringBuffer.push(midiBuffer);
        midiBuffer.clear();
    }

    SECTION("Test average and RMS") {
        CHECK(ringBuffer.getAverage() == Catch::Approx(63).margin(0));
    }

    SECTION("Test rate"){
        CHECK(ringBuffer.getFrequency() == Catch::Approx(frequency).margin(0.2));
    }

}