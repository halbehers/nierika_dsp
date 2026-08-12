#pragma once

#include <memory>
#include <string>

#include "../Component.h"
#include "../../dsp/ParameterManager.h"

namespace nierika::gui::element
{

// A frequency-response curve (log-frequency x-axis, dB y-axis, 20Hz-20kHz) for a 2-pole
// low/high/band-pass state-variable filter, with an optional second cascaded stage for a 24dB
// /octave slope - APVTS-attached to type/slope/cutoff/resonance parameters. Computes the standard
// analytic 2nd-order SVF magnitude response itself (juce::dsp::StateVariableTPTFilter doesn't
// expose a magnitude-response query) - this must stay numerically consistent with whatever DSP
// class is actually driving the sound (e.g. audio::VoiceFilter in the chords_theory plugin) or
// the curve will visibly misrepresent what the filter is doing; see the .cpp for the formula.
// One draggable point at (cutoff, resonance)'s plotted position - horizontal drag -> cutoff
// (log-mapped), vertical drag -> resonance (also log-mapped, independently of the curve's own dB
// scale - the point's height is a resonance readout, not literally sampled off the curve).
class FilterResponseCurve: public Component
{
public:
    // typeParameterID: AudioParameterChoice index 0=low-pass,1=high-pass,2=band-pass.
    // slopeParameterID: AudioParameterChoice index 0=12dB/octave (1 stage), 1=24dB/octave (2 cascaded).
    FilterResponseCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
                         const std::string& typeParameterID, const std::string& slopeParameterID,
                         const std::string& cutoffParameterID, const std::string& resonanceParameterID);
    ~FilterResponseCurve() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseMove(const juce::MouseEvent& event) override;

private:
    [[nodiscard]] float computeMagnitudeDb(float frequencyHz) const noexcept;

    [[nodiscard]] float freqToX(float frequencyHz) const noexcept;
    [[nodiscard]] float xToFreq(float x) const noexcept;
    [[nodiscard]] float dbToY(float db) const noexcept;

    [[nodiscard]] float resonanceToY(float resonance) const noexcept;
    [[nodiscard]] float yToResonance(float y) const noexcept;

    [[nodiscard]] juce::Point<float> computeDragPointPosition() const;

    template<typename T>
    struct Binding
    {
        T* parameter = nullptr;
        std::unique_ptr<juce::ParameterAttachment> attachment;
    };

    Binding<juce::RangedAudioParameter> _type;
    Binding<juce::RangedAudioParameter> _slope;
    Binding<juce::RangedAudioParameter> _cutoff;
    Binding<juce::RangedAudioParameter> _resonance;

    int _typeIndex = 0;
    int _slopeIndex = 0;
    float _cutoffHz = 1000.0f;
    float _resonanceValue = 0.70710678f;

    bool _isDragging = false;
    juce::Point<float> _dragStartMouse;
    float _dragStartCutoffHz = 0.0f;
    float _dragStartResonance = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterResponseCurve)
};

}
