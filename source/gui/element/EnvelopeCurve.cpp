#include "../../../include/gui/element/EnvelopeCurve.h"

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

namespace
{
    constexpr float kEnvelopeVisibleWindowMs = 4000.0f; // matches the fixed-ruler convention of the reference design
    constexpr float kEnvelopeBreakpointRadius = 5.0f;
    constexpr float kEnvelopeHitTestRadius = 12.0f;
    constexpr float kEnvelopeCurveVerticalPadding = 10.0f; // keeps peak/baseline breakpoint dots clear of the top/bottom border
}

const std::array<EnvelopeCurve::Breakpoint, 5> EnvelopeCurve::kBreakpoints {{
    { StageIndex::Delay, false },
    { StageIndex::Attack, false },
    { StageIndex::Hold, false },
    { StageIndex::Decay, true }, // also drags Sustain (level)
    { StageIndex::Release, false },
}};

EnvelopeCurve::EnvelopeCurve(dsp::ParameterManager& parameterManager, const std::string& identifier, const std::array<std::string, 6>& stageParameterIDs):
    Component(identifier)
{
    displayBackground(Theme::ThemeColor::SECONDARY_BACKGROUND, Theme::getBorderRadius());

    for (std::size_t i = 0; i < kNumStages; ++i)
    {
        auto* parameter = parameterManager.getState().getParameter(stageParameterIDs[i]);
        if (parameter == nullptr)
            continue;

        auto& stage = _stages[i];
        stage.parameter = parameter;
        stage.attachment = std::make_unique<juce::ParameterAttachment>(*parameter,
            [this, i](float newValue)
            {
                _stages[i].currentValue = newValue;
                repaint();
            });
        stage.attachment->sendInitialUpdate();
    }
}

EnvelopeCurve::~EnvelopeCurve() = default;

void EnvelopeCurve::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto bounds = getLocalBounds().toFloat();
    const auto positions = computeBreakpointPositions();

    // Light vertical gridlines every 500ms, matching the reference design's ruler.
    g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce().withAlpha(0.3f));
    for (float ms = 500.0f; ms < kEnvelopeVisibleWindowMs; ms += 500.0f)
    {
        const auto x = msToX(ms);
        g.drawVerticalLine(static_cast<int>(x), 0.f, bounds.getHeight());
    }

    juce::Path path;
    path.startNewSubPath(0.f, levelToY(0.f));
    for (const auto& point : positions)
        path.lineTo(point);

    auto fillPath = path;
    fillPath.lineTo(positions.back().x, levelToY(0.f));
    fillPath.lineTo(0.f, levelToY(0.f));
    fillPath.closeSubPath();

    const auto accent = Theme::newColor(Theme::ThemeColor::ACCENT).asJuce();
    g.setGradientFill(juce::ColourGradient(accent.withAlpha(0.35f), 0.f, 0.f, accent.withAlpha(0.05f), 0.f, bounds.getHeight(), false));
    g.fillPath(fillPath);

    g.setColour(accent);
    g.strokePath(path, juce::PathStrokeType(2.f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    for (const auto& point : positions)
    {
        g.setColour(Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce());
        g.fillEllipse(point.x - kEnvelopeBreakpointRadius, point.y - kEnvelopeBreakpointRadius, kEnvelopeBreakpointRadius * 2.f, kEnvelopeBreakpointRadius * 2.f);
        g.setColour(accent);
        g.drawEllipse(point.x - kEnvelopeBreakpointRadius, point.y - kEnvelopeBreakpointRadius, kEnvelopeBreakpointRadius * 2.f, kEnvelopeBreakpointRadius * 2.f, 1.5f);
    }
}

void EnvelopeCurve::resized()
{
    Component::resized();
}

void EnvelopeCurve::mouseDown(const juce::MouseEvent& event)
{
    _draggedBreakpoint = hitTestBreakpoint(event.position);
    if (_draggedBreakpoint < 0)
        return;

    const auto& breakpoint = kBreakpoints[static_cast<std::size_t>(_draggedBreakpoint)];
    _dragStartMouse = event.position;
    _dragStartTimeMs = _stages[static_cast<std::size_t>(breakpoint.timeStage)].currentValue;

    if (auto* attachment = _stages[static_cast<std::size_t>(breakpoint.timeStage)].attachment.get())
        attachment->beginGesture();

    if (breakpoint.hasLevelDrag)
    {
        _dragStartSustainPercent = _stages[StageIndex::Sustain].currentValue;
        if (auto* sustainAttachment = _stages[StageIndex::Sustain].attachment.get())
            sustainAttachment->beginGesture();
    }
}

void EnvelopeCurve::mouseDrag(const juce::MouseEvent& event)
{
    if (_draggedBreakpoint < 0)
        return;

    const auto& breakpoint = kBreakpoints[static_cast<std::size_t>(_draggedBreakpoint)];
    const auto delta = event.position - _dragStartMouse;

    auto& timeStage = _stages[static_cast<std::size_t>(breakpoint.timeStage)];
    if (timeStage.attachment != nullptr && timeStage.parameter != nullptr)
    {
        const auto& range = timeStage.parameter->getNormalisableRange();
        const auto newTimeMs = juce::jlimit(range.start, range.end, _dragStartTimeMs + xDeltaToMsDelta(delta.x));
        timeStage.attachment->setValueAsPartOfGesture(newTimeMs);
    }

    if (breakpoint.hasLevelDrag)
    {
        auto& sustainStage = _stages[StageIndex::Sustain];
        if (sustainStage.attachment != nullptr && sustainStage.parameter != nullptr)
        {
            const auto& range = sustainStage.parameter->getNormalisableRange();
            const auto newSustainPercent = juce::jlimit(range.start, range.end, _dragStartSustainPercent + yDeltaToLevelDelta(delta.y) * 100.f);
            sustainStage.attachment->setValueAsPartOfGesture(newSustainPercent);
        }
    }
}

void EnvelopeCurve::mouseUp(const juce::MouseEvent&)
{
    if (_draggedBreakpoint < 0)
        return;

    const auto& breakpoint = kBreakpoints[static_cast<std::size_t>(_draggedBreakpoint)];

    if (auto* attachment = _stages[static_cast<std::size_t>(breakpoint.timeStage)].attachment.get())
        attachment->endGesture();

    if (breakpoint.hasLevelDrag)
        if (auto* sustainAttachment = _stages[StageIndex::Sustain].attachment.get())
            sustainAttachment->endGesture();

    _draggedBreakpoint = -1;
}

void EnvelopeCurve::mouseMove(const juce::MouseEvent& event)
{
    setMouseCursor(hitTestBreakpoint(event.position) >= 0 ? juce::MouseCursor::DraggingHandCursor : juce::MouseCursor::NormalCursor);
}

float EnvelopeCurve::msToX(float cumulativeMs) const noexcept
{
    const auto width = static_cast<float>(getWidth());
    return juce::jlimit(0.f, width, (cumulativeMs / kEnvelopeVisibleWindowMs) * width);
}

float EnvelopeCurve::xDeltaToMsDelta(float xDelta) const noexcept
{
    const auto width = static_cast<float>(getWidth());
    return width > 0.f ? (xDelta / width) * kEnvelopeVisibleWindowMs : 0.f;
}

float EnvelopeCurve::levelToY(float level01) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = juce::jmax(0.f, height - kEnvelopeCurveVerticalPadding * 2.f);
    return (height - kEnvelopeCurveVerticalPadding) - juce::jlimit(0.f, 1.f, level01) * usableHeight;
}

float EnvelopeCurve::yDeltaToLevelDelta(float yDelta) const noexcept
{
    const auto height = static_cast<float>(getHeight());
    const auto usableHeight = juce::jmax(0.f, height - kEnvelopeCurveVerticalPadding * 2.f);
    return usableHeight > 0.f ? -(yDelta / usableHeight) : 0.f;
}

std::array<juce::Point<float>, 5> EnvelopeCurve::computeBreakpointPositions() const
{
    const auto delayMs = _stages[StageIndex::Delay].currentValue;
    const auto attackMs = _stages[StageIndex::Attack].currentValue;
    const auto holdMs = _stages[StageIndex::Hold].currentValue;
    const auto decayMs = _stages[StageIndex::Decay].currentValue;
    const auto sustainLevel01 = _stages[StageIndex::Sustain].currentValue / 100.f;
    const auto releaseMs = _stages[StageIndex::Release].currentValue;

    const auto delayEndMs = delayMs;
    const auto attackEndMs = delayEndMs + attackMs;
    const auto holdEndMs = attackEndMs + holdMs;
    const auto decayEndMs = holdEndMs + decayMs;
    const auto releaseEndMs = decayEndMs + releaseMs;

    return {
        juce::Point<float>(msToX(delayEndMs), levelToY(0.f)),
        juce::Point<float>(msToX(attackEndMs), levelToY(1.f)),
        juce::Point<float>(msToX(holdEndMs), levelToY(1.f)),
        juce::Point<float>(msToX(decayEndMs), levelToY(sustainLevel01)),
        juce::Point<float>(msToX(releaseEndMs), levelToY(0.f)),
    };
}

int EnvelopeCurve::hitTestBreakpoint(juce::Point<float> position) const
{
    const auto positions = computeBreakpointPositions();

    auto closestIndex = -1;
    auto closestDistance = kEnvelopeHitTestRadius;

    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        const auto distance = positions[i].getDistanceFrom(position);
        if (distance <= closestDistance)
        {
            closestDistance = distance;
            closestIndex = static_cast<int>(i);
        }
    }

    return closestIndex;
}

}
