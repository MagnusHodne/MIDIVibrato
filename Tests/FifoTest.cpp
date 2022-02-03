#include <catch2/catch_test_macros.hpp>
#include "../Source/Utility/VibratoBuffer.h"

TEST_CASE("TestRMS", "[processors]")
{
    Utility::VibratoBuffer ringBuffer(2);
    juce::MidiBuffer midiBuffer;

    REQUIRE(ringBuffer.getRms() == 0);

    ringBuffer.calculateValues(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 0);

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 2);

    ringBuffer.calculateValues(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 31);

    ringBuffer.calculateValues(midiBuffer);
    REQUIRE(ringBuffer.getRms() == 63);
}

TEST_CASE("TestRate", "[processors]"){
    Utility::VibratoBuffer buffer(1);
    juce::MidiBuffer midiBuffer;

    REQUIRE(buffer.getRate() == 0);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 2);

    buffer.calculateValues(midiBuffer);
    REQUIRE(buffer.getRate() == 1);

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 3);
    buffer.calculateValues(midiBuffer);
    REQUIRE(buffer.getRate() == 2);

    buffer.reset(2);

    REQUIRE(buffer.getRate() == 0);

    buffer.calculateValues(midiBuffer);
    REQUIRE(buffer.getRate() == 2.f/2);

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 4);
    buffer.calculateValues(midiBuffer);
    REQUIRE(buffer.getRate() == (2.f + 3.f)/2.f);


}