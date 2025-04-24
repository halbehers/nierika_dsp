#pragma once

#include "../Border.h"
#include "Alignment.h"

namespace nierika::gui::layout
{

class GridLayoutItem
{
public:
    GridLayoutItem(const std::string& identifier,
                   const int rowPosition,
                   const int columnPosition,
                   const int width,
                   const int height,
                   const Alignment alignment
                   );
    
    void setAlignment(const Alignment alignment);
    Alignment getAlignment() const;
    void setLeftBorder(juce::Colour color, float thickness = 1.f);
    void setTopBorder(juce::Colour color, float thickness = 1.f);
    void setRightBorder(juce::Colour color, float thickness = 1.f);
    void setBottomBorder(juce::Colour color, float thickness = 1.f);
    void setBorder(juce::Colour color, float thickness = 1.f);
    juce::Line<float> getLeft() const;
    juce::Line<float> getTop() const;
    juce::Line<float> getRight() const;
    juce::Line<float> getBottom() const;
    
    std::string getID() const;
    juce::Rectangle<float> getBounds() const;
    void setBounds(juce::Rectangle<float> bounds);
    const int getRowPosition() const;
    void setRowPosition(const int rowPosition);
    const int getColumnPosition() const;
    void setColumnPosition(const int columnPosition);
    const int getWidth() const;
    void setWidth(const int width);
    const int getHeight() const;
    void setHeight(const int height);
    const Border getLeftBorder() const;
    const Border getTopBorder() const;
    const Border getRightBorder() const;
    const Border getBottomBorder() const;
    bool hasBorders() const;
    const float getMinResizableHeight() const;
    void setMinResizableHeight(const float minHeight);
    const float getMinResizableWidth() const;
    void setMinResizableWidth(const float minWidth);
    const float getMaxResizableHeight() const;
    void setMaxResizableHeight(const float minHeight);
    const float getMaxResizableWidth() const;
    void setMaxResizableWidth(const float minWidth);
    
    void setMovable(const bool isMovable, const int movableGroup = 1);
    bool isMovable() const;
    const int getMovableGroup() const;
    juce::Rectangle<float> getMovableZone(const float height = 20.f) const;
    
    bool operator==(const GridLayoutItem& other) const;

private:
    std::string _identifier;
    juce::Rectangle<float> _bounds;
    int _rowPosition;
    int _columnPosition;
    int _width;
    int _height;
    Alignment _alignment;
    Border _leftBorder;
    Border _topBorder;
    Border _rightBorder;
    Border _bottomBorder;
    float _minResizableHeight = -1.f;
    float _minResizableWidth = -1.f;
    float _maxResizableHeight = -1.f;
    float _maxResizableWidth = -1.f;
    
    bool _isMovable = false;
    int _movableGroup = 1;
};

}
