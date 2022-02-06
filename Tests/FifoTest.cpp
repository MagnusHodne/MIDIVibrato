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
    juce::MidiBuffer noData, oneCrossing, twoCrossing, threeCrossing;

    REQUIRE(buffer.getRate() == 0);
    buffer.calculateValues(noData);
    REQUIRE(buffer.getRate() == 0);

    oneCrossing.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 1);
    oneCrossing.addEvent(juce::MidiMessage::controllerEvent(1, 1, 127), 2);
    threeCrossing = twoCrossing = oneCrossing;
    twoCrossing.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 3);
    threeCrossing.addEvent(juce::MidiMessage::controllerEvent(1, 1, 0), 3);
    threeCrossing.addEvent(juce::MidiMessage::controllerEvent(1,1,127), 4);

    REQUIRE(oneCrossing.getNumEvents() == 2);
    REQUIRE(twoCrossing.getNumEvents() == 3);
    REQUIRE(threeCrossing.getNumEvents() == 4);
    //============================================
    buffer.calculateValues(oneCrossing);
    REQUIRE(buffer.getRate() == 1.f);
    buffer.calculateValues(twoCrossing);
    REQUIRE(buffer.getRate() == 2.f);
    buffer.calculateValues(threeCrossing);
    REQUIRE(buffer.getRate() == 3.f);

    //============================================
    buffer.reset(4);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);

    REQUIRE(buffer.getRate() == 0.5f);

    //============================================
    buffer.reset(4);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);

    REQUIRE(buffer.getRate() == 0.75f);
}