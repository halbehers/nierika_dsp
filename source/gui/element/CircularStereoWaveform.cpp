#include "../../../include/gui/element/CircularStereoWaveform.h"

#include <algorithm>
#include <cmath>

#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

namespace
{
    constexpr int kTimerHz = 30;
}

CircularStereoWaveform::CircularStereoWaveform(const std::string& identifier, FIFOType* leftFifo, FIFOType* rightFifo):
    Component(identifier),
    _leftFifo(leftFifo),
    _rightFifo(rightFifo)
{
    startTimerHz(kTimerHz);
}

CircularStereoWaveform::~CircularStereoWaveform()
{
    stopTimer();
}

void CircularStereoWaveform::paint(juce::Graphics& g)
{
    Component::paint(g);

    const auto circleBounds = getCircleBounds();

    g.setColour(getBackgroundColour());
    g.fillEllipse(circleBounds);
    g.setColour(getBorderColour());
    g.drawEllipse(circleBounds, 1.5f);

    juce::Path circleClip;
    circleClip.addEllipse(circleBounds);

    juce::Graphics::ScopedSaveState saved(g);
    g.reduceClipRegion(circleClip);

    const auto traceColour = getTraceColour();

    // Each path is already a closed min/max envelope ribbon (see buildWaveformPath) - just fill
    // and outline it directly, semi-transparent so a near-mono signal's overlapping L/R ribbons
    // read as one blended shape and genuine stereo content visibly diverges.
    for (const auto* path : { &_leftPath, &_rightPath })
    {
        g.setColour(traceColour.withAlpha(0.3f));
        g.fillPath(*path);

        g.setColour(traceColour.withAlpha(0.65f));
        g.strokePath(*path, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }
}

void CircularStereoWaveform::timerCallback()
{
    pullIntoHistory(_leftHistory, _leftFifo);
    pullIntoHistory(_rightHistory, _rightFifo);

    const auto circleBounds = getCircleBounds();
    _leftPath = buildWaveformPath(_leftHistory, circleBounds);
    _rightPath = buildWaveformPath(_rightHistory, circleBounds);

    repaint();
}

void CircularStereoWaveform::pullIntoHistory(std::array<float, kHistorySize>& history, FIFOType* fifo)
{
    if (fifo == nullptr)
        return;

    juce::AudioBuffer<float> tempBuffer;

    while (fifo->getNumCompleteBuffersAvailable() > 0)
    {
        if (!fifo->getAudioBuffer(tempBuffer))
            continue;

        // Same rolling shift-then-append technique SpectrumAnalyzer::fillFFTDataGenerator already
        // uses on its own mono buffer, just applied to a plain std::array here.
        const auto size = std::min(tempBuffer.getNumSamples(), kHistorySize);
        std::copy(history.begin() + size, history.end(), history.begin());
        std::copy(tempBuffer.getReadPointer(0), tempBuffer.getReadPointer(0) + size, history.end() - size);
    }
}

juce::Path CircularStereoWaveform::buildWaveformPath(const std::array<float, kHistorySize>& history, juce::Rectangle<float> bounds) const
{
    constexpr auto samplesPerBin = kHistorySize / kPathResolution;

    std::array<float, kPathResolution> upperY {};
    std::array<float, kPathResolution> lowerY {};

    for (int i = 0; i < kPathResolution; ++i)
    {
        // Track the bin's actual max and min (not just the absolute peak) - a single peak-per-bin
        // centreline zigzags wildly once the signal's pitch is high enough that adjacent bins land
        // on opposite half-cycles. Min/max-per-bin instead builds a smooth envelope ribbon, the
        // same technique every DAW's waveform overview uses to stay readable at any pitch.
        auto binMax = 0.0f;
        auto binMin = 0.0f;
        for (int j = 0; j < samplesPerBin; ++j)
        {
            const auto sample = history[static_cast<std::size_t>(i * samplesPerBin + j)];
            binMax = std::max(binMax, sample);
            binMin = std::min(binMin, sample);
        }

        // -1 (left edge) .. 0 (centre) .. +1 (right edge).
        const auto normalizedX = (static_cast<float>(i) / static_cast<float>(kPathResolution - 1)) * 2.0f - 1.0f;
        // Parabola: peaks at centre, unscaled (1.0) at both edges - the magnifying-glass look.
        const auto lensFactor = _magnifyingGlassEnabled ? 1.0f + kMaxMagnification * (1.0f - normalizedX * normalizedX) : 1.0f;

        const auto scaledMax = juce::jlimit(-1.0f, 1.0f, binMax * kAmplitudeScale * lensFactor);
        const auto scaledMin = juce::jlimit(-1.0f, 1.0f, binMin * kAmplitudeScale * lensFactor);

        upperY[static_cast<std::size_t>(i)] = bounds.getCentreY() - scaledMax * bounds.getHeight() * 0.5f;
        lowerY[static_cast<std::size_t>(i)] = bounds.getCentreY() - scaledMin * bounds.getHeight() * 0.5f;
    }

    juce::Path path;
    for (int i = 0; i < kPathResolution; ++i)
    {
        const auto x = bounds.getX() + (static_cast<float>(i) / static_cast<float>(kPathResolution - 1)) * bounds.getWidth();
        if (i == 0)
            path.startNewSubPath(x, upperY[static_cast<std::size_t>(i)]);
        else
            path.lineTo(x, upperY[static_cast<std::size_t>(i)]);
    }
    for (int i = kPathResolution - 1; i >= 0; --i)
    {
        const auto x = bounds.getX() + (static_cast<float>(i) / static_cast<float>(kPathResolution - 1)) * bounds.getWidth();
        path.lineTo(x, lowerY[static_cast<std::size_t>(i)]);
    }
    path.closeSubPath();

    return path;
}

juce::Rectangle<float> CircularStereoWaveform::getCircleBounds()
{
    const auto bounds = getLocalBounds().toFloat();
    const auto diameter = std::min(bounds.getWidth(), bounds.getHeight());
    return bounds.withSizeKeepingCentre(diameter, diameter);
}

}
