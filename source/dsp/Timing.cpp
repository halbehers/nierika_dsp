namespace nierika::dsp
{

Timing::Timing()
{
}

Timing::~Timing()
{
}

int Timing::getBeatPosition(const double ppqPosition, const NoteTiming timing)
{
    const std::unordered_map<NoteTiming, int> timingToPpq {
       {
           { NOTE_32, 120 },
           { NOTE_32T, 120 }, // TODO: find value.
           { NOTE_16, 240 },
           { NOTE_16T, 240 }, // TODO: find value.
           { NOTE_8, 480 },
           { NOTE_8T, 480 }, // TODO: find value.
           { NOTE_4, 960 },
           { NOTE_4T, 960 }, // TODO: find value.
           { NOTE_2, 1920 },
           { NOTE_2T, 1920 }, // TODO: find value.
           { NOTE_FULL, 3840 },
           { NOTE_TWO_STEP, 7680 },
           { NOTE_THREE_STEP, 11520 },
           { NOTE_FOUR_STEP, 15360 }
       }
   };
    return (int) ((ppqPosition * 960.0 + 24.0) / (int) timingToPpq.at(timing));
}
 

int Timing::getBeatPosition(const double ppqPosition, const NoteTiming timing, const int nbOfSteps)
{
    return getBeatPosition(ppqPosition, timing) % nbOfSteps;
}

const std::unordered_map<Timing::NoteTiming, std::string> Timing::getNoteTimingToNames()
{
    return {
        { NOTE_32, "1/32" },
        { NOTE_32T, "1/32 T" },
        { NOTE_16, "1/16" },
        { NOTE_16T, "1/16 T" },
        { NOTE_8, "1/8" },
        { NOTE_8T, "1/8 T" },
        { NOTE_4, "1/4" },
        { NOTE_4T, "1/4 T" },
        { NOTE_2, "1/2" },
        { NOTE_2T, "1/2 T" },
        { NOTE_FULL, "Full note" },
        { NOTE_TWO_STEP, "Two step" },
        { NOTE_THREE_STEP, "Three step" },
        { NOTE_FOUR_STEP, "Four step" }
    };
}

std::vector<std::pair<Timing::NoteTiming, std::string>> Timing::getAvailableNoteTimings()
{
    const std::unordered_map<NoteTiming, std::string> timingToName = getNoteTimingToNames();
    std::vector<std::pair<Timing::NoteTiming, std::string>> noteTimings;
    constexpr int nbOfAvailableNoteTimings = sizeof(Timing::AvailableNoteTimings) / sizeof(int);
    noteTimings.reserve(static_cast<std::size_t>(nbOfAvailableNoteTimings));

    for (auto noteTiming : Timing::AvailableNoteTimings)
    {
        noteTimings.emplace_back(noteTiming, timingToName.at(noteTiming));
    }

    return noteTimings;
}

float Timing::getTimeInMs(const double bpm, const Timing::NoteTiming timing)
{
    const float timeInMsQuarter = 60000.f / static_cast<float>(bpm);
    const std::unordered_map<NoteTiming, float> timingToMultiplicator {
       {
           { NOTE_32, 0.125f },
           { NOTE_32T, 0.125f }, // TODO: find value.
           { NOTE_16, 0.25f },
           { NOTE_16T, 0.25f }, // TODO: find value.
           { NOTE_8, 0.5f },
           { NOTE_8T, 0.5f }, // TODO: find value.
           { NOTE_4, 1.0f },
           { NOTE_4T, 1.0f }, // TODO: find value.
           { NOTE_2, 2.0f },
           { NOTE_2T, 2.0f }, // TODO: find value.
           { NOTE_FULL, 4.0f },
           { NOTE_TWO_STEP, 8.0f },
           { NOTE_THREE_STEP, 12.0f },
           { NOTE_FOUR_STEP, 16.0f }
       }
   };

    return timeInMsQuarter * timingToMultiplicator.at(timing);
}

}
