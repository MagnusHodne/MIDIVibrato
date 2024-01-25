#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

namespace Gui {
    class MidiMeter final : public juce::Component, juce::Timer {
    public:
        explicit MidiMeter(std::function<int()> &&valueFunction);

        void paint(juce::Graphics &g) override;

        void timerCallback() override;

    private:
        std::function<int()> valueSupplier; //Method that gets us our value to display
    };

}
