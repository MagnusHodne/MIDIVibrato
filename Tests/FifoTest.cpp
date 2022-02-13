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

TEST_CASE("TestRatePrimitive", "[processors]"){
    Utility::VibratoBuffer buffer(1);
    juce::MidiBuffer noData, oneCrossing, twoCrossing, threeCrossing;

    REQUIRE(buffer.getAvgNumCrossings() == 0);
    buffer.calculateValues(noData);
    REQUIRE(buffer.getAvgNumCrossings() == 0);

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
    REQUIRE(buffer.getAvgNumCrossings() == 1.f);
    buffer.calculateValues(twoCrossing);
    REQUIRE(buffer.getAvgNumCrossings() == 2.f);
    buffer.calculateValues(threeCrossing);
    REQUIRE(buffer.getAvgNumCrossings() == 3.f);

    //============================================
    buffer.reset(4);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);

    REQUIRE(buffer.getAvgNumCrossings() == 0.5f);

    //============================================
    buffer.reset(4);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(oneCrossing);
    buffer.calculateValues(noData);

    REQUIRE(buffer.getAvgNumCrossings() == 0.75f);
}

TEST_CASE("Test Rate", "[processors]"){
    Utility::VibratoBuffer ringBuffer(2000);
    int inputController = 2;
    float frequency = 4.f;
    double sampleRate = 48000;
    int samplesPerBlock = 256;

    //sampleRate/samplesPerBlock num blocks per second (187,5 in the case of 48000/256)
    //Expect avg.num crossings times 187,5 to be roughly 1...


    Utility::MidiSineGenerator sineGenerator(inputController, frequency, sampleRate, samplesPerBlock);

    juce::MidiBuffer buffer;
    for(int i = 0; i < 190; i++){
        buffer.clear();
        sineGenerator.fillBuffer(buffer);
        ringBuffer.calculateValues(buffer);
    }

    REQUIRE((ringBuffer.getAvgNumCrossings()/2)*(sampleRate/samplesPerBlock) == Catch::Approx(frequency).margin(0.5));
    //REQUIRE(ringBuffer.getAvgNumCrossings(sampleRate, samplesPerBlock) == Catch::Approx(frequency).margin(0.02));
}