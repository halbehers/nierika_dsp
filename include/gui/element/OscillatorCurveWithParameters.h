#pragma once

#include <string>

#include "../Component.h"
#include "../layout/GridLayout.h"
#include "../../dsp/ParameterManager.h"
#include "./OscillatorCurve.h"
#include "./ValueSetter.h"

namespace nierika::gui::element
{

// Composes a passive OscillatorCurve (top row) with the four ValueSetter widgets that make its
// Phase/Noise/Warp/Fold parameters actually editable (bottom row, fixed height) - the curve itself
// only ever draws its waveform preview into its own local bounds, this is what turns that preview
// into the interactive control. Both the curve's own ParameterAttachments and the ValueSetters'
// independent ones attach to the identical four parameters - two simultaneous attachments per
// parameter is an already-established, working pattern in this codebase, so no cross-wiring
// between the two is needed for either to stay correct.
class OscillatorCurveWithParameters: public Component
{
public:
    OscillatorCurveWithParameters(dsp::ParameterManager& parameterManager, const std::string& identifier,
                                   const std::string& shapeParameterID, const std::string& warpParameterID,
                                   const std::string& foldParameterID, const std::string& shapeNoiseParameterID,
                                   const std::string& phasePercentParameterID, const std::string& phaseRandomizeParameterID);
    ~OscillatorCurveWithParameters() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Locale-reactive label passthrough to the four owned ValueSetters - the caller (e.g.
    // SynthOscillatorSection::refreshLabels()) already has the translated strings, matching how
    // it refreshes every other dial label.
    void setPhaseDisplayLabel(const juce::String& label) { _phaseSetter.setDisplayLabel(label); }
    void setNoiseDisplayLabel(const juce::String& label) { _noiseSetter.setDisplayLabel(label); }
    void setWarpDisplayLabel(const juce::String& label) { _warpSetter.setDisplayLabel(label); }
    void setFoldDisplayLabel(const juce::String& label) { _foldSetter.setDisplayLabel(label); }

    // Same passthrough shape as the labels above, but applied uniformly to all four - unlike their
    // (necessarily distinct) text, the four ValueSetters are always meant to read as one consistent
    // set, so there's no case for sizing them independently.
    void setValueSettersFontSize(Theme::FontSize size);

private:
    static constexpr float kValueRowHeight = 38.0f;
    static constexpr float kValueParametersPadding = 4.f;

    layout::GridLayout<Component> _layout { *this };

    OscillatorCurve _curve;
    ValueSetter _phaseSetter;
    ValueSetter _noiseSetter;
    ValueSetter _warpSetter;
    ValueSetter _foldSetter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorCurveWithParameters)
};

}
