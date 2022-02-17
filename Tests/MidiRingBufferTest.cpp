#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>

#include "../Source/Utility/MidiRingBuffer.h"
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("Test sine wave") {
    double sampleRate = 48000;
    int blockSize = 256;
    float frequency = GENERATE(1.f, 2.f, 2.5f, 3.f, 3.5f);

    //This is the min window size if we want to detect frequencies down to 1Hz
    auto minimumWindowSize = static_cast<int>(sampleRate/1.f);
    int controllerType = 1;

    Utility::MidiSineGenerator sineGenerator(controllerType, frequency, sampleRate, blockSize);
    Utility::MidiRingBuffer ringBuffer(1.0f, sampleRate, blockSize);

    juce::MidiBuffer midiBuffer;
    ringBuffer.setSmoothingRampLength(0.5);

    auto numSecondsToGenerate = GENERATE(2.f, 2.5f, 3.f, 3.25f, 3.33f);
    auto numSamplesToGenerate = 48000 * numSecondsToGenerate;

    auto numBuffersToGenerate = static_cast<int>((int)numSamplesToGenerate/blockSize);
    for(int i = 0; i < numBuffersToGenerate; i++){
        sineGenerator.fillBuffer(midiBuffer);
        ringBuffer.push(midiBuffer);
        midiBuffer.clear();
    }

    SECTION("Test average and RMS") {
        //RMS of a sine wave should be 0.707 times the peak level of the signal
        //Therefore, we expect the RMS of this signal to be 0.707x64
        CHECK(ringBuffer.getRawRms() == Catch::Approx(64.f * 0.707f).margin(3.f));
        CHECK(ringBuffer.getRms() == Catch::Approx(127).margin(6));
    }

    SECTION("Test rate"){
        CHECK(ringBuffer.getRawFrequency() == Catch::Approx(frequency).margin(0.2));
    }

}