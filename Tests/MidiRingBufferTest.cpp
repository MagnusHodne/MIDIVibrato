#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"

TEST_CASE("Test sum and average") {
    int numSamplesToHold = GENERATE(16, 32, 64);
    int samplesPerBuffer = 16;

    Utility::MidiRingBuffer ringBuffer(16, numSamplesToHold);
    juce::MidiBuffer midiBuffer;

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 0);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), samplesPerBuffer/2);
    ringBuffer.push(midiBuffer);
    auto expectedSum = (0 * 8 + 127 * 8);
    REQUIRE(ringBuffer.getSum() == expectedSum);
    REQUIRE(ringBuffer.getAverage() == (float) expectedSum / (float) numSamplesToHold);
}