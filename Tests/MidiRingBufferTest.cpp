#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"

TEST_CASE("Invalid range should throw error") {
    REQUIRE_THROWS(Utility::MidiRingBuffer(256, 4));
}

TEST_CASE("Test ring buffer") {

    juce::MidiBuffer midiBuffer;
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 1), 1);
    midiBuffer.addEvent(juce::MidiMessage::controllerEvent(1, 1, 2), 2);

    SECTION("With no delay") {
        Utility::MidiRingBuffer ringBuffer(4, 0);
        ringBuffer.push(midiBuffer);
        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 1);
        CHECK(res.getLastEventTime() == 2);
    }

    SECTION("With 1 sample delay") {
        Utility::MidiRingBuffer ringBuffer(4, 1);
        ringBuffer.push(midiBuffer);
        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 2);
        CHECK(res.getLastEventTime() == 3);
    }

    SECTION("With 8 samples delay") {
        Utility::MidiRingBuffer ringBuffer(4, 8);
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

    SECTION("With 5 milliseconds delay") {
        Utility::MidiRingBuffer ringBuffer(64, 48000, 5);

        // At 48KHz, one millisecond = 48000/1000 = 48 samples;
        // 5 ms = 240 samples = 240/64 = ~4 buffers of data to
        ringBuffer.push(midiBuffer);
        ringBuffer.push(juce::MidiBuffer());
        ringBuffer.push(juce::MidiBuffer());
        ringBuffer.push(juce::MidiBuffer());

        auto res = ringBuffer.peek();
        CHECK(res.getNumEvents() == 2);
        CHECK(res.getFirstEventTime() == 1 + 48);
        CHECK(res.getLastEventTime() == 2 + 48);
    }
}