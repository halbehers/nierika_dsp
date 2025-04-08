#include "../../../includes/dsp/sequencer/FXSequencer.h"

namespace dsp {

FXSequencer::FXSequencer(int defaultNbOfSteps, Timing::NoteTiming defaultNoteTiming):
    _nbOfSteps(defaultNbOfSteps),
    _noteTiming(defaultNoteTiming)
{
    _activeSteps.reserve(_nbOfSteps);
    for (int i = 0; i < _nbOfSteps; ++i)
    {
        if (i < _activeSteps.size()) continue;
        
        _activeSteps.push_back(true);
    }
}

FXSequencer::~FXSequencer()
{
}

void FXSequencer::setNbOfSteps(int nbOfSteps)
{
    for (int i = 0; i < nbOfSteps; ++i)
    {
        if (i < _activeSteps.size()) continue;
        
        _activeSteps.push_back(true);
    }
    if (nbOfSteps < _nbOfSteps)
    {
        _activeSteps.erase(_activeSteps.begin() + nbOfSteps - 1, _activeSteps.begin() + _activeSteps.size() - 1);
    }
    _nbOfSteps = nbOfSteps;
}

int FXSequencer::getNbOfSteps()
{
    return _nbOfSteps;
}

void FXSequencer::setNoteTiming(Timing::NoteTiming noteTiming)
{
    _noteTiming = noteTiming;
}

std::optional<int> FXSequencer::getCurrentStepIndex(juce::Optional<juce::AudioPlayHead::PositionInfo> position)
{
    if (!position->getIsPlaying())
    {
        return std::optional<int>();
    }
    
    juce::Optional<double> ppqPosition = position->getPpqPosition();
    
    if (ppqPosition.hasValue())
    {
        return std::optional<int>(getCurrentStepIndex(*ppqPosition));
    }
    return std::optional<int>();
}

int FXSequencer::getCurrentStepIndex(double ppqPosition)
{
    return _timing.getBeatPosition(ppqPosition, _noteTiming, _nbOfSteps);
}

bool FXSequencer::shouldProcess(juce::Optional<juce::AudioPlayHead::PositionInfo> position)
{
    if (!_isEnabled) return true;

    std::optional<int> currentStep = getCurrentStepIndex(position);

    if (currentStep.has_value()) return _activeSteps[currentStep.value()];

    return true;
}


bool FXSequencer::shouldProcess(double ppqPosition)
{
    if (!_isEnabled) return true;

    return _activeSteps[getCurrentStepIndex(ppqPosition)];
}

void FXSequencer::setEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

void FXSequencer::toggleStep(int index)
{
    if (index >= _activeSteps.size() || index < 0) return;

    _activeSteps[index] = !_activeSteps[index];
}

void FXSequencer::setStepValue(int index, bool isActive)
{
    if (index >= _activeSteps.size() || index < 0) return;
    
    _activeSteps[index] = isActive;
}

void FXSequencer::registerSection(std::string ID, bool isActivated)
{
    _sectionActivations[ID] = isActivated;
}

void FXSequencer::unregisterSection(std::string ID)
{
    _sectionActivations.erase(ID);
}

void FXSequencer::toggleSectionActivation(std::string ID)
{
    if (!isSectionRegistered(ID)) return;
    
    setSectionActivation(ID, !_sectionActivations[ID]);
}

void FXSequencer::setSectionActivation(std::string ID, bool isActivated)
{
    if (!isSectionRegistered(ID)) return;

    _sectionActivations[ID] = isActivated;
}

bool FXSequencer::isSectionRegistered(std::string ID)
{
    return _sectionActivations.count(ID) != 0;
}

bool FXSequencer::isSectionActivated(std::string ID)
{
    if (!isSectionRegistered(ID)) return false;
    
    return _sectionActivations[ID];
}

}
