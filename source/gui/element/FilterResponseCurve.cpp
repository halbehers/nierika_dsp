#include "../../../include/gui/element/FilterResponseCurve.h"

#include <cmath>

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

namespace
{
    constexpr float kFilterCurveMinFreqHz = 20.0f;
    constexpr float kFilterCurveMaxFreqHz = 20000.0f;
    constexpr float kFilterCurveMinDb = -48.0f;
    constexpr float kFilterCurveMaxDb = 24.0f;
    constexpr float kFilterCurveMinResonance = 0.1f;
    constexpr float kFilterCurveMaxResonance = 10.0f;
    constexpr float kFilterCurveBreakpointRadius = 5.0f;
    constexpr float kFilterCurveHitTestRadius = 14.0f;
    constexpr int kFilterCurvePathResolution = 128;
    constexpr float kFilterCurveVerticalPadding = 10.0f; // keeps the response curve's dB extremes clear of the top/bottom border
}

FilterResponseCurve::FilterResponseCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
                                          const std::string& typeParameterID, const std::string& slopeParameterID,
                                          const std::string& cutoffParameterID, const std::string& resonanceParameterID):
    Component(identifier)
{
    displayBackground(Theme::ThemeColor::SECONDARY_BACKGROUND, Theme::getBorderRadius());

    _type.parameter = parameterManager.getState().getParameter(typeParameterID);
    if (_type.parameter != nullptr)
    {
        _type.attachment = std::make_unique<juce::ParameterAttachment>(*_type.parameter,
            [this](float newValue) { _typeIndex = static_cast<int>(std::round(newValue)); repaint(); });
        _type.attachment->sendInitialUpdate();
    }

    _slope.parameter = parameterManager.getState().getParameter(slopeParameterID);
    if (_slope.parameter != nullptr)
    {
        _slope.attachment = std::make_unique<juce::ParameterAttachment>(*_slope.parameter,
            [this](float newValue) { _slopeIndex = static_cast<int>(std::round(newValue)); repaint(); });
        _slope.attachment->sendInitialUpdate();
    }

    _cutoff.parameter = parameterManager.getState().getParameter(cutoffParameterID);
    if (_cutoff.parameter != nullptr)
    {
        _cutoff.attachment = std::make_unique<juce::ParameterAttachment>(*_cutoff.parameter,
            [this](float newValue) { _cutoffHz = newValue; repaint(); });
        _cutoff.attachment->sendInitialUpdate();
    }

    _resonance.parameter = parameterManager.getState().getParameter(resonanceParameterID);
    if (_resonance.parameter != nullptr)
    {
        _resonance.attachment = std::make_unique<juce::ParameterAttachment>(*_resonance.parameter,
            [this](float newValue) { _resonanceValue = newValue; repaint(); });
        _resonance.attachment->sendInitialUpdate();
    }
}

FilterResponseCurve::~FilterResponseCurve() = default;

void FilterResponseCurve::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds().toFloat();
    const auto width = bounds.getWidth();

    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.3f));
    g.drawHorizontalLine(static_cast<int>(dbToY(0.f)), 0.f, width); // 0dB reference line
    for (const float freq : { 100.f, 1000.f, 10000.f })
        g.drawVerticalLine(static_cast<int>(freqToX(freq)), 0.f, bounds.getHeight());

    juce::Path path;
    for (int i = 0; i <= kFilterCurvePathResolution; ++i)
    {
        const auto x = (static_cast<float>(i) / static_cast<float>(kFilterCurvePathResolution)) * width;
        const auto y = dbToY(computeMagnitudeDb(xToFreq(x)));

        if (i == 0)
            path.startNewSubPath(x, y);
        else
            path.lineTo(x, y);
    }

    const auto accent = Theme::newColor(Theme::ThemeColor::ACCENT).asJuce();

    auto fillPath = path;
    fillPath.lineTo(width, dbToY(kFilterCurveMinDb));
    fillPath.lineTo(0.f, dbToY(kFilterCurveMinDb));
    fillPath.closeSubPath();
    g.setColour(accent.withAlpha(0.15f));
    g.fillPath(fillPath);

    g.setColour(accent);
    g.strokePath(path, juce::PathStrokeType(2.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    const auto dragPoint = computeDragPointPosition();
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
    g.fillEllipse(dragPoint.x - kFilterCurveBreakpointRadius, dragPoint.y - kFilterCurveBreakpointRadius, kFilterCurveBreakpointRadius * 2.f, kFilterCurveBreakpointRadius * 2.f);
    g.setColour(accent);
    g.drawEllipse(dragPoint.x - kFilterCurveBreakpointRadius, dragPoint.y - kFilterCurveBreakpointRadius, kFilterCurveBreakpointRadius * 2.f, kFilterCurveBreakpointRadius * 2.f, 1.5f);
}

void FilterResponseCurve::resized()
{
    Component::resized();
}

void FilterResponseCurve::mouseDown(const juce::MouseEvent& event)
{
    _isDragging = computeDragPointPosition().getDistanceFrom(event.position) <= kFilterCurveHitTestRadius;
    if (!_isDragging)
        return;

    _dragStartMouse = event.position;
    _dragStartCutoffHz = _cutoffHz;
    _dragStartResonance = _resonanceValue;

    if (_cutoff.attachment)
        _cutoff.attachment->beginGesture();
    if (_resonance.attachment)
        _resonance.attachment->beginGesture();
}

void FilterResponseCurve::mouseDrag(const juce::MouseEvent& event)
{
    if (!_isDragging)
        return;

    if (_cutoff.attachment != nullptr && _cutoff.parameter != nullptr)
    {
        const auto startX = freqToX(_dragStartCutoffHz);
        const auto newFreq = xToFreq(startX + (event.position.x - _dragStartMouse.x));
        const auto& range = _cutoff.parameter->getNormalisableRange();
        _cutoff.attachment->setValueAsPartOfGesture(juce::jlimit(range.start, range.end, newFreq));
    }

    if (_resonance.attachment != nullptr && _resonance.parameter != nullptr)
    {
        const auto startY = resonanceToY(_dragStartResonance);
        const auto newResonance = yToResonance(startY + (event.position.y - _dragStartMouse.y));
        const auto& range = _resonance.parameter->getNormalisableRange();
        _resonance.attachment->setValueAsPartOfGesture(juce::jlimit(range.start, range.end, newResonance));
    }
}

void FilterResponseCurve::mouseUp(const juce::MouseEvent&)
{
    if (!_isDragging)
        return;

    if (_cutoff.attachment)
        _cutoff.attachment->endGesture();
    if (_resonance.attachment)
        _resonance.attachment->endGesture();

    _isDragging = false;
}

void FilterResponseCurve::mouseMove(const juce::MouseEvent& event)
{
    const auto isOverDragPoint = computeDragPointPosition().getDistanceFrom(event.position) <= kFilterCurveHitTestRadius;
    setMouseCursor(isOverDragPoint ? juce::MouseCursor::DraggingHandCursor : juce::MouseCursor::NormalCursor);
}

float FilterResponseCurve::computeMagnitudeDb(float frequencyHz) const noexcept
{
    // Standard analytic magnitude response of a 2nd-order (12dB/octave) state-variable filter -
    // x is the frequency ratio to cutoff, q is resonance (q=1/sqrt(2) is the flat/Butterworth
    // response, matching audio::VoiceFilter's own default in the chords_theory plugin).
    const auto x = frequencyHz / juce::jmax(1.0f, _cutoffHz);
    const auto q = juce::jmax(0.01f, _resonanceValue);
    const auto denom = std::sqrt(std::pow(1.0f - x * x, 2.0f) + std::pow(x / q, 2.0f));

    float magnitude;
    switch (_typeIndex)
    {
        case 1: magnitude = (x * x) / denom; break;       // high-pass
        case 2: magnitude = (x / q) / denom; break;        // band-pass
        case 0:
        default: magnitude = 1.0f / denom; break;          // low-pass
    }

    auto db = 20.0f * std::log10(juce::jmax(1.0e-6f, magnitude));

    if (_slopeIndex == 1) // 24dB/octave: two identical cascaded stages -> magnitudes multiply, dB doubles
        db *= 2.0f;

    return db;
}

float FilterResponseCurve::freqToX(float frequencyHz) const noexcept
{
    const auto width = static_cast<float>(getWidth());
    const auto clamped = juce::jlimit(kFilterCurveMinFreqHz, kFilterCurveMaxFreqHz, frequencyHz);
    return width * std::log(clamped / kFilterCurveMinFreqHz) / std::log(kFilterCurveMaxFreqHz / kFilterCurveMinFreqHz);
}

float FilterResponseCurve::xToFreq(float x) const noexcept
{
    const auto width = static_cast<float>(getWidth());
    if (width <= 0.f)
        return kFilterCurveMinFreqHz;

    const auto t = juce::jlimit(0.0f, 1.0f, x / width);
    return kFilterCurveMinFreqHz * std::pow(kFilterCurveMaxFreqHz / kFilterCurveMinFreqHz, t);
}

float FilterResponseCurve::dbToY(float db) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = juce::jmax(0.f, height - kFilterCurveVerticalPadding * 2.f);
    const auto clamped = juce::jlimit(kFilterCurveMinDb, kFilterCurveMaxDb, db);
    return kFilterCurveVerticalPadding + usableHeight * (kFilterCurveMaxDb - clamped) / (kFilterCurveMaxDb - kFilterCurveMinDb);
}

float FilterResponseCurve::resonanceToY(float resonance) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = juce::jmax(0.f, height - kFilterCurveVerticalPadding * 2.f);
    const auto clamped = juce::jlimit(kFilterCurveMinResonance, kFilterCurveMaxResonance, resonance);
    const auto t = std::log(clamped / kFilterCurveMinResonance) / std::log(kFilterCurveMaxResonance / kFilterCurveMinResonance);
    return (height - kFilterCurveVerticalPadding) - t * usableHeight;
}

float FilterResponseCurve::yToResonance(float y) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = height - kFilterCurveVerticalPadding * 2.f;
    if (usableHeight <= 0.f)
        return kFilterCurveMinResonance;

    const auto t = juce::jlimit(0.0f, 1.0f, 1.0f - (y - kFilterCurveVerticalPadding) / usableHeight);
    return kFilterCurveMinResonance * std::pow(kFilterCurveMaxResonance / kFilterCurveMinResonance, t);
}

juce::Point<float> FilterResponseCurve::computeDragPointPosition() const
{
    return { freqToX(_cutoffHz), resonanceToY(_resonanceValue) };
}

}
