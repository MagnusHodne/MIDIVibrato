#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiLengthFilter {
    public:
        MidiLengthFilter(double sampleRate, int blockSize, int samplesToHold) :
                spb(blockSize), sr(sampleRate), samplesToHold(samplesToHold), readHead(blockSize) {
        }

        void push(juce::MidiBuffer &buffer) {
            int startPos = writeHead;
            int samplePos = 0; //Holds the last written data position
            for (auto metadata: buffer) {
                moveWriteHead(metadata.samplePosition - samplePos);
                samplePos = metadata.samplePosition;
                metadata.samplePosition = writeHead;
                data.addSorted(comparator, metadata);
            }
            if (samplePos < spb) {
                //make sure the writeHead has moved to the "end" of the given buffer
                moveWriteHead(spb - samplePos);
            }
            removeElementsInRange(startPos, writeHead);
        }

        /// Returns a MidiBuffer with the "oldest" data in the buffer. The block size
        /// of this buffer is the same as the buffer supplied in the push method
        juce::MidiBuffer pop() {
            return readBuffer;
        }

    private:
        // Moves the read head and overwrites any old data
        void moveWriteHead(int amount) {
            /// If incrementing the writeHead doesn't go out of bounds, just increment. Else,
            writeHead = (writeHead + amount < samplesToHold) ? writeHead += amount : samplesToHold -
                                                                                     (amount + writeHead);
        }

        void moveReadHead(int amount) {
            readHead = (readHead + amount < samplesToHold) ? readHead += amount : amount - samplesToHold - readHead;
        }

        void removeElementsInRange(int begin, int end) {
            data.removeIf([begin, end, this](juce::MidiMessageMetadata item) {
                return isInRange(begin, end, item.samplePosition);
            });
        }

        //Checks whether the given position is within the range. This handles wraparounds for us
        [[nodiscard]] bool isInRange(int begin, int end, int pos) const {
            if (begin < end) {
                return pos >= begin && pos <= end;
            }
            bool isBetweenBeginAndBounds = pos >= begin && pos < samplesToHold;
            bool isBetweenZeroAndEnd = pos >= 0 && pos <= end;
            return isBetweenBeginAndBounds && isBetweenZeroAndEnd;
        }

        //Translates an index in the ringBuffer to a corresponding index in an output buffer
        //For example, an element at index 5 should translate to index 3 if we give it the range 2-7
        [[nodiscard]] int whereInRange(int begin, int end, int pos) const {
            if (begin < end) return pos - begin;
            if (pos > begin && pos < samplesToHold) return pos - begin;
            return pos - (samplesToHold - begin);
        }

        static int millisecondsToSamples(double sampleRate, int milliseconds) {
            return ((int) sampleRate / 1000) * milliseconds;
        }


        struct MetadataComparator {
            static int compareElements(juce::MidiMessageMetadata first, juce::MidiMessageMetadata second) {
                if (first.samplePosition == second.samplePosition) return 0;
                return first.samplePosition < second.samplePosition ? -1 : 1;
            }
        };

        int spb; //Block size
        int samplesToHold;
        int writeHead = 0;
        int readHead;
        juce::MidiBuffer readBuffer;
        double sr; //Sample rate

        MetadataComparator comparator;
        juce::Array<juce::MidiMessageMetadata> data;
    };
}