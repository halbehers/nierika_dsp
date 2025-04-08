#pragma once

#include "../dsp/FIFO.h"

namespace nierika::gui
{

template<typename PathType>
class AnalyzerPathGenerator
{
public:
    void generatePath(const std::vector<float>& renderData,
                      juce::Rectangle<float> fftBounds,
                      int fftSize,
                      float binWidth,
                      float negativeInfinity)
    {
        const float top = fftBounds.getY();
        const auto bottom = fftBounds.getHeight();
        const auto width = fftBounds.getWidth();
        
        int numBins = static_cast<int>(fftSize / 2);
        
        PathType p;
        p.preallocateSpace(3 * static_cast<int>(width));
        
        auto map = [bottom, top, negativeInfinity](float v)
        {
            return juce::jmap(v, negativeInfinity, 0.f, static_cast<float>(bottom), top);
        };
        
        auto y = map(renderData[0]);
        
        jassert(!std::isnan(y) && !std::isinf(y));
        
        p.startNewSubPath(0, y);
        
        const int pathResolution = 1;
        
        for (int binNum = 1; binNum < numBins; binNum += pathResolution)
        {
            y = map(renderData[binNum]);
            
            if (!std::isnan(y) && !std::isinf(y))
            {
                auto binFreq = binNum * binWidth;
                auto normalizedBinX = juce::mapFromLog10(binFreq, 1.f, 20000.f);
                int binX = std::floor(normalizedBinX * width);
                p.lineTo(binX, y);
            }
        }
        
        _pathFifo.push(p);
    }
    
    int getNumPathsAvailable() const
    {
        return _pathFifo.getNumAvailableForReading();
    }
    
    bool getPath(PathType& path)
    {
        return _pathFifo.pull(path);
    }

private:
    dsp::FIFO<PathType> _pathFifo;
};

}
