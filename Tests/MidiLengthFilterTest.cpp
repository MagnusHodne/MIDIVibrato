#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiLengthFilter.h"

TEST_CASE("Invalid range should throw error") {
    REQUIRE_THROWS(Utility::MidiLengthFilter(256, 4));
}

TEST_CASE("Test ring buffer") {

    juce::MidiBuffer midiBuffer;
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 1), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 2), 2);

    SECTION("With block size 4 (no delay)") {
        Utility::MidiLengthFilter ringBuffer(4, 4);
        ringBuffer.push(midiBuffer);
        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 1);
        CHECK(res.getLastEventTime() == 2);
    }

    SECTION("With block size 5 (delay of 1 sample)") {
        Utility::MidiLengthFilter ringBuffer(4, 5);
        ringBuffer.push(midiBuffer);
        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 2);
        CHECK(res.getLastEventTime() == 3);
    }

    SECTION("With block size 12 (delay of 8 samples") {
        Utility::MidiLengthFilter ringBuffer(4, 12);
        ringBuffer.push(midiBuffer);
        CHECK(ringBuffer.peek().getNumEvents() == 0);

        ringBuffer.push(midiBuffer);
        CHECK(ringBuffer.peek().getNumEvents() == 0);

        ringBuffer.push(midiBuffer);
        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 1);
        CHECK(res.getLastEventTime() == 2);
    }
}