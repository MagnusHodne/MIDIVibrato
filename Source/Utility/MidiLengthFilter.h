#include "juce_audio_basics/juce_audio_basics.h"
#include <deque>

namespace Utility {
    class MidiLengthFilter {
    public:
        MidiLengthFilter(int blockSize, int samplesToHold) :
                spb(blockSize), samplesToHold(samplesToHold) {
            if (spb > samplesToHold) {
                throw std::range_error("Cannot hold a negative time delay!");
            }
        }

        void push(const juce::MidiBuffer &buffer) {
            if (spb == samplesToHold) {
                readBuffer.addEvents(buffer, 0, spb, 0);
                return;
            }

            int samplePos = 0; //Holds the last written data position
            for (auto metadata: buffer) {
                moveWriteHead(metadata.samplePosition - samplePos);
                samplePos = metadata.samplePosition;
                metadata.samplePosition = writeHead;
                data.emplace_back(metadata);
            }
            if (samplePos < spb) {
                //make sure the writeHead has moved to the "end" of the given buffer
                moveWriteHead(spb - samplePos);
            }

            handleOldData();

        }

        /// Returns a MidiBuffer with the "oldest" data in the buffer. The block size
        /// of this buffer is the same as the buffer supplied in the push method
        juce::MidiBuffer peek() {
            return readBuffer;
        }

    private:

        /// Copies over the oldest data in the ringBuffer to the readBuffer before erasing ("overwriting") it
        void handleOldData() {
            int start = writeHead;
            moveWriteHead(spb);
            readBuffer.clear();

            if (!data.empty()) {
                auto iterator = data.begin();
                while (!data.empty() && iterator != data.end() &&
                       isInRange(start, writeHead, iterator->samplePosition)) {
                    int translatedPosition = whereInRange(start, writeHead, iterator->samplePosition);
                    readBuffer.addEvent(iterator->getMessage(), translatedPosition);

                    data.pop_front();
                    iterator = data.begin();
                }
            }
            writeHead = start;
        }

        void moveWriteHead(int amount) {
            if (amount > 0) {
                //Traverse forwards
                writeHead = (writeHead + amount < samplesToHold) ? writeHead += amount : amount -
                                                                                         (samplesToHold - writeHead);
            } else {
                //Traverse backwards
                writeHead = (writeHead + amount >= 0) ? writeHead += amount : samplesToHold - (writeHead + amount);
            }
        }

        //Checks whether the given position is within the range. This handles wraparounds for us
        [[nodiscard]] bool isInRange(int begin, int end, int pos) const {
            if (begin < end) {
                return pos >= begin && pos <= end;
            }
            bool isBetweenBeginAndBounds = pos >= begin && pos < samplesToHold;
            bool isBetweenZeroAndEnd =
                    pos >= 0 && pos < end; //If the blockSize is 4, then we want to return true for indices 0-3
            return isBetweenBeginAndBounds || isBetweenZeroAndEnd;
        }

        //Translates an index in the ringBuffer to a corresponding index in an output buffer
        //For example, an element at index 5 should translate to index 3 if we give it the range 2-7
        [[nodiscard]] int whereInRange(int begin, int end, int pos) const {
            if (begin > end) {
                return pos + (samplesToHold - begin);
            }
            return pos - begin;
        }

        static int millisecondsToSamples(double sampleRate, int milliseconds) {
            return ((int) sampleRate / 1000) * milliseconds;
        }

        int spb; //Block size
        int samplesToHold;
        int writeHead = 0;
        juce::MidiBuffer readBuffer;
        std::deque<juce::MidiMessageMetadata> data;
    };
}