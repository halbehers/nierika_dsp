#pragma once

namespace nierika::gui::layout
{

class GridLayoutItem
{
public:
    GridLayoutItem(const std::string& identifier,
                   int rowPosition,
                   int columnPosition,
                   int width,
                   int height,
                   Alignment alignment
                   );
    
    void setAlignment(Alignment alignment);
    [[nodiscard]] Alignment getAlignment() const;
    void setLeftBorder(juce::Colour color, float thickness = 1.f);
    void setTopBorder(juce::Colour color, float thickness = 1.f);
    void setRightBorder(juce::Colour color, float thickness = 1.f);
    void setBottomBorder(juce::Colour color, float thickness = 1.f);
    void setBorder(juce::Colour color, float thickness = 1.f);
    [[nodiscard]] juce::Line<float> getLeft() const;
    [[nodiscard]] juce::Line<float> getTop() const;
    [[nodiscard]] juce::Line<float> getRight() const;
    [[nodiscard]] juce::Line<float> getBottom() const;
    
    [[nodiscard]] std::string getID() const;
    [[nodiscard]] juce::Rectangle<float> getBounds() const;
    void setBounds(juce::Rectangle<float> bounds);
    [[nodiscard]] int getRowPosition() const;
    void setRowPosition(int rowPosition);
    [[nodiscard]] int getColumnPosition() const;
    void setColumnPosition(int columnPosition);
    [[nodiscard]] int getWidth() const;
    void setWidth(int width);
    [[nodiscard]] int getHeight() const;
    void setHeight(int height);
    [[nodiscard]] Border getLeftBorder() const;
    [[nodiscard]] Border getTopBorder() const;
    [[nodiscard]] Border getRightBorder() const;
    [[nodiscard]] Border getBottomBorder() const;
    [[nodiscard]] bool hasBorders() const;
    [[nodiscard]] float getMinResizableHeight() const;
    void setMinResizableHeight(float minHeight);
    [[nodiscard]] float getMinResizableWidth() const;
    void setMinResizableWidth(float minWidth);
    [[nodiscard]] float getMaxResizableHeight() const;
    void setMaxResizableHeight(float minHeight);
    [[nodiscard]] float getMaxResizableWidth() const;
    void setMaxResizableWidth(float minWidth);
    
    void setMovable(bool isMovable, int movableGroup = 1);
    [[nodiscard]] bool isMovable() const;
    [[nodiscard]] int getMovableGroup() const;
    [[nodiscard]] juce::Rectangle<float> getMovableZone(float height = 20.f) const;
    
    bool operator==(const GridLayoutItem& other) const;

private:
    std::string _identifier;
    juce::Rectangle<float> _bounds {};
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
