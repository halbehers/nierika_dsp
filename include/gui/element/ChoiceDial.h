#pragma once

#include <string>
#include <vector>

#include "Dial.h"

namespace nierika::gui::element
{

// A Dial whose displayed/hovered value text is looked up from an arbitrary list of string
// choices instead of a raw number - the dial-shaped equivalent of Cycler, for discrete-choice
// parameters that read better as a small dial than a pair of arrows (e.g. a 2-3 state mode
// selector). Choices are indexed 0..choices.size()-1, matching juce::AudioParameterChoice's own
// indexing, so the APVTS-attached constructor works directly against any AudioParameterChoice -
// note that parameter must still be registered through ParameterManager's float-typed template
// (not int), since Dial's own APVTS constructor reads its min/max/default via
// ParameterManager::getParameterMinValue<float>/etc, which requires a type match.
class ChoiceDial: public Dial
{
public:
    ChoiceDial(const std::string& identifier, const std::string& label, const std::vector<std::string>& choices, int defaultIndex = 0);
    ChoiceDial(dsp::ParameterManager& parameterManager, const std::string& parameterID);
    ~ChoiceDial() override = default;

private:
    std::vector<std::string> _choices;

    void setup() override;
    [[nodiscard]] juce::String getTextFromValue(double value) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChoiceDial)
};

}
