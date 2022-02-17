#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("Test sine wave") {
    double sampleRate = 48000;
    int blockSize = 256;
    float frequency = GENERATE(2.f, 2.5f, 3.f, 3.5f);

    //This is the min window size if we want to detect frequencies down to 1Hz
    auto minimumWindowSize = static_cast<int>(sampleRate/1.f);
    int controllerType = 1;

    Utility::MidiSineGenerator sineGenerator(controllerType, frequency, sampleRate, blockSize);
    Utility::MidiRingBuffer ringBuffer(blockSize, minimumWindowSize*8, sampleRate);

    juce::MidiBuffer midiBuffer;
    ringBuffer.setSmoothingRampLength(1.5);

    //We need at least 0.5 seconds of data to get our smoothed values in roughly the right place
    float numSecondsToGenerate = GENERATE(1.5f, 2.f);
    auto numSamplesToGenerate = 48000 * numSecondsToGenerate;

    auto numBuffersToGenerate = static_cast<int>((int)numSamplesToGenerate/blockSize);
    for(int i = 0; i < numBuffersToGenerate; i++){
        sineGenerator.fillBuffer(midiBuffer);
        ringBuffer.push(midiBuffer);
        midiBuffer.clear();
    }

    SECTION("Test average and RMS") {
        CHECK(ringBuffer.getRawRms() == Catch::Approx(35).margin(4));
    }

    SECTION("Test rate"){
        CHECK(ringBuffer.getRawFrequency() == Catch::Approx(frequency).margin(0.2));
    }

}