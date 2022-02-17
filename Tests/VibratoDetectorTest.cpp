#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include "../Source/VibratoDetector.h"
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("Test target rate", "[processors]")
{
    int blockSize = GENERATE(256, 512, 1024);
    double sampleRate = GENERATE(44100., 48000.);
    auto frequency = GENERATE(3.f, 4.f, 5.f, 6.f);

    auto inputController = 2;
    auto initialBufferSize = 190;
    VibratoDetector detector = VibratoDetector(initialBufferSize);
    detector.resetValues(sampleRate, blockSize);
    detector.setInputController(inputController);

    Utility::MidiSineGenerator gen(inputController, frequency, sampleRate, blockSize);
    juce::MidiBuffer buffer;
    for(int i = 0; i < initialBufferSize; i++){
        buffer.clear();
        gen.fillBuffer(buffer);
        detector.processMidi(buffer, blockSize);
    }

    REQUIRE(detector.getTargetRate() == Catch::Approx(frequency).margin(0.35));
}

TEST_CASE("Test rate slider value", "[processors]"){
    auto initialBufferSize = 190;
    VibratoDetector detector = VibratoDetector(initialBufferSize);
    auto inputController = 2;
    float minRate = 1.68f, maxRate = 7.33f;

    int samplesPerBlock = GENERATE(256, 512, 1024);
    double sampleRate = GENERATE(44100., 48000.);
    auto frequency = GENERATE(1.5f, 3.f, 4.f, 5.f, 6.f);

    detector.setMinMaxRate(minRate, maxRate);
    detector.setInputController(inputController);
    detector.resetValues(initialBufferSize, 1.0);

    Utility::MidiSineGenerator gen(inputController, frequency, sampleRate, samplesPerBlock);
    juce::MidiBuffer buffer;
    for(int i = 0; i < initialBufferSize; i++){
        buffer.clear();
        gen.fillBuffer(buffer);
        detector.processMidi(buffer, samplesPerBlock);
    }

    auto frequencyAsMidiValue = juce::jmap(std::clamp(frequency, minRate, maxRate), minRate, maxRate, 0.f, 127.f);
    auto midiRateValue = detector.getFrequency();

    REQUIRE(midiRateValue == Catch::Approx(frequencyAsMidiValue).margin(2));
}

TEST_CASE("Test max amplitude", "[processors]")
{
    VibratoDetector detector = VibratoDetector(4);
    int blockSize = 256;
    double sampleRate = 48000;
    auto inputController = 2;
    detector.resetValues(sampleRate, blockSize);
    detector.setInputController(inputController);

    juce::MidiBuffer buffer;
    for(int i = 0; i < 4; i++){
        buffer.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 0), 1);
        buffer.addEvent(juce::MidiMessage::controllerEvent(1, inputController, 127), 128);
        detector.processMidi(buffer, blockSize);
    }

    REQUIRE(detector.getRms() == 127);
}