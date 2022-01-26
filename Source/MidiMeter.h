#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

using namespace juce;

namespace Gui
{
class MidiMeter : public juce::Component
    {
    public:
        void paint(Graphics& g ) override
        {
            auto bounds = getLocalBounds().toFloat();
            g.setColour(Colours::white.withBrightness(0.4f));
            g.fillRoundedRectangle(bounds, 5.f);

            g.setColour(Colours::white);
            const auto scaledX = jmap(amplitude, 0, 127, 0, getWidth());
            g.fillRoundedRectangle(bounds.removeFromLeft(static_cast<float>(scaledX)), 5.f);
        }
        void setValue(const int value) { amplitude = value; }
    private:
        int amplitude = 0; //Should go 0-127
        int rate = 0;
    };

}
