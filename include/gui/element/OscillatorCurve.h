#pragma once

#include <array>
#include <memory>
#include <string>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

// A live, passive preview of one full cycle of an oscillator's waveform - Shape (Sine/Triangle/
// Saw/Square, matching shapeParameterID's AudioParameterChoice index order), Warp (phase squeeze +
// zero-fill), Shape Noise (blended in, same as the DSP), and Fold (wavefolding), plus a start-phase
// marker driven by phasePercentParameterID/phaseRandomizeParameterID. Draws only into its own local
// bounds and owns no editing controls - see OscillatorCurveWithParameters for the composed widget
// that pairs this with the four ValueSetters that make those parameters editable.
//
// Reimplements the same shape/warp/noise/fold math the DSP oscillator class uses so the preview is
// literal, not approximate - see the .cpp for the exact formulas, which must be kept in sync with
// the actual DSP if either changes. Deliberately does not reflect Octave/Transpose/Detune/Unison/
// Sub/Output - those change pitch/thickness/level, not the single-cycle shape being previewed
// (same scoping LFOCurve already applies to its own params).
//
// The noise texture is drawn from a fixed table generated once at construction, not re-randomized
// every repaint - a literal per-repaint-random curve would look like flicker rather than a stable
// "this is roughly what the noise looks like" preview, which is what a static widget can usefully
// show.
class OscillatorCurve: public Component
{
public:
    OscillatorCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
                     const std::string& shapeParameterID, const std::string& warpParameterID,
                     const std::string& foldParameterID, const std::string& shapeNoiseParameterID,
                     const std::string& phasePercentParameterID, const std::string& phaseRandomizeParameterID);
    ~OscillatorCurve() override;

    void paint(juce::Graphics& g) override;

private:
    static constexpr int kPathResolution = 128; // samples across the width, plenty smooth at typical widget sizes

    [[nodiscard]] float computeShapeValue(double phase01) const noexcept;
    [[nodiscard]] float computeWarpedShapeValue(double outerPhase01) const noexcept;
    [[nodiscard]] float computeFinalShapeValue(int sampleIndex, double outerPhase01) const noexcept;
    [[nodiscard]] static float foldSample(float x) noexcept;
    [[nodiscard]] static float levelToY(float levelBipolar, float height) noexcept;

    juce::RangedAudioParameter* _shapeParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _shapeAttachment;
    int _shapeIndex = 0;

    juce::RangedAudioParameter* _warpParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _warpAttachment;
    float _warpPercent = 0.0f;

    juce::RangedAudioParameter* _foldParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _foldAttachment;
    float _foldPercent = 0.0f;

    juce::RangedAudioParameter* _shapeNoiseParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _shapeNoiseAttachment;
    float _shapeNoisePercent = 0.0f;
    std::array<float, kPathResolution + 1> _noiseTable {};

    juce::RangedAudioParameter* _phasePercentParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _phasePercentAttachment;
    float _phasePercent = 0.0f;

    juce::RangedAudioParameter* _phaseRandomizeParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _phaseRandomizeAttachment;
    bool _phaseRandomizeEnabled = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorCurve)
};

}
