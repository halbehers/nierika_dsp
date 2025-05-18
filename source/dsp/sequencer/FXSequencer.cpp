#include "../../../include/dsp/sequencer/FXSequencer.h"

namespace nierika::dsp
{

FXSequencer::FXSequencer(int defaultNbOfSteps, Timing::NoteTiming defaultNoteTiming):
    _nbOfSteps(defaultNbOfSteps),
    _noteTiming(defaultNoteTiming)
{
    _activeSteps.reserve(static_cast<std::size_t>(_nbOfSteps));
    for (int i = 0; i < _nbOfSteps; ++i)
    {
        if (static_cast<std::size_t>(i) < _activeSteps.size()) continue;
        
        _activeSteps.push_back(true);
    }
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
        _activeSteps.erase(_activeSteps.begin() + nbOfSteps - 1, _activeSteps.begin() + static_cast<int>(_activeSteps.size()) - 1);
    }
    _nbOfSteps = nbOfSteps;
}

int FXSequencer::getNbOfSteps() const
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
    return nierika::dsp::Timing::getBeatPosition(ppqPosition, _noteTiming, _nbOfSteps);
}

bool FXSequencer::shouldProcess(const juce::Optional<juce::AudioPlayHead::PositionInfo>& position)
{
    if (!_isEnabled) return true;

    if (const std::optional<int> currentStep = getCurrentStepIndex(position); currentStep.has_value())
        return _activeSteps[static_cast<std::size_t>(currentStep.value())];

    return true;
}

bool FXSequencer::shouldProcess(double ppqPosition)
{
    if (!_isEnabled) return true;

    return _activeSteps[static_cast<std::size_t>(getCurrentStepIndex(ppqPosition))];
}

void FXSequencer::setEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

void FXSequencer::toggleStep(int index)
{
    if (static_cast<std::size_t>(index) >= _activeSteps.size() || index < 0) return;

    _activeSteps[static_cast<std::size_t>(index)] = !_activeSteps[static_cast<std::size_t>(index)];
}

void FXSequencer::setStepValue(int index, bool isActive)
{
    if (static_cast<std::size_t>(index) >= _activeSteps.size() || index < 0) return;
    
    _activeSteps[static_cast<std::size_t>(index)] = isActive;
}

void FXSequencer::registerSection(const std::string& identifier, bool isActivated)
{
    _sectionActivations[identifier] = isActivated;
}

void FXSequencer::unregisterSection(const std::string& identifier)
{
    _sectionActivations.erase(identifier);
}

void FXSequencer::toggleSectionActivation(const std::string& identifier)
{
    if (!isSectionRegistered(identifier)) return;
    
    setSectionActivation(identifier, !_sectionActivations[identifier]);
}

void FXSequencer::setSectionActivation(const std::string& identifier, bool isActivated)
{
    if (!isSectionRegistered(identifier)) return;

    _sectionActivations[identifier] = isActivated;
}

bool FXSequencer::isSectionRegistered(const std::string& identifier) const
{
    return _sectionActivations.contains(identifier);
}

bool FXSequencer::isSectionActivated(const std::string& identifier)
{
    if (!isSectionRegistered(identifier)) return false;
    
    return _sectionActivations[identifier];
}

}
