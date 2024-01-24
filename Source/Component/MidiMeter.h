#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

namespace Gui {
    class MidiMeter : public juce::Component, juce::Timer {
    public:
        explicit MidiMeter(std::function<int()> &&valueFunction) : valueSupplier(std::move(valueFunction)) {
            startTimerHz(60);
        }

        void paint(juce::Graphics &g) override {
            const auto level = valueSupplier();
            auto bounds = getLocalBounds().toFloat();
            g.setColour(juce::Colours::white.withBrightness(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);

            g.setColour(juce::Colours::white);
            const auto scaledX = juce::jmap(level, 0, 127, 0, getWidth());
            g.fillRoundedRectangle(bounds.removeFromLeft(static_cast<float>(scaledX)), 5.f);
        }

        void timerCallback() override {
            repaint();
        }

    private:
        std::function<int()> valueSupplier; //Method that gets us our value to display
    };

}
