#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include "../Source/VibratoDetector.h"
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("TestOneHz", "[processors]")
{
    auto initialBufferSize = 190;
    VibratoDetector detector = VibratoDetector(initialBufferSize);
    int samplesPerBlock = 256;
    double sampleRate = 48000;

    detector.setMetadata(sampleRate, samplesPerBlock);
    auto numBlocksPerSecond = sampleRate/samplesPerBlock;
    auto inputController = 2;
    detector.setInputController(inputController);
    auto frequency = 1.f;

    MidiSineGenerator gen(inputController, frequency, sampleRate, samplesPerBlock);
    juce::MidiBuffer buffer;
    for(int i = 0; i < initialBufferSize; i++){
        buffer.clear();
        gen.fillBuffer(buffer);
        detector.processMidi(buffer, samplesPerBlock);
    }

    REQUIRE(detector.getTargetRate() == Catch::Approx(frequency).margin(0.02));
}

TEST_CASE("Test max amplitude", "[processors]")
{
    VibratoDetector detector = VibratoDetector(4);
    int samplesPerBlock = 256;
    double sampleRate = 48000;
    detector.setMetadata(sampleRate, samplesPerBlock);
    auto inputController = 2;
    detector.setInputController(inputController);

    juce::MidiBuffer buffer;
    for(int i = 0; i < 4; i++){
        buffer.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 0), 1);
        buffer.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 127), 128);
        detector.processMidi(buffer, samplesPerBlock);
    }

    REQUIRE(detector.getAmplitude() == 127);
}