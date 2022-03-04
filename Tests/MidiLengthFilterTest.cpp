#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiLengthFilter.h"

TEST_CASE("Test ring buffer") {
    double sampleRate = 48000;
    int blockSize = 4;
    int samplesToHold = 10;

    Utility::MidiLengthFilter ringBuffer(sampleRate, blockSize, samplesToHold);
    juce::MidiBuffer midiBuffer;
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 1), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 2), 2);

    ringBuffer.push(midiBuffer);
    CHECK(ringBuffer.pop().getNumEvents() == 0);

    ringBuffer.push(midiBuffer);
    ringBuffer.push(midiBuffer);
    auto res = ringBuffer.pop();
    CHECK(res.getNumEvents() == 1);
    CHECK(res.getFirstEventTime() == 3);

    ringBuffer.push(midiBuffer);
    res = ringBuffer.pop();
    CHECK(res.getNumEvents() == 1);
    CHECK(res.getFirstEventTime() == 3);

    ringBuffer.push(midiBuffer);
    res = ringBuffer.pop();
    CHECK(ringBuffer.pop().getNumEvents() == 2);
    CHECK(res.getFirstEventTime() == 1);
    CHECK(res.getFirstEventTime() == 2);
}