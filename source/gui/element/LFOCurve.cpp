#include "../../../include/gui/element/LFOCurve.h"

#include <cmath>

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

namespace
{
    constexpr float kLfoCurveBreakpointRadius = 5.0f;
    constexpr float kLfoCurveHitTestRadius = 14.0f;
    constexpr int kLfoCurvePathResolution = 128; // samples across the width, plenty smooth at typical widget sizes
    constexpr float kLfoCurveVerticalPadding = 10.0f; // keeps the peak/trough of the waveform clear of the top/bottom border
}

LFOCurve::LFOCurve(dsp::ParameterManager& parameterManager, const std::string& identifier, const std::string& shapeParameterID, const std::string& smoothParameterID):
    Component(identifier)
{
    displayBackground(Theme::ThemeColor::SECONDARY_BACKGROUND, Theme::getBorderRadius());

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

    _smoothParameter = parameterManager.getState().getParameter(smoothParameterID);
    if (_smoothParameter != nullptr)
    {
        _smoothAttachment = std::make_unique<juce::ParameterAttachment>(*_smoothParameter,
            [this](float newValue)
            {
                _smoothPercent = newValue;
                repaint();
            });
        _smoothAttachment->sendInitialUpdate();
    }
}

LFOCurve::~LFOCurve() = default;

void LFOCurve::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds().toFloat();
    const auto width = bounds.getWidth();

    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.3f));
    g.drawHorizontalLine(static_cast<int>(bounds.getCentreY()), 0.f, width);

    juce::Path path;
    for (int i = 0; i <= kLfoCurvePathResolution; ++i)
    {
        const auto phase01 = static_cast<double>(i) / static_cast<double>(kLfoCurvePathResolution);
        const auto x = (static_cast<float>(i) / static_cast<float>(kLfoCurvePathResolution)) * width;
        const auto y = levelToY(computeShapeValue(phase01));

        if (i == 0)
            path.startNewSubPath(x, y);
        else
            path.lineTo(x, y);
    }

    const auto accent = Theme::newColor(Theme::ThemeColor::ACCENT).asJuce();

    auto fillPath = path;
    fillPath.lineTo(width, bounds.getCentreY());
    fillPath.lineTo(0.f, bounds.getCentreY());
    fillPath.closeSubPath();
    g.setColour(accent.withAlpha(0.15f));
    g.fillPath(fillPath);

    g.setColour(accent);
    g.strokePath(path, juce::PathStrokeType(2.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    const auto dragPoint = computeDragPointPosition();
    g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
    g.fillEllipse(dragPoint.x - kLfoCurveBreakpointRadius, dragPoint.y - kLfoCurveBreakpointRadius, kLfoCurveBreakpointRadius * 2.f, kLfoCurveBreakpointRadius * 2.f);
    g.setColour(accent);
    g.drawEllipse(dragPoint.x - kLfoCurveBreakpointRadius, dragPoint.y - kLfoCurveBreakpointRadius, kLfoCurveBreakpointRadius * 2.f, kLfoCurveBreakpointRadius * 2.f, 1.5f);
}

void LFOCurve::resized()
{
    Component::resized();
}

void LFOCurve::mouseDown(const juce::MouseEvent& event)
{
    _isDraggingSmooth = computeDragPointPosition().getDistanceFrom(event.position) <= kLfoCurveHitTestRadius;
    if (!_isDraggingSmooth)
        return;

    _dragStartMouseY = event.position.y;
    _dragStartSmoothPercent = _smoothPercent;

    if (_smoothAttachment)
        _smoothAttachment->beginGesture();
}

void LFOCurve::mouseDrag(const juce::MouseEvent& event)
{
    if (!_isDraggingSmooth || _smoothAttachment == nullptr || _smoothParameter == nullptr)
        return;

    const auto height = static_cast<float>(getHeight());
    const auto deltaY = event.position.y - _dragStartMouseY;
    // Dragging up increases smooth - height/2 normalizes so a half-height drag spans the full 0-100 range.
    const auto deltaPercent = height > 0.f ? -(deltaY / (height * 0.5f)) * 100.f : 0.f;

    const auto& range = _smoothParameter->getNormalisableRange();
    const auto newSmoothPercent = juce::jlimit(range.start, range.end, _dragStartSmoothPercent + deltaPercent);
    _smoothAttachment->setValueAsPartOfGesture(newSmoothPercent);
}

void LFOCurve::mouseUp(const juce::MouseEvent&)
{
    if (!_isDraggingSmooth)
        return;

    if (_smoothAttachment)
        _smoothAttachment->endGesture();

    _isDraggingSmooth = false;
}

float LFOCurve::computeShapeValue(double phase01) const noexcept
{
    double raw;

    switch (_shapeIndex)
    {
        case 1: // Triangle
            raw = 2.0 * std::abs(2.0 * (phase01 - std::floor(phase01 + 0.5))) - 1.0;
            break;
        case 2: // Saw
            raw = 2.0 * phase01 - 1.0;
            break;
        case 3: // Square
            raw = phase01 < 0.5 ? 1.0 : -1.0;
            break;
        case 0: // Sine
        default:
            raw = std::sin(juce::MathConstants<double>::twoPi * phase01);
            break;
    }

    const auto sine = std::sin(juce::MathConstants<double>::twoPi * phase01);
    const auto smooth = static_cast<double>(juce::jlimit(0.0f, 100.0f, _smoothPercent)) / 100.0;

    return static_cast<float>(raw * (1.0 - smooth) + sine * smooth);
}

juce::Point<float> LFOCurve::computeDragPointPosition() const
{
    const auto width = static_cast<float>(getWidth());
    return { width * 0.5f, levelToY(computeShapeValue(0.5)) };
}

float LFOCurve::levelToY(float levelBipolar) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = juce::jmax(0.f, height - kLfoCurveVerticalPadding * 2.f);
    const auto level01 = (juce::jlimit(-1.0f, 1.0f, levelBipolar) + 1.0f) * 0.5f;
    return (height - kLfoCurveVerticalPadding) - level01 * usableHeight;
}

}
