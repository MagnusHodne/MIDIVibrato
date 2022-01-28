#include <catch2/catch_test_macros.hpp>
#include "juce_core/juce_core.h"
#include "../Source/Utility/Fifo.h"

TEST_CASE("TestRMS", "[dummy]")
{
    Utility::MidiRingBuffer ringBuffer;
    ringBuffer.reset(2);
    juce::MidiBuffer mbuff;

    REQUIRE(ringBuffer.getRms() == 0);

    ringBuffer.push(mbuff);
    REQUIRE(ringBuffer.getRms() == 0);

    mbuff.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 1);
    mbuff.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 1);

    ringBuffer.push(mbuff);
    REQUIRE(ringBuffer.getRms() == 31);

    ringBuffer.push(mbuff);
    REQUIRE(ringBuffer.getRms() == 63);
}