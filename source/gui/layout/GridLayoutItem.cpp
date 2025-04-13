#include "../../../include/gui/layout/GridLayoutItem.h"

namespace nierika::gui::layout
{

GridLayoutItem::GridLayoutItem(std::string identifier,
                               const int rowPosition,
                               const int columnPosition,
                               const int width,
                               const int height
                               ):
    _identifier(identifier),
    _rowPosition(rowPosition),
    _columnPosition(columnPosition),
    _width(width),
    _height(height)
{
}

void GridLayoutItem::setLeftBorder(juce::Colour color, float thickness)
{
    _leftBorder = Border(color, thickness);
}

void GridLayoutItem::setTopBorder(juce::Colour color, float thickness)
{
    _topBorder = Border(color, thickness);
}

void GridLayoutItem::setRightBorder(juce::Colour color, float thickness)
{
    _rightBorder = Border(color, thickness);
}

void GridLayoutItem::setBottomBorder(juce::Colour color, float thickness)
{
    _bottomBorder = Border(color, thickness);
}

void GridLayoutItem::setBorder(juce::Colour color, float thickness)
{
    setLeftBorder(color, thickness);
    setTopBorder(color, thickness);
    setRightBorder(color, thickness);
    setBottomBorder(color, thickness);
}

juce::Line<float> GridLayoutItem::getLeft() const
{
    return juce::Line<float>(_bounds.getTopLeft(), _bounds.getBottomLeft());
}

juce::Line<float> GridLayoutItem::getTop() const
{
    return juce::Line<float>(_bounds.getTopLeft(), _bounds.getTopRight());
}

juce::Line<float> GridLayoutItem::getRight() const
{
    return juce::Line<float>(_bounds.getTopRight(), _bounds.getBottomRight());
}

juce::Line<float> GridLayoutItem::getBottom() const
{
    return juce::Line<float>(_bounds.getBottomLeft(), _bounds.getBottomRight());
}

std::string GridLayoutItem::getID() const
{
    return _identifier;
}

juce::Rectangle<float> GridLayoutItem::getBounds() const
{
    return _bounds;
}

void GridLayoutItem::setBounds(juce::Rectangle<float> bounds)
{
    _bounds.setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

const int GridLayoutItem::getRowPosition() const
{
    return _rowPosition;
}

void GridLayoutItem::setRowPosition(const int rowPosition)
{
    _rowPosition = rowPosition;
}

const int GridLayoutItem::getColumnPosition() const
{
    return _columnPosition;
}

void GridLayoutItem::setColumnPosition(const int columnPosition)
{
    _columnPosition = columnPosition;
}

const int GridLayoutItem::getWidth() const
{
    return _width;
}

void GridLayoutItem::setWidth(const int width)
{
    _width = width;
}

const int GridLayoutItem::getHeight() const
{
    return _height;
}

void GridLayoutItem::setHeight(const int height)
{
    _height = height;
}

const Border GridLayoutItem::getLeftBorder() const
{
    return _leftBorder;
}

const Border GridLayoutItem::getTopBorder() const
{
    return _topBorder;
}

const Border GridLayoutItem::getRightBorder() const
{
    return _rightBorder;
}

const Border GridLayoutItem::getBottomBorder() const
{
    return _bottomBorder;
}

bool GridLayoutItem::hasBorders() const
{
    return !_leftBorder.isEmpty() || !_topBorder.isEmpty() || !_rightBorder.isEmpty() || !_bottomBorder.isEmpty();
}

const float GridLayoutItem::getMinResizableHeight() const
{
    return _minResizableHeight;
}

void GridLayoutItem::setMinResizableHeight(const float minHeight)
{
    _minResizableHeight = minHeight;
}

const float GridLayoutItem::getMinResizableWidth() const
{
    return _minResizableWidth;
}

void GridLayoutItem::setMinResizableWidth(const float minWidth)
{
    _minResizableWidth = minWidth;
}

const float GridLayoutItem::getMaxResizableHeight() const
{
    return _maxResizableHeight;
}

void GridLayoutItem::setMaxResizableHeight(const float maxHeight)
{
    _maxResizableHeight = maxHeight;
}

const float GridLayoutItem::getMaxResizableWidth() const
{
    return _maxResizableWidth;
}

void GridLayoutItem::setMaxResizableWidth(const float maxWidth)
{
    _maxResizableWidth = maxWidth;
}

void GridLayoutItem::setMovable(const bool isMovable, const int movableGroup)
{
    _isMovable = isMovable;
    _movableGroup = movableGroup;
}

bool GridLayoutItem::isMovable() const
{
    return _isMovable;
}

const int GridLayoutItem::getMovableGroup() const
{
    return _movableGroup;
}

juce::Rectangle<float> GridLayoutItem::getMovableZone(const float height) const
{
    if (!isMovable()) return juce::Rectangle<float>();
    
    return juce::Rectangle<float>(getBounds().getX(), getBounds().getY(), getBounds().getWidth(), height);
}

bool GridLayoutItem::operator==(const GridLayoutItem& other) const
{
    return _identifier == other.getID();
}

}
