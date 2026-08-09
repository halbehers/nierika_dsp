#include "../../../include/gui/element/OscillatorCurve.h"

#include <cmath>

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

namespace
{
    constexpr float kOscillatorCurveVerticalPadding = 10.0f; // keeps the peak/trough clear of the top/bottom border
    constexpr float kOscillatorCurveMinActiveWidth = 0.02f; // mirrors audio::Oscillator's own Warp minimum
    constexpr float kOscillatorCurveFoldMaxGain = 6.0f; // mirrors audio::Oscillator's own Fold pre-gain constant
    constexpr float kPhaseMarkerFlagWidth = 8.0f;
    constexpr float kPhaseMarkerFlagHeight = 8.0f;
}

OscillatorCurve::OscillatorCurve(dsp::ParameterManager& parameterManager, const std::string& identifier,
                                  const std::string& shapeParameterID, const std::string& warpParameterID,
                                  const std::string& foldParameterID, const std::string& shapeNoiseParameterID,
                                  const std::string& phasePercentParameterID, const std::string& phaseRandomizeParameterID):
    Component(identifier)
{
    juce::Random noiseRandom;
    for (auto& sample : _noiseTable)
        sample = noiseRandom.nextFloat() * 2.0f - 1.0f;

    _shapeParameter = parameterManager.getState().getParameter(shapeParameterID);
    if (_shapeParameter != nullptr)
    {
        _shapeAttachment = std::make_unique<juce::ParameterAttachment>(*_shapeParameter,
            [this](float newValue)
            {
                _shapeIndex = static_cast<int>(std::round(newValue));
                repaint();
            });
        _shapeAttachment->sendInitialUpdate();
    }

    _warpParameter = parameterManager.getState().getParameter(warpParameterID);
    if (_warpParameter != nullptr)
    {
        _warpAttachment = std::make_unique<juce::ParameterAttachment>(*_warpParameter,
            [this](float newValue)
            {
                _warpPercent = newValue;
                repaint();
            });
        _warpAttachment->sendInitialUpdate();
    }

    _foldParameter = parameterManager.getState().getParameter(foldParameterID);
    if (_foldParameter != nullptr)
    {
        _foldAttachment = std::make_unique<juce::ParameterAttachment>(*_foldParameter,
            [this](float newValue)
            {
                _foldPercent = newValue;
                repaint();
            });
        _foldAttachment->sendInitialUpdate();
    }

    _shapeNoiseParameter = parameterManager.getState().getParameter(shapeNoiseParameterID);
    if (_shapeNoiseParameter != nullptr)
    {
        _shapeNoiseAttachment = std::make_unique<juce::ParameterAttachment>(*_shapeNoiseParameter,
            [this](float newValue)
            {
                _shapeNoisePercent = newValue;
                repaint();
            });
        _shapeNoiseAttachment->sendInitialUpdate();
    }

    _phasePercentParameter = parameterManager.getState().getParameter(phasePercentParameterID);
    if (_phasePercentParameter != nullptr)
    {
        _phasePercentAttachment = std::make_unique<juce::ParameterAttachment>(*_phasePercentParameter,
            [this](float newValue)
            {
                _phasePercent = newValue;
                repaint();
            });
        _phasePercentAttachment->sendInitialUpdate();
    }

    _phaseRandomizeParameter = parameterManager.getState().getParameter(phaseRandomizeParameterID);
    if (_phaseRandomizeParameter != nullptr)
    {
        _phaseRandomizeAttachment = std::make_unique<juce::ParameterAttachment>(*_phaseRandomizeParameter,
            [this](float newValue)
            {
                _phaseRandomizeEnabled = newValue >= 0.5f;
                repaint();
            });
        _phaseRandomizeAttachment->sendInitialUpdate();
    }
}

OscillatorCurve::~OscillatorCurve() = default;

void OscillatorCurve::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds().toFloat();
    const auto width = bounds.getWidth();
    const auto height = bounds.getHeight();

    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.3f));
    g.drawHorizontalLine(static_cast<int>(levelToY(0.f, height)), 0.f, width);

    juce::Path path;
    for (int i = 0; i <= kPathResolution; ++i)
    {
        const auto phase01 = static_cast<double>(i) / static_cast<double>(kPathResolution);
        const auto x = (static_cast<float>(i) / static_cast<float>(kPathResolution)) * width;
        const auto y = levelToY(computeFinalShapeValue(i, phase01), height);

        if (i == 0)
            path.startNewSubPath(x, y);
        else
            path.lineTo(x, y);
    }

    // Reflects Section::setBypass()'s disable cascade (see SynthOscillatorSection, which
    // registers this component so setEnabled(false) reaches it) rather than a separate override
    // method - isEnabled() already carries the correct state once that's wired up.
    const auto accent = Theme::newColor(isEnabled() ? Theme::ThemeColor::SECONDARY_ACCENT : Theme::ThemeColor::DISABLED).asJuce();

    auto fillPath = path;
    fillPath.lineTo(width, levelToY(0.f, height));
    fillPath.lineTo(0.f, levelToY(0.f, height));
    fillPath.closeSubPath();
    g.setColour(accent.withAlpha(0.15f));
    g.fillPath(fillPath);

    g.setColour(accent);
    g.strokePath(path, juce::PathStrokeType(2.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Start-phase marker - hidden when phaseRandomizeEnabled, since the dial's own value isn't
    // actually used for playback in that mode and showing a precise line would be misleading.
    if (!_phaseRandomizeEnabled)
    {
        const auto phaseX = (juce::jlimit(0.0f, 100.0f, _phasePercent) / 100.0f) * width;
        const auto topY = levelToY(1.f, height);
        const auto bottomY = levelToY(-1.f, height);

        g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce().withAlpha(0.7f));

        juce::Path markerLine;
        markerLine.startNewSubPath(phaseX, topY);
        markerLine.lineTo(phaseX, bottomY);
        g.strokePath(markerLine, juce::PathStrokeType(1.5f));

        juce::Path markerFlag;
        markerFlag.addTriangle(phaseX - kPhaseMarkerFlagWidth * 0.5f, topY - kPhaseMarkerFlagHeight,
                                phaseX + kPhaseMarkerFlagWidth * 0.5f, topY - kPhaseMarkerFlagHeight,
                                phaseX, topY);
        g.fillPath(markerFlag);
    }
}

float OscillatorCurve::computeShapeValue(double phase01) const noexcept
{
    // Mirrors audio::Oscillator::computeShapeValue's naive (non-PolyBLEP-corrected) formulas - a
    // static preview doesn't need per-sample band-limiting, only the actual DSP does. Must be kept
    // in sync with the DSP if either changes.
    switch (_shapeIndex)
    {
        case 1: // Triangle
            return static_cast<float>(2.0 * std::abs(2.0 * (phase01 - std::floor(phase01 + 0.5))) - 1.0);
        case 2: // Saw
            return static_cast<float>(2.0 * phase01 - 1.0);
        case 3: // Square
            return phase01 < 0.5 ? 1.0f : -1.0f;
        case 0:
        default:
            return static_cast<float>(std::sin(juce::MathConstants<double>::twoPi * phase01));
    }
}

float OscillatorCurve::computeWarpedShapeValue(double outerPhase01) const noexcept
{
    // Mirrors audio::Oscillator::computeWarpedShapeValue - must be kept in sync if either changes.
    const auto warp = juce::jlimit(-1.0f, 1.0f, _warpPercent / 100.0f);
    const auto activeWidth = juce::jmax(kOscillatorCurveMinActiveWidth, 1.0f - std::abs(warp));

    const auto activeStart = warp <= 0.0f ? (1.0 - static_cast<double>(activeWidth)) : 0.0;
    const auto activeEnd = activeStart + static_cast<double>(activeWidth);

    if (outerPhase01 < activeStart || outerPhase01 >= activeEnd)
        return 0.0f;

    const auto shapePhase01 = (outerPhase01 - activeStart) / static_cast<double>(activeWidth);
    return computeShapeValue(shapePhase01);
}

float OscillatorCurve::computeFinalShapeValue(int sampleIndex, double outerPhase01) const noexcept
{
    // Order mirrors audio::Oscillator::getNextSample(): shape+warp -> noise crossfade -> fold.
    auto value = computeWarpedShapeValue(outerPhase01);

    const auto noiseAmount = juce::jlimit(0.0f, 100.0f, _shapeNoisePercent) / 100.0f;
    if (noiseAmount > 0.0f)
    {
        const auto noise = _noiseTable[static_cast<std::size_t>(sampleIndex)];
        value = value * (1.0f - noiseAmount) + noise * noiseAmount;
    }

    const auto foldAmount = juce::jlimit(0.0f, 100.0f, _foldPercent) / 100.0f;
    const auto foldGain = 1.0f + foldAmount * kOscillatorCurveFoldMaxGain;
    return foldSample(value * foldGain);
}

float OscillatorCurve::foldSample(float x) noexcept
{
    // Mirrors audio::Oscillator::foldSample - must be kept in sync if either changes.
    auto wrapped = std::fmod(x + 1.0f, 4.0f);
    if (wrapped < 0.0f)
        wrapped += 4.0f;
    if (wrapped > 2.0f)
        wrapped = 4.0f - wrapped;
    return wrapped - 1.0f;
}

float OscillatorCurve::levelToY(float levelBipolar, float height) noexcept
{
    const auto usableHeight = juce::jmax(0.f, height - kOscillatorCurveVerticalPadding * 2.f);
    const auto level01 = (juce::jlimit(-1.0f, 1.0f, levelBipolar) + 1.0f) * 0.5f;
    return (height - kOscillatorCurveVerticalPadding) - level01 * usableHeight;
}

}
