#pragma once

#include <unordered_map>
#include <vector>

namespace nierika::dsp
{

class Timing
{
public:
    enum NoteTiming {
        NOTE_32 = 1,
        NOTE_32T = 2,
        NOTE_16 = 3,
        NOTE_16T = 4,
        NOTE_8 = 5,
        NOTE_8T = 6,
        NOTE_4 = 7,
        NOTE_4T = 8,
        NOTE_2 = 9,
        NOTE_2T = 10,
        NOTE_FULL = 11,
        NOTE_TWO_STEP = 12,
        NOTE_THREE_STEP = 13,
        NOTE_FOUR_STEP = 14,
    };
    
    static constexpr NoteTiming AvailableNoteTimings[] = {
        NOTE_32,
        NOTE_32T,
        NOTE_16,
        NOTE_16T,
        NOTE_8,
        NOTE_8T,
        NOTE_4,
        NOTE_4T,
        NOTE_2,
        NOTE_2T,
        NOTE_FULL,
        NOTE_TWO_STEP,
        NOTE_THREE_STEP,
        NOTE_FOUR_STEP
    };
    
    Timing();
    ~Timing();
    
    static int getBeatPosition(double ppqPosition, NoteTiming timing);
    static int getBeatPosition(double ppqPosition, NoteTiming timing, int nbOfSteps);
    static std::vector<std::pair<NoteTiming, std::string>> getAvailableNoteTimings();
    static const std::unordered_map<NoteTiming, std::string> getNoteTimingToNames();
    static float getTimeInMs(double bpm, Timing::NoteTiming timing);
    static float getRate(double bpm, Timing::NoteTiming timing);
};

}
