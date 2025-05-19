#pragma once

#include "../Timing.h"

namespace nierika::dsp
{

class FXSequencer
{
public:
    FXSequencer() = default;
    FXSequencer(int defaultNbOfSteps, Timing::NoteTiming defaultNoteTiming);
    ~FXSequencer() = default;

    void setNbOfSteps(int nbOfSteps);
    void setNoteTiming(Timing::NoteTiming noteTiming);
    [[nodiscard]] int getNbOfSteps() const;

    std::optional<int> getCurrentStepIndex(juce::Optional<juce::AudioPlayHead::PositionInfo> position);
    int getCurrentStepIndex(double ppqPosition);
    bool shouldProcess(const juce::Optional<juce::AudioPlayHead::PositionInfo> &position);
    bool shouldProcess(double ppqPosition);

    void setEnabled(bool isEnabled);

    void toggleStep(int index);
    void setStepValue(int index, bool isActive);
    
    void registerSection(const std::string &identifier, bool isActivated = true);
    void unregisterSection(const std::string &identifier);
    void toggleSectionActivation(const std::string &identifier);
    void setSectionActivation(const std::string &identifier, bool isActivated);
    bool isSectionRegistered(const std::string &identifier) const;
    bool isSectionActivated(const std::string &identifier);

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
