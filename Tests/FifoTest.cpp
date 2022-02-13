#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "../Source/Utility/VibratoBuffer.h"
#include "../Source/Utility/MidiSineGenerator.h"

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

TEST_CASE("Test Rate", "[processors]"){
    Utility::VibratoBuffer ringBuffer(2000);
    int inputController = 2;
    float frequency = 4.f;
    double sampleRate = 48000;
    int samplesPerBlock = 256;

    Utility::MidiSineGenerator sineGenerator(inputController, frequency, sampleRate, samplesPerBlock);

    juce::MidiBuffer buffer;
    for(int i = 0; i < 190; i++){
        buffer.clear();
        sineGenerator.fillBuffer(buffer);
        ringBuffer.calculateValues(buffer);
    }

    REQUIRE(ringBuffer.getRate(sampleRate, samplesPerBlock) == Catch::Approx(frequency).margin(0.5));
    //REQUIRE(ringBuffer.getAvgNumCrossings(sampleRate, samplesPerBlock) == Catch::Approx(frequency).margin(0.02));
}