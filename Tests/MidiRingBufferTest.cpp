#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"

TEST_CASE("Test parsing of single buffer"){
    Utility::MidiRingBuffer ringBuffer(256, 256*3);
    juce::MidiBuffer midiBuffer;

    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 16), 4);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 18), 7);
    ringBuffer.push(midiBuffer);
    midiBuffer.clear();
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 12), 10);
    ringBuffer.push(midiBuffer);
}