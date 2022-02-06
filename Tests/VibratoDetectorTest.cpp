#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "../Source/VibratoDetector.h"

TEST_CASE("TestOneHz", "[processors]")
{
    VibratoDetector detector = VibratoDetector(190);
    int samplesPerBlock = 256;
    double sampleRate = 48000;

    detector.setMetadata(sampleRate, samplesPerBlock);
    auto numBlocksPerSecond = sampleRate/samplesPerBlock;
    auto inputController = 2;
    detector.setInputController(inputController);


    juce::MidiBuffer buffer;
    for(int i = 0; i < 189; i++){
        detector.processMidi(buffer, samplesPerBlock);
    }
    juce::MidiBuffer buffer1;
    buffer1.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 127), 128);
    buffer1.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 0), 1);
    detector.processMidi(buffer1, samplesPerBlock);

    REQUIRE(detector.getTargetRate() == Catch::Approx(1.0).margin(0.02));
}