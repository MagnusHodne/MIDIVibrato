
// Custom IDs for simplifying ValueTree identifiers. Rule of thumb: capitalized names are ValueTree "types", others are properties

#pragma once

namespace IDs
{
#define DECLARE_ID(name) const juce::Identifier name (#name);

    DECLARE_ID(SETTINGS)
    DECLARE_ID(SETTING)

    DECLARE_ID(RESOURCE_PATHS)
    DECLARE_ID(MIDI_SETTINGS)

    DECLARE_ID(type)
    DECLARE_ID(path)
    DECLARE_ID(PATH)

    DECLARE_ID(PRESET)

    DECLARE_ID(DESTINATIONS)
    DECLARE_ID(version)
    DECLARE_ID(DESTINATION)
    DECLARE_ID(port)
    DECLARE_ID(voice)
    DECLARE_ID(channel)

    DECLARE_ID(PROPERTIES)

    DECLARE_ID(INPUT_MODIFIERS)
    DECLARE_ID(MODIFIER)

    DECLARE_ID(name)


    DECLARE_ID(ColourTheme)
    DECLARE_ID(COLOUR)
    DECLARE_ID(id)
    DECLARE_ID(colour)
#undef DECLARE_ID
}