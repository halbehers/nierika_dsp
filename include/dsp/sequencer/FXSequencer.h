#pragma once

#include "../Timing.h"

namespace nierika::dsp
{

class FXSequencer
{
public:
    FXSequencer() = default;
    FXSequencer(int defaultNbOfSteps, Timing::NoteTiming defaultNoteTiming);
    ~FXSequencer();

    void setNbOfSteps(int nbOfSteps);
    void setNoteTiming(Timing::NoteTiming noteTiming);
    int getNbOfSteps();

    std::optional<int> getCurrentStepIndex(juce::Optional<juce::AudioPlayHead::PositionInfo> position);
    int getCurrentStepIndex(double ppqPosition);
    bool shouldProcess(juce::Optional<juce::AudioPlayHead::PositionInfo> position);
    bool shouldProcess(double ppqPosition);

    void setEnabled(bool isEnabled);

    void toggleStep(int index);
    void setStepValue(int index, bool isActive);
    
    void registerSection(std::string ID, bool isActivated = true);
    void unregisterSection(std::string ID);
    void toggleSectionActivation(std::string ID);
    void setSectionActivation(std::string ID, bool isActivated);
    bool isSectionRegistered(std::string ID);
    bool isSectionActivated(std::string ID);

protected:
    std::vector<bool> _activeSteps;
    std::unordered_map<std::string, bool> _sectionActivations;

private:
    int _nbOfSteps = 16;
    Timing::NoteTiming _noteTiming = Timing::NOTE_4;
    bool _isEnabled = true;
    dsp::Timing _timing;
};

}
