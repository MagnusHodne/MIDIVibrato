#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <catch2/generators/catch_generators_all.hpp>
#include "../Source/Utility/MidiHeldNoteUtil.h"

TEST_CASE("Test held note util") {
	double sampleRate = 48000;
	int blockSize = 256;
	Utility::MidiHeldNoteUtil midiHeldNoteUtil;
}