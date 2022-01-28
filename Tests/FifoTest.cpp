#include <catch2/catch_test_macros.hpp>
#include "../Source/Utility/Fifo.h"

TEST_CASE("TestRMS", "[dummy]")
{
    Utility::MidiRingBuffer ringBuffer(2);
    juce::MidiBuffer midiBuffer;

    REQUIRE(ringBuffer.getRms() == 0);

    ringBuffer.push(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 0);

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 2);

    ringBuffer.push(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 31);

    ringBuffer.push(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 63);
}