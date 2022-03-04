#include "juce_audio_basics/juce_audio_basics.h"

namespace Utility {
    class MidiLengthFilter {
    public:
        MidiLengthFilter(double sampleRate, int blockSize, int samplesToHold) :
                spb(blockSize), sr(sampleRate), samplesToHold(samplesToHold), readHead(blockSize) {
        }

        void push(juce::MidiBuffer &buffer) {
            int samplePos = 0; //Holds the last written data position
            for (auto metadata: buffer) {
                moveWriteHead(metadata.samplePosition - samplePos);
                samplePos = metadata.samplePosition;
                metadata.samplePosition = writeHead;
                data.emplace_back(metadata); //write the data
            }
            if (samplePos < spb) {
                //make sure the writeHead has moved to the "end" of the given buffer
                moveWriteHead(spb - samplePos);
            }
            moveReadHead(spb);

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

            if (data.empty()) return; //No need to overwrite anything

            //We only need to check for overwrites if the
            auto lastInsertedPosition = data.back().samplePosition;
            auto oldestPosition = data.front().samplePosition;
            if (lastInsertedPosition < oldestPosition) {
                while (!data.empty() && oldestPosition <= writeHead) {
                    data.pop_front(); //Delete elements at front
                    oldestPosition = data.front().samplePosition;
                }
            }
        }

        void moveReadHead(int amount) {
            int oldPos = readHead;
            readHead = (readHead + amount < samplesToHold) ? readHead += amount : amount - samplesToHold - readHead;

            if(data.empty()) return;
            auto it = data.begin();
        }

        bool isInRange(int begin, int end, int pos){
            if(begin < end){
                return pos >= begin && pos <= end;
            }
            bool isBetweenBeginAndBounds = pos >= begin && pos < samplesToHold;
            bool isBetweenZeroAndEnd = pos >= 0 && pos <= end;
            return isBetweenBeginAndBounds && isBetweenZeroAndEnd;
        }

        static int millisecondsToSamples(double sampleRate, int milliseconds) {
            return ((int) sampleRate / 1000) * milliseconds;
        }

        int spb; //Block size
        int samplesToHold;
        int writeHead = 0;
        int readHead;
        juce::MidiBuffer readBuffer;
        double sr; //Sample rate
        std::deque<juce::MidiMessageMetadata> data;
    };
}