#include <catch2/catch_test_macros.hpp>
#include "../Source/Utility/MidiSineGenerator.h"

TEST_CASE("Test Min/Max Sine Wave values", "[Utility]"){
    Utility::MidiSineGenerator generator(2, 3.f, 48000, 256);
    juce::MidiBuffer buffer;

    int minValue = 500, maxValue = -1;
    for(int i = 0; i < 2048; i++){
        buffer.clear();
        generator.fillBuffer(buffer);
        for(auto metadata : buffer){
            const auto value = metadata.getMessage().getControllerValue();
            if(value < minValue) minValue = value;
            if(value > maxValue) maxValue = value;
        }
    }

    REQUIRE(minValue == 0);
    REQUIRE(maxValue == 127);

}