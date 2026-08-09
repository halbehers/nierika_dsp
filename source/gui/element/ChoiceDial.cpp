#include "../../../include/gui/element/ChoiceDial.h"

#include <cmath>

namespace nierika::gui::element
{

ChoiceDial::ChoiceDial(const std::string& identifier, const std::string& label, const std::vector<std::string>& choices, int defaultIndex):
    Dial(identifier, label, 0.0f, choices.empty() ? 0.0f : static_cast<float>(choices.size() - 1), static_cast<float>(defaultIndex)),
    _choices(choices)
{
    ChoiceDial::setup();
}

ChoiceDial::ChoiceDial(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Dial(parameterManager, parameterID)
{
    if (auto* parameter = parameterManager.getState().getParameter(parameterID))
        if (auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*>(parameter))
            for (const auto& choice : choiceParameter->choices)
                _choices.push_back(choice.toStdString());

    ChoiceDial::setup();
}

void ChoiceDial::setup()
{
    _slider.textFromValueFunction = [this](double value) { return getTextFromValue(value); };
}

juce::String ChoiceDial::getTextFromValue(double value) const
{
    const auto index = static_cast<int>(std::round(value));
    if (index < 0 || index >= static_cast<int>(_choices.size()))
        return {};

    return _choices[static_cast<std::size_t>(index)];
}

}
