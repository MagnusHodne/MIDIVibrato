#include "MidiUtils.h"
#include <algorithm>

namespace Utility
{
    int clamp7Bit(const int value)
    {
        return std::clamp(value, 1, 127);
    }
}
