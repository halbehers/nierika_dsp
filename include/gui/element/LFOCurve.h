#pragma once

#include <memory>
#include <string>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

// A live preview of one full cycle of an LFO's waveform (Sine/Triangle/Saw/Square, matching the
// shapeParameterID's AudioParameterChoice index order), APVTS-attached to a shape parameter and a
// smooth parameter, with one draggable point (vertical drag -> smooth). Reimplements the same
// shape/smooth math a bound DSP LFO would use internally (raw shape blended toward a sine via
// smooth) so the curve is a literal, not approximate, preview - see the .cpp for the exact
// per-shape formulas, which must be kept in sync with the actual DSP if either changes.
class LFOCurve: public Component
{
public:
    LFOCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
             const std::string& shapeParameterID, const std::string& smoothParameterID);
    ~LFOCurve() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

private:
    [[nodiscard]] float computeShapeValue(double phase01) const noexcept;
    // The draggable point sits at the curve's horizontal midpoint (phase 0.5) - not necessarily
    // each shape's visual peak, but a single fixed, predictable reference that works consistently
    // across all four shapes without special-casing each one's differing peak phase.
    [[nodiscard]] juce::Point<float> computeDragPointPosition() const;
    [[nodiscard]] float levelToY(float levelBipolar) const noexcept;

    juce::RangedAudioParameter* _shapeParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _shapeAttachment;
    int _shapeIndex = 0;

    juce::RangedAudioParameter* _smoothParameter = nullptr;
    std::unique_ptr<juce::ParameterAttachment> _smoothAttachment;
    float _smoothPercent = 0.0f;

    bool _isDraggingSmooth = false;
    float _dragStartMouseY = 0.0f;
    float _dragStartSmoothPercent = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFOCurve)
};

}
