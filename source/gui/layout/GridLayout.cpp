#include "../../../include/gui/layout/GridLayout.h"

#include <cmath>

namespace nierika::gui::layout
{

GridLayout::GridLayout(juce::Component& component):
    _component(component)
{
    component.addMouseListener(this, true);
    component.addKeyListener(this);
    component.setWantsKeyboardFocus(true);
}

GridLayout::GridLayout(juce::Component& component, std::vector<int> gridRows, std::vector<int> gridColumns):
    _component(component)
{
    component.addMouseListener(this, true);
    component.addKeyListener(this);
    init(gridRows, gridColumns);
}

GridLayout::~GridLayout()
{
    _component.removeMouseListener(this);
    _component.removeKeyListener(this);
}

void GridLayout::init(std::vector<int> gridRows, std::vector<int> gridColumns)
{
    _gridRatioRows.reserve(gridRows.size());
    _gridRatioColumns.reserve(gridColumns.size());
    
    _gridRatioRows.push_back(0.f);
    _gridRatioColumns.push_back(0.f);
    
    { // Rows
        const float sum = static_cast<float>(std::reduce(gridRows.begin(), gridRows.end()));
        const float ratio = 1.f / sum;
        for (auto& size : gridRows)
            _gridRatioRows.push_back(static_cast<float>(size) * ratio);
    }
    { // Columns
        const float sum = static_cast<float>(std::reduce(gridColumns.begin(), gridColumns.end()));
        const float ratio = 1.f / sum;
        for (auto& size : gridColumns)
            _gridRatioColumns.push_back(static_cast<float>(size) * ratio);
    }

    for (auto x = 1; x < _gridRatioColumns.size(); ++x)
        _gridRatioColumns[x] += _gridRatioColumns[x - 1];
    for (auto y = 1; y < _gridRatioRows.size(); ++y)
        _gridRatioRows[y] += _gridRatioRows[y - 1];
    
    _gridRows = _gridRatioRows;
    _gridColumns = _gridRatioColumns;
    _gridResizedRatioRows = _gridRatioRows;
    _gridResizedRatioColumns = _gridRatioColumns;
}

void GridLayout::resized() noexcept
{
    const juce::Rectangle<float> bounds = _margin.computeBounds(_component.getLocalBounds().toFloat());
    
    _width = bounds.getWidth();
    _height = bounds.getHeight();

    for (int i = 0; i < _gridRows.size(); ++i)
        _gridRows[i] = _gridResizedRatioRows[i] * bounds.getHeight() + bounds.getY();
    for (int i = 0; i < _gridColumns.size(); ++i)
        _gridColumns[i] = _gridResizedRatioColumns[i] * bounds.getWidth() + bounds.getX();
    
    replaceAll();
}

void GridLayout::paint(juce::Graphics& g)
{
    if (_displayGrid)
    {
        g.setColour(nierika::gui::Theme::getInstance().getColor(nierika::gui::Theme::ThemeColor::ACCENT).asJuce());
        for (int i = 0; i < _gridRows.size(); ++i)
            g.drawHorizontalLine(static_cast<int>(_gridRows[i]), _gridColumns[0], _gridColumns[0] + _width);
        for (int i = 0; i < _gridColumns.size(); ++i)
            g.drawVerticalLine(static_cast<int>(_gridColumns[i]), _gridRows[0], _gridRows[0] + _height);
        
        g.drawRect(_component.getLocalBounds());
    }
    
    paintItemDrawables(g);
    paintResizableLines(g);
}

void GridLayout::paintItemDrawables(juce::Graphics& g)
{
    for (auto itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        paintItemBorders(g, item);
        paintItemMovableZones(g, item);
        paintDropableZone(g, item);
    }
}

void GridLayout::paintItemBorders(juce::Graphics& g, const GridLayoutItem& item)
{
    if (!item.hasBorders()) return;

    if (!item.getLeftBorder().isEmpty()) {
        g.setColour(item.getLeftBorder().getColor());
        g.drawLine(item.getLeft(), item.getLeftBorder().getThickness());
    }
    if (!item.getTopBorder().isEmpty()) {
        g.setColour(item.getTopBorder().getColor());
        g.drawLine(item.getTop(), item.getTopBorder().getThickness());
    }
    if (!item.getRightBorder().isEmpty()) {
        g.setColour(item.getRightBorder().getColor());
        g.drawLine(item.getRight(), item.getRightBorder().getThickness());
    }
    if (!item.getBottomBorder().isEmpty()) {
        g.setColour(item.getBottomBorder().getColor());
        g.drawLine(item.getBottom(), item.getBottomBorder().getThickness());
    }
}

void GridLayout::paintItemMovableZones(juce::Graphics& g, const GridLayoutItem& item)
{
    if (!item.isMovable()) return;
    
    g.setColour(_movableConfiguration.handleColor.withAlpha(_movableConfiguration.handleAlpha));
    paintItemMovableZones(g, item.getMovableZone(_movableConfiguration.movableZoneHeight));
}

void GridLayout::paintItemMovableZones(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    paintHandle(g, bounds.getCentre(), _movableConfiguration.handleDotSize);
}

void GridLayout::paintResizableLines(juce::Graphics& g)
{
    for (ResizableLine resizableLine : _resizableLines)
    {
        const bool isHorizontal = resizableLine.direction == ResizableLine::HORIZONTAL;
        const auto lines = isHorizontal ? getHorizontalLines(resizableLine.position) : getVerticalLines(resizableLine.position);

        for (auto line : lines)
        {
            paintResizableLine(g, line);
            paintHandle(g, line, resizableLine.direction == ResizableLine::HORIZONTAL);
        }
    }
}

void GridLayout::paintResizableLine(juce::Graphics& g, juce::Line<float> line)
{
    if (!_resizableLineConfiguration.displayLine) return;
    
    g.setColour(_resizableLineConfiguration.color.withAlpha(_resizableLineConfiguration.lineAlpha));
    
    if (!_resizableLineConfiguration.displayHandle)
    {
        g.drawLine(getLineWithOffset(line, _resizableLineConfiguration.lineOffset), _resizableLineConfiguration.thickness);
        return;
    }

    const auto centerStart = getLineCenter(line, -14);
    const auto centerEnd = getLineCenter(line, 15);
    const auto firstHalfLine = juce::Line<float>(line.getStart(), centerStart);
    const auto secondHalfLine = juce::Line<float>(centerEnd, line.getEnd());

    g.drawLine(getLineWithStartOffset(firstHalfLine, _resizableLineConfiguration.lineOffset), _resizableLineConfiguration.thickness);
    g.drawLine(getLineWithEndOffset(secondHalfLine, _resizableLineConfiguration.lineOffset), _resizableLineConfiguration.thickness);
}

void GridLayout::paintHandle(juce::Graphics& g, juce::Line<float> line, bool isHorizontal)
{
    if (!_resizableLineConfiguration.displayHandle) return;

    const auto center = getLineCenter(line);
    const auto dotSize = _resizableLineConfiguration.handleDotSize;

    g.setColour(_resizableLineConfiguration.color.withAlpha(_resizableLineConfiguration.handleAlpha));
    
    paintHandle(g, center, dotSize, isHorizontal);
}

void GridLayout::paintHandle(juce::Graphics& g, juce::Point<float> center, float dotSize, bool isHorizontal)
{
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; y += 2)
        {
            const auto dotX = (isHorizontal ? center.x : center.y) + x * 5;
            const auto dotY = (isHorizontal ? center.y : center.x) + y * 2 - 1;

            g.fillEllipse(isHorizontal ? dotX : dotY, isHorizontal ? dotY : dotX, dotSize, dotSize);
        }
    }
}

void GridLayout::paintDropableZone(juce::Graphics& g, const juce::Rectangle<float>& bounds, float borderAlpha, float fillAlpha)
{
    const bool mouseIn = bounds.contains(_dropableModeMousePosition);
    const auto color = mouseIn ? _movableConfiguration.dropableOnZoneColor : _movableConfiguration.dropableZoneColor;

    g.setColour(color.withAlpha(fillAlpha));
    g.fillRoundedRectangle(bounds, _movableConfiguration.dropableZoneRadius);
 
    g.setColour(color.withAlpha(borderAlpha));
    g.drawRoundedRectangle(bounds, _movableConfiguration.dropableZoneRadius, 1.f);
}

void GridLayout::paintDropableZone(juce::Graphics& g, const GridLayoutItem& item)
{
    if (!_paintDropableZones || !item.isMovable()) return;
    if (!canBeMovedInto(_itemsById.at(_currentMovableItemId), item)) return;
    if (_currentMovableItemId != item.getID())
    {
        paintDropableZone(g, item.getBounds());
        return;
    }

    const auto dragingZone = juce::Rectangle<float>(_dropableModeMousePosition.x - item.getBounds().getWidth() / 2, _dropableModeMousePosition.y - _movableConfiguration.movableZoneHeight / 2, item.getBounds().getWidth(), item.getBounds().getHeight());
    
    paintDropableZone(g, dragingZone, .3f, .1f);
    
    const auto dragingHeaderZone = juce::Rectangle<float>(dragingZone.getX(), dragingZone.getY(), dragingZone.getWidth(), _movableConfiguration.movableZoneHeight);
    
    g.setColour(_movableConfiguration.dropableOnZoneColor.withAlpha(.7f));
    paintItemMovableZones(g, dragingHeaderZone);
}

juce::Line<float> GridLayout::getHorizontalLine(const int position)
{
    const auto column = getColumn(0);
    const auto row = getRow(position);
    const auto lastColumn = column + _width;

    return juce::Line<float>(column, row, lastColumn, row);
}

std::vector<juce::Line<float>> GridLayout::getHorizontalLines(const int position)
{
    const auto line = getHorizontalLine(position);

    std::vector<juce::Line<float>> lines;
    float lastX = line.getStartX();

    for (auto item : getItemsAcrossLine(position, true))
    {
        const float componentStartX = item.getBounds().getX() - _gap;
        const float componentEndX = item.getBounds().getX() + item.getBounds().getWidth() + _gap;

        if (componentStartX < lastX) {
            lastX = componentEndX;
            continue;
        }

        lines.push_back(juce::Line<float>(lastX, line.getStartY(), componentStartX, line.getEndY()));
        lastX = componentEndX;
    }

    if (lastX < line.getEndX())
        lines.push_back(juce::Line<float>(lastX, line.getStartY(), line.getEndX(), line.getEndY()));

    return lines;
}

std::vector<juce::Line<float>> GridLayout::getVerticalLines(const int position)
{
    const auto line = getVerticalLine(position);

    std::vector<juce::Line<float>> lines;
    float lastY = line.getStartY(); // 0

    for (auto item : getItemsAcrossLine(position, false))
    {
        const float componentStartY = item.getBounds().getY() - (item.getRowPosition() > 0 ? _gap : 0.f);
        const float componentEndY = item.getBounds().getY() + item.getBounds().getHeight() + _gap;
        
        if (componentStartY <= lastY) {
            lastY = componentEndY;
            continue;
        }

        lines.push_back(juce::Line<float>(line.getStartX(), lastY , line.getEndX(), componentStartY));
        lastY = componentEndY;
    }

    if (lastY < line.getEndY())
        lines.push_back(juce::Line<float>(line.getStartX(), lastY, line.getEndX(), line.getEndY()));

    return lines;
}

juce::Line<float> GridLayout::getVerticalLine(const int position)
{
    const auto column = getColumn(position);
    const auto row = getRow(0);
    const auto lastRow = row + _height;

    return juce::Line<float>(column, row, column, lastRow);
}

void GridLayout::replace(const std::string& identifier)
{
    auto item = _itemsById.at(identifier);
    
    replace(item);
}

void GridLayout::replace(GridLayoutItem& item)
{
    const auto bounds = (*this)(item.getRowPosition(), item.getColumnPosition(), item.getWidth(), item.getHeight());

    item.setBounds(bounds);
    _componentsById.at(item.getID()).setBounds(bounds.toNearestInt());
}

void GridLayout::replaceAll()
{
    for (auto& itemsKV : _itemsById)
        replace(itemsKV.second);
}

void GridLayout::addComponent(const std::string& identifier, juce::Component& component, const int rowPosition, const int columnPosition, const int width, const int height)
{
    _component.addAndMakeVisible(component, -1);

    _componentsById.emplace(identifier, component);
    _itemsById.emplace(identifier, GridLayoutItem(identifier, rowPosition, columnPosition, width, height));
}

float GridLayout::getColumn(int position) const
{
    return _gridColumns[position];
}

float GridLayout::getRow(int position) const
{
    return _gridRows[position];
}

juce::Rectangle<float> GridLayout::getBounds(const std::string& identifier)
{
    return _itemsById.at(identifier).getBounds();
}

juce::Line<float> GridLayout::getLeft(const std::string& identifier)
{
    return _itemsById.at(identifier).getLeft();
}

juce::Line<float> GridLayout::getTop(const std::string& identifier)
{
    return _itemsById.at(identifier).getTop();
}

juce::Line<float> GridLayout::getRight(const std::string& identifier)
{
    return _itemsById.at(identifier).getRight();
}

juce::Line<float> GridLayout::getBottom(const std::string& identifier)
{
    return _itemsById.at(identifier).getBottom();
}

void GridLayout::setLeftBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setLeftBorder(color, thickness);
}

void GridLayout::setTopBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setTopBorder(color, thickness);
}

void GridLayout::setRightBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setRightBorder(color, thickness);
}

void GridLayout::setBottomBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setBottomBorder(color, thickness);
}

void GridLayout::setBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setBorder(color, thickness);
}

void GridLayout::setMinResizableHeight(const std::string& identifier, const float minHeight)
{
    _itemsById.at(identifier).setMinResizableHeight(minHeight);
}

void GridLayout::setMinResizableWidth(const std::string& identifier, const float minWidth)
{
    _itemsById.at(identifier).setMinResizableWidth(minWidth);
}

void GridLayout::setMaxResizableHeight(const std::string& identifier, const float maxHeight)
{
    _itemsById.at(identifier).setMaxResizableHeight(maxHeight);
}

void GridLayout::setMaxResizableWidth(const std::string& identifier, const float maxWidth)
{
    _itemsById.at(identifier).setMaxResizableWidth(maxWidth);
}

void GridLayout::setMovable(const std::string& identifier, const bool isMovable, const int movableGroup)
{
    _itemsById.at(identifier).setMovable(isMovable, movableGroup);
}

juce::Rectangle<float> GridLayout::operator()(const int rowPosition, const int columnPosition, const int width, const int height) const
{
    const auto column = getColumn(columnPosition);
    const auto row = getRow(rowPosition);

    const auto bounds = juce::Rectangle<float>(column, row, getColumn(columnPosition + width) - column, getRow(rowPosition + height) - row);

    if (_gap <= 0.f) return bounds;
    
    const float halfGap = _gap / 2.f;

    const auto newX = isLeft(columnPosition) ? bounds.getX() : bounds.getX() + halfGap;
    const auto newY = isTop(rowPosition) ? bounds.getY() : bounds.getY() + halfGap;
    auto newWidth = bounds.getWidth();
    auto newHeight = bounds.getHeight();
    
    if (!isLeft(columnPosition)) newWidth -= halfGap;
    if (!isRight(columnPosition, width)) newWidth -= halfGap;
    
    if (!isTop(rowPosition)) newHeight -= halfGap;
    if (!isBottom(rowPosition, height)) newHeight -= halfGap;
    
    return juce::Rectangle<float>(newX, newY, newWidth, newHeight);
}

bool GridLayout::isTop(const int rowPosition) const
{
    return rowPosition == 0;
}

bool GridLayout::isBottom(const int rowPosition, const int height) const
{
    return rowPosition + height >= _gridRows.size() - 1;
}

bool GridLayout::isLeft(const int columnPosition) const
{
    return columnPosition == 0;
}

bool GridLayout::isRight(const int columnPosition, const int width) const
{
    return columnPosition + width >= _gridColumns.size() - 1;
}

template<typename T>
void GridLayout::setMargin(Margin<T> margin)
{
    _margin = margin.toFloat();
}

template<typename T>
void GridLayout::setMargin(const T marginLeft, const T marginTop, const T marginRight, const T marginBottom)
{
    _margin = Margin<T>(marginLeft, marginTop, marginRight, marginBottom).toFloat();
}

template<typename T>
void GridLayout::setMargin(const T horizontalMargin, const T verticalMargin)
{
    _margin = Margin<T>(horizontalMargin, verticalMargin).toFloat();
}

template<typename T>
void GridLayout::setMargin(const T value)
{
    _margin = Margin<T>(value).toFloat();
}

void GridLayout::setGap(const float gap)
{
    _gap = gap;
}

void GridLayout::setDisplayGrid(bool displayGrid)
{
    _displayGrid = displayGrid;
}

void GridLayout::setResizableLineConfiguration(ResizableLineConfiguration configuration)
{
    _resizableLineConfiguration = configuration;
}

void GridLayout::setMovableConfiguration(MovableConfiguration configuration)
{
    _movableConfiguration = configuration;
}

void GridLayout::setResizableLine(ResizableLine resizableLine)
{
    const auto size = resizableLine.direction == ResizableLine::HORIZONTAL ? _gridRows.size() : _gridColumns.size();

    if (resizableLine.position < 1 || resizableLine.position > size - 2) return;

    _resizableLines.push_back(resizableLine);
}

juce::Point<float> GridLayout::getLineCenter(juce::Line<float> line, float offset)
{
    return line.getPointAlongLine(line.getLength() / 2 + offset);
}

juce::Line<float> GridLayout::getLineWithStartOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getPointAlongLine(offset), line.getEnd());
}

juce::Line<float> GridLayout::getLineWithEndOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getStart(), line.getPointAlongLine(line.getLength() - offset));
}

juce::Line<float> GridLayout::getLineWithOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getPointAlongLine(offset), line.getPointAlongLine(line.getLength() - offset));
}

std::vector<GridLayoutItem> GridLayout::getItemsBelowLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (auto itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        if (item.getRowPosition() == position)
            result.push_back(item);
    }
    return result;
}

std::vector<GridLayoutItem> GridLayout::getItemsAboveLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (auto itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        if (item.getRowPosition() + item.getHeight() == position)
            result.push_back(item);
    }
    return result;
}

std::vector<GridLayoutItem> GridLayout::getItemsRightOfLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (auto itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        if (item.getColumnPosition() == position)
            result.push_back(item);
    }
    return result;
}

std::vector<GridLayoutItem> GridLayout::getItemsLeftOfLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (auto itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        if (item.getColumnPosition() + item.getWidth() == position)
            result.push_back(item);
    }
    return result;
}

std::vector<GridLayoutItem> GridLayout::getItemsAcrossLine(int position, bool isHorizontal)
{
    std::vector<GridLayoutItem> results;

    results.reserve(_itemsById.size());

    for (auto itemKV : _itemsById)
    {
        auto item = itemKV.second;

        const auto itemStartPosition = isHorizontal ? item.getRowPosition() : item.getColumnPosition();
        const auto itemEndPosition = isHorizontal ? item.getRowPosition() + item.getHeight() : item.getColumnPosition() + item.getWidth();

        if (itemStartPosition >= position || itemEndPosition <= position) continue;

        // Crosses the line.

        auto iterator = std::find_if(results.begin(), results.end(), [&isHorizontal, &itemStartPosition] (auto& result)
        {
            const auto resultStartPosition = isHorizontal ? result.getRowPosition() : result.getColumnPosition();
            return resultStartPosition >= itemStartPosition;
        });

        results.insert(iterator, item);
    }

    results.shrink_to_fit();

    return results;
}

std::vector<GridLayoutItem> GridLayout::getPortItemsImpactedByResize(int position, bool isHorizontal)
{
    if (isHorizontal) return getItemsBelowLine(position);

    return getItemsRightOfLine(position);
}


std::vector<GridLayoutItem> GridLayout::getStarboardItemsImpactedByResize(int position, bool isHorizontal)
{
    if (isHorizontal) return getItemsAboveLine(position);

    return getItemsLeftOfLine(position);
}

void GridLayout::mouseMove(const juce::MouseEvent& event)
{
    for (ResizableLine resizableLine : _resizableLines)
    {
        const bool isHorizontal = resizableLine.direction == ResizableLine::HORIZONTAL;
        const auto lines = isHorizontal ? getHorizontalLines(resizableLine.position) : getVerticalLines(resizableLine.position);
        const float padding = 5.f;
        for (auto line : lines)
        {
            const auto width = isHorizontal ? line.getLength() : padding * 2.f;
            const auto height = isHorizontal ? padding * 2.f : line.getLength();
            
            const auto hoverZone = juce::Rectangle<float>(line.getStartX() - padding, line.getStartY() - padding, width, height);
            
            if (hoverZone.contains(event.position)) {
                _currentResizable = resizableLine;
                return;
            }
        }
    }
    
    _currentResizable.clear();

    if (event.mouseWasDraggedSinceMouseDown()) return;

    for (auto itemKV : _itemsById) {
        const auto item = itemKV.second;

        if (!item.isMovable()) continue;
        
        const auto movableZone = item.getMovableZone(_movableConfiguration.movableZoneHeight);
        const auto relativeMovableZone = juce::Rectangle<float>(movableZone.getX() - _margin.left, movableZone.getY() - _margin.top, movableZone.getWidth(), movableZone.getHeight());
        
        const auto relativeMousePosition = juce::Point<float>(event.getScreenPosition().x - _component.getScreenX(), event.getScreenPosition().y - _component.getScreenY());

        if (relativeMovableZone.contains(relativeMousePosition))
        {
            _currentMovableItemId = item.getID();
            _dropableModeMousePosition = relativeMousePosition;
            _component.repaint();
            return;
        }
    }

    _currentMovableItemId = "";
    const bool tmp = _paintDropableZones;
    _paintDropableZones= false;
    if (tmp) _component.repaint();
}

void GridLayout::mouseDrag(const juce::MouseEvent& event)
{
    if (!_currentResizable.isEmpty())
    {
        const bool isHorizontal = _currentResizable.direction == ResizableLine::HORIZONTAL;
        
        const float position = static_cast<float>(isHorizontal ? event.position.getY() : event.position.getX());
        const float length = static_cast<float>(isHorizontal ? _component.getHeight() : _component.getWidth());
        const float ratio = 1.f / (length / position);
        auto& storedRatio = isHorizontal ? _gridResizedRatioRows[_currentResizable.position] : _gridResizedRatioColumns[_currentResizable.position];
        const auto previousRatio = isHorizontal ? _gridResizedRatioRows[_currentResizable.position - 1] : _gridResizedRatioColumns[_currentResizable.position - 1];
        const auto nextRatio = isHorizontal ? _gridResizedRatioRows[_currentResizable.position + 1] : _gridResizedRatioColumns[_currentResizable.position + 1];
        
        const float delta = storedRatio - ratio;
        const float absDelta = std::abs(delta);
        
        if (absDelta > .005f && ratio > previousRatio && ratio < nextRatio)
        {
            storedRatio = ratio;
            _component.resized();
            _component.repaint();
        }
    }

    if (_currentMovableItemId != "")
    {
        _paintDropableZones = true;
        const auto relativeMousePosition = juce::Point<float>(event.getScreenPosition().x - _component.getScreenX(), event.getScreenPosition().y - _component.getScreenY());
        _dropableModeMousePosition = relativeMousePosition;
        _component.repaint();
    }
    else
    {
        const bool tmp = _paintDropableZones;
        _paintDropableZones = false;
        if (tmp) _component.repaint();
    }
}

void GridLayout::mouseUp(const juce::MouseEvent& event)
{
    if (_currentMovableItemId != "")
    {
        for (auto itemKV : _itemsById) {
            const auto item = itemKV.second;
            
            if (!item.isMovable() || item.getID() == _currentMovableItemId) continue;
            
            const auto relativeMovableZone = juce::Rectangle<float>(item.getBounds().getX() - _margin.left, item.getBounds().getY() - _margin.top, item.getBounds().getWidth(), item.getBounds().getHeight());
            
            if (relativeMovableZone.contains(_dropableModeMousePosition))
            {
                auto& itemToSwap = _itemsById.at(item.getID());
                auto& itemToSwapWith = _itemsById.at(_currentMovableItemId);

                if (!canBeMovedInto(itemToSwapWith, itemToSwap)) break;

                const float newItemRowPosition = itemToSwapWith.getRowPosition();
                const float newItemColumnPosition = itemToSwapWith.getColumnPosition();
                const float newItemWidth = itemToSwapWith.getWidth();
                const float newItemHeight = itemToSwapWith.getHeight();

                itemToSwapWith.setRowPosition(itemToSwap.getRowPosition());
                itemToSwapWith.setColumnPosition(itemToSwap.getColumnPosition());
                itemToSwapWith.setWidth(itemToSwap.getWidth());
                itemToSwapWith.setHeight(itemToSwap.getHeight());

                itemToSwap.setRowPosition(newItemRowPosition);
                itemToSwap.setColumnPosition(newItemColumnPosition);
                itemToSwap.setWidth(newItemWidth);
                itemToSwap.setHeight(newItemHeight);
                _component.resized();
                break;
            }
        }
    }

    _currentResizable.clear();
    _currentMovableItemId = "";
    _paintDropableZones= false;
    _component.repaint();
}

void GridLayout::mouseDoubleClick(const juce::MouseEvent& event)
{
    if (_currentResizable.isEmpty()) return;

    const bool isHorizontal = _currentResizable.direction == ResizableLine::HORIZONTAL;
    auto& storedRatio = isHorizontal ? _gridResizedRatioRows[_currentResizable.position] : _gridResizedRatioColumns[_currentResizable.position];

    storedRatio = isHorizontal ? _gridRatioRows[_currentResizable.position] : _gridRatioColumns[_currentResizable.position];
    _component.resized();
    _component.repaint();
}

bool GridLayout::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    if (key == juce::KeyPress::escapeKey)
    {
        _currentResizable.clear();
        _paintDropableZones = false;
        _dropableModeMousePosition = juce::Point<float>();
        _currentMovableItemId = "";
        _component.repaint();
        return true;
    }

    return false;
}

bool GridLayout::canBeMovedInto(const GridLayoutItem& itemToMove, const GridLayoutItem& itemToMoveInto)
{
    if (!itemToMove.isMovable() || !itemToMoveInto.isMovable()) return false;

    return itemToMove.getMovableGroup() == itemToMoveInto.getMovableGroup();
}

template void GridLayout::setMargin(Margin<int> margin);
template void GridLayout::setMargin(Margin<float> margin);
template void GridLayout::setMargin(Margin<double> margin);
template void GridLayout::setMargin(const int marginLeft, const int marginTop, const int marginRight, const int marginBottom);
template void GridLayout::setMargin(const float marginLeft, const float marginTop, const float marginRight, const float marginBottom);
template void GridLayout::setMargin(const double marginLeft, const double marginTop, const double marginRight, const double marginBottom);
template void GridLayout::setMargin(const int horizontalMargin, const int verticalMargin);
template void GridLayout::setMargin(const float horizontalMargin, const float verticalMargin);
template void GridLayout::setMargin(const double horizontalMargin, const double verticalMargin);
template void GridLayout::setMargin(const int value);
template void GridLayout::setMargin(const float value);
template void GridLayout::setMargin(const double value);

}
