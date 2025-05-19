namespace nierika::gui::layout
{

template<typename T>
GridLayout<T>::GridLayout(T& component):
    _component(component)
{
    component.addMouseListener(this, true);
    component.addKeyListener(this);
    component.setWantsKeyboardFocus(true);
}

template<typename T>
GridLayout<T>::GridLayout(T& component, const std::vector<int> &gridRows, const std::vector<int> &gridColumns):
    _component(component)
{
    component.addMouseListener(this, true);
    component.addKeyListener(this);
    init(gridRows, gridColumns);
}

template<typename T>
GridLayout<T>::~GridLayout()
{
    _component.removeMouseListener(this);
    _component.removeKeyListener(this);
}

template<typename T>
void GridLayout<T>::init(const std::vector<int>& gridRows, const std::vector<int>& gridColumns)
{
    reset();
    _gridRatioRows.reserve(gridRows.size());
    _gridRatioColumns.reserve(gridColumns.size());
    
    _gridRatioRows.push_back(0.f);
    _gridRatioColumns.push_back(0.f);
    
    { // Rows
        const auto sum = static_cast<float>(std::reduce(gridRows.begin(), gridRows.end()));
        const float ratio = 1.f / sum;
        for (auto& size : gridRows)
            _gridRatioRows.push_back(static_cast<float>(size) * ratio);
    }
    { // Columns
        const auto sum = static_cast<float>(std::reduce(gridColumns.begin(), gridColumns.end()));
        const float ratio = 1.f / sum;
        for (auto& size : gridColumns)
            _gridRatioColumns.push_back(static_cast<float>(size) * ratio);
    }

    for (std::size_t x = 1; x < _gridRatioColumns.size(); ++x)
        _gridRatioColumns[x] += _gridRatioColumns[x - 1];
    for (std::size_t y = 1; y < _gridRatioRows.size(); ++y)
        _gridRatioRows[y] += _gridRatioRows[y - 1];
    
    _gridRows = _gridRatioRows;
    _gridColumns = _gridRatioColumns;
    _gridResizedRatioRows = _gridRatioRows;
    _gridResizedRatioColumns = _gridRatioColumns;
}

template<typename T>
void GridLayout<T>::reset()
{
    _gridRatioRows.clear();
    _gridRatioColumns.clear();
    
    _gridRows.clear();
    _gridColumns.clear();
    
    _gridResizedRatioRows.clear();
    _gridResizedRatioColumns.clear();
}

template<typename T>
void GridLayout<T>::resized() noexcept
{
    const juce::Rectangle<float> bounds = _margin.computeBounds(_component.getLocalBounds().toFloat());
    
    _width = bounds.getWidth();
    _height = bounds.getHeight();

    for (std::size_t i = 0; i < _gridRows.size(); ++i)
        _gridRows[i] = _gridResizedRatioRows[i] * bounds.getHeight() + bounds.getY();
    for (std::size_t i = 0; i < _gridColumns.size(); ++i)
        _gridColumns[i] = _gridResizedRatioColumns[i] * bounds.getWidth() + bounds.getX();
    
    replaceAll();
}

template<typename T>
void GridLayout<T>::paint(juce::Graphics& g)
{
    if (!isVisible()) return;

    if (_displayGrid)
    {
        g.setColour(Theme::getInstance().getColor(Theme::ThemeColor::ACCENT).asJuce());
        for (const float gridRow : _gridRows)
            g.drawHorizontalLine(static_cast<int>(gridRow), _gridColumns[0], _gridColumns[0] + _width);
        for (const float gridColumn : _gridColumns)
            g.drawVerticalLine(static_cast<int>(gridColumn), _gridRows[0], _gridRows[0] + _height);
        
        g.drawRect(_component.getLocalBounds());
    }
    
    paintItemDrawables(g);
    paintResizableLines(g);
    
    _component.resized();
}

template<typename T>
void GridLayout<T>::paintItemDrawables(juce::Graphics& g)
{
    for (const auto& itemKV : _itemsById)
    {
        const auto item = itemKV.second;
        
        paintItemBorders(g, item);
        paintItemMovableZones(g, item);
        paintDropableZone(g, item);
    }
}

template<typename T>
void GridLayout<T>::paintItemBorders(juce::Graphics& g, const GridLayoutItem& item)
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

template<typename T>
void GridLayout<T>::paintItemMovableZones(juce::Graphics& g, const GridLayoutItem& item)
{
    if (!item.isMovable()) return;

    g.setColour(_movableConfiguration.handleColor.withAlpha(_movableConfiguration.handleAlpha));
    paintItemMovableZones(g, item.getMovableZone(_movableConfiguration.movableZoneHeight));
}

template<typename T>
void GridLayout<T>::paintItemMovableZones(juce::Graphics& g, const juce::Rectangle<float>& bounds)
{
    if (!_movableConfiguration.displayHandle) return;
    if (!_movableConfiguration.splitHandles)
    {
        paintHandle(g, bounds.getCentre());
        return;
    }

    const auto lines = splitLine(juce::Line<float>(bounds.getX(), bounds.getY() + bounds.getHeight() / 2, bounds.getX() + bounds.getWidth(), bounds.getY() + bounds.getHeight() / 2));
    paintHandle(g, getLineCenter(std::get<0>(lines)));
    paintHandle(g, getLineCenter(std::get<1>(lines)));
}

template<typename T>
void GridLayout<T>::paintResizableLines(juce::Graphics& g)
{
    for (ResizableLine resizableLine : _resizableLines)
    {
        const bool isHorizontal = resizableLine.direction == HORIZONTAL;
        const auto lines = isHorizontal ? getHorizontalLines(resizableLine.position) : getVerticalLines(resizableLine.position);

        for (auto line : lines)
        {
            paintResizableLine(g, line);
            paintHandle(g, line, resizableLine.direction == HORIZONTAL);
        }
    }
}

template<typename T>
void GridLayout<T>::paintResizableLine(juce::Graphics& g, juce::Line<float> line)
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

template<typename T>
void GridLayout<T>::paintHandle(juce::Graphics& g, juce::Line<float> line, bool isHorizontal)
{
    if (!_resizableLineConfiguration.displayHandle) return;

    const auto center = getLineCenter(line);
    const auto dotSize = _resizableLineConfiguration.handleDotSize;

    g.setColour(_resizableLineConfiguration.color.withAlpha(_resizableLineConfiguration.handleAlpha));
    
    paintHandle(g, center, dotSize, isHorizontal);
}

template<typename T>
void GridLayout<T>::paintHandle(juce::Graphics& g, juce::Point<float> center, float dotSize, bool isHorizontal)
{
    for (int x = -3; x <= 3; ++x)
    {
        for (int y = -1; y <= 1; y += 2)
        {
            const auto dotX = (isHorizontal ? center.x : center.y) + x * 5;
            const auto dotY = (isHorizontal ? center.y : center.x) + y * 2 - 1;

            g.fillEllipse(isHorizontal ? dotX : dotY, isHorizontal ? dotY : dotX, dotSize, dotSize);
        }
    }
}

template<typename T>
void GridLayout<T>::paintDropableZone(juce::Graphics& g, const juce::Rectangle<float>& bounds, float borderAlpha, float fillAlpha)
{
    const bool mouseIn = bounds.contains(_dropableModeMousePosition);
    const auto color = mouseIn ? _movableConfiguration.dropableOnZoneColor : _movableConfiguration.dropableZoneColor;

    g.setColour(color.withAlpha(fillAlpha));
    g.fillRoundedRectangle(bounds, _movableConfiguration.dropableZoneRadius);
 
    g.setColour(color.withAlpha(borderAlpha));
    g.drawRoundedRectangle(bounds, _movableConfiguration.dropableZoneRadius, 1.f);
}

template<typename T>
void GridLayout<T>::paintDropableZone(juce::Graphics& g, const GridLayoutItem& item)
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

template<typename T>
juce::Line<float> GridLayout<T>::getHorizontalLine(const int position)
{
    const auto column = getColumn(0);
    const auto row = getRow(position);
    const auto lastColumn = column + _width;

    return juce::Line<float>(column, row, lastColumn, row);
}

template<typename T>
std::vector<juce::Line<float>> GridLayout<T>::getHorizontalLines(const int position)
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

template<typename T>
std::vector<juce::Line<float>> GridLayout<T>::getVerticalLines(const int position)
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

template<typename T>
juce::Line<float> GridLayout<T>::getVerticalLine(const int position)
{
    const auto column = getColumn(position);
    const auto row = getRow(0);
    const auto lastRow = row + _height;

    return juce::Line<float>(column, row, column, lastRow);
}

template<typename T>
void GridLayout<T>::replace(const std::string& identifier)
{
    auto item = _itemsById.at(identifier);
    
    replace(item);
}

template<typename T>
void GridLayout<T>::replace(GridLayoutItem& item)
{
    const auto bounds = (*this)(item.getRowPosition(), item.getColumnPosition(), item.getWidth(), item.getHeight());

    item.setBounds(bounds);
    _componentsById.at(item.getID()).setBounds(bounds.toNearestInt());
}

template<typename T>
void GridLayout<T>::replaceAll()
{
    for (auto& itemsKV : _itemsById)
        replace(itemsKV.second);
}

template<typename T>
void GridLayout<T>::addComponent(const std::string& identifier, juce::Component& component, const int rowPosition, const int columnPosition, const int width, const int height, const Alignment alignment)
{
    _component.addAndMakeVisible(component, -1);

    _componentsById.emplace(identifier, component);
    _itemsById.emplace(identifier, GridLayoutItem(identifier, rowPosition, columnPosition, width, height, alignment));
}

template<typename T>
float GridLayout<T>::getColumn(int position) const
{
    return _gridColumns[static_cast<std::size_t>(position)];
}

template<typename T>
float GridLayout<T>::getRow(int position) const
{
    return _gridRows[static_cast<std::size_t>(position)];
}

template<typename T>
juce::Rectangle<float> GridLayout<T>::getBounds(const std::string& identifier) const {
    return _itemsById.at(identifier).getBounds();
}

template<typename T>
juce::Line<float> GridLayout<T>::getLeft(const std::string& identifier) const {
    return _itemsById.at(identifier).getLeft();
}

template<typename T>
juce::Line<float> GridLayout<T>::getTop(const std::string& identifier) const {
    return _itemsById.at(identifier).getTop();
}

template<typename T>
juce::Line<float> GridLayout<T>::getRight(const std::string& identifier) const {
    return _itemsById.at(identifier).getRight();
}

template<typename T>
juce::Line<float> GridLayout<T>::getBottom(const std::string& identifier) const {
    return _itemsById.at(identifier).getBottom();
}

template<typename T>
void GridLayout<T>::setLeftBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setLeftBorder(color, thickness);
}

template<typename T>
void GridLayout<T>::setTopBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setTopBorder(color, thickness);
}

template<typename T>
void GridLayout<T>::setRightBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setRightBorder(color, thickness);
}

template<typename T>
void GridLayout<T>::setBottomBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setBottomBorder(color, thickness);
}

template<typename T>
void GridLayout<T>::setBorder(const std::string& identifier, juce::Colour color, float thickness)
{
    _itemsById.at(identifier).setBorder(color, thickness);
}

template<typename T>
void GridLayout<T>::setMinResizableHeight(const std::string& identifier, const float minHeight)
{
    _itemsById.at(identifier).setMinResizableHeight(minHeight);
}

template<typename T>
void GridLayout<T>::setMinResizableWidth(const std::string& identifier, const float minWidth)
{
    _itemsById.at(identifier).setMinResizableWidth(minWidth);
}

template<typename T>
void GridLayout<T>::setMaxResizableHeight(const std::string& identifier, const float maxHeight)
{
    _itemsById.at(identifier).setMaxResizableHeight(maxHeight);
}

template<typename T>
void GridLayout<T>::setMaxResizableWidth(const std::string& identifier, const float maxWidth)
{
    _itemsById.at(identifier).setMaxResizableWidth(maxWidth);
}

template<typename T>
void GridLayout<T>::setMovable(const std::string& identifier, const bool isMovable, const int movableGroup)
{
    _itemsById.at(identifier).setMovable(isMovable, movableGroup);
}

template<typename T>
juce::Rectangle<float> GridLayout<T>::operator()(const int rowPosition, const int columnPosition, const int width, const int height) const
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

template<typename T>
bool GridLayout<T>::isTop(const int rowPosition) const
{
    return rowPosition == 0;
}

template<typename T>
bool GridLayout<T>::isBottom(const int rowPosition, const int height) const
{
    return rowPosition + height >= static_cast<int>(_gridRows.size()) - 1;
}

template<typename T>
bool GridLayout<T>::isLeft(const int columnPosition) const
{
    return columnPosition == 0;
}

template<typename T>
bool GridLayout<T>::isRight(const int columnPosition, const int width) const
{
    return columnPosition + width >= static_cast<int>(_gridColumns.size()) - 1;
}

template<typename T>
void GridLayout<T>::setMargin(Spacing<float> margin)
{
    _margin = margin.toFloat();
}

template<typename T>
void GridLayout<T>::setMargin(const float marginLeft, const float marginTop, const float marginRight, const float marginBottom)
{
    _margin = Spacing<float>(marginLeft, marginTop, marginRight, marginBottom);
}

template<typename T>
void GridLayout<T>::setMargin(const float horizontalMargin, const float verticalMargin)
{
    _margin = Spacing<float>(horizontalMargin, verticalMargin);
}

template<typename T>
void GridLayout<T>::setMargin(const float value)
{
    _margin = Spacing<float>(value);
}

template<typename T>
void GridLayout<T>::setGap(const float gap)
{
    _gap = gap;
}

template<typename T>
void GridLayout<T>::setDisplayGrid(bool displayGrid)
{
    _displayGrid = displayGrid;
}

template<typename T>
void GridLayout<T>::setResizableLineConfiguration(ResizableLineConfiguration configuration)
{
    _resizableLineConfiguration = configuration;
}

template<typename T>
void GridLayout<T>::setMovableConfiguration(MovableConfiguration configuration)
{
    _movableConfiguration = configuration;
}

template<typename T>
void GridLayout<T>::setResizableLine(ResizableLine resizableLine)
{
    if (const auto size = static_cast<int>(resizableLine.direction == HORIZONTAL ? _gridRows.size() : _gridColumns.size()); resizableLine.position < 1 || resizableLine.position > size - 2) return;

    _resizableLines.push_back(resizableLine);
}

template<typename T>
juce::Point<float> GridLayout<T>::getLineCenter(juce::Line<float> line, float offset)
{
    return line.getPointAlongLine(line.getLength() / 2 + offset);
}

template<typename T>
std::tuple<juce::Line<float>, juce::Line<float>> GridLayout<T>::splitLine(juce::Line<float> line, float offset)
{
    const auto center = getLineCenter(line, offset);
    const auto firstHalf = juce::Line<float>(line.getStart(), center);
    const auto secondHalf = juce::Line<float>(center, line.getEnd());

    return std::make_tuple(firstHalf, secondHalf);
}

template<typename T>
juce::Line<float> GridLayout<T>::getLineWithStartOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getPointAlongLine(offset), line.getEnd());
}

template<typename T>
juce::Line<float> GridLayout<T>::getLineWithEndOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getStart(), line.getPointAlongLine(line.getLength() - offset));
}

template<typename T>
juce::Line<float> GridLayout<T>::getLineWithOffset(juce::Line<float> line, float offset)
{
    return juce::Line<float>(line.getPointAlongLine(offset), line.getPointAlongLine(line.getLength() - offset));
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getItemsBelowLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (const auto&[_, item] : _itemsById)
    {
        if (item.getRowPosition() == position)
            result.push_back(item);
    }
    return result;
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getItemsAboveLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (const auto&[_, item] : _itemsById)
    {
        if (item.getRowPosition() + item.getHeight() == position)
            result.push_back(item);
    }
    return result;
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getItemsRightOfLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (const auto&[_, item] : _itemsById)
    {
        if (item.getColumnPosition() == position)
            result.push_back(item);
    }
    return result;
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getItemsLeftOfLine(int position)
{
    std::vector<GridLayoutItem> result;

    for (const auto&[_, item] : _itemsById)
    {
        if (item.getColumnPosition() + item.getWidth() == position)
            result.push_back(item);
    }
    return result;
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getItemsAcrossLine(int position, bool isHorizontal)
{
    std::vector<GridLayoutItem> results;

    results.reserve(_itemsById.size());

    for (const auto&[_, item] : _itemsById)
    {
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

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getPortItemsImpactedByResize(int position, bool isHorizontal)
{
    if (isHorizontal) return getItemsBelowLine(position);

    return getItemsRightOfLine(position);
}

template<typename T>
std::vector<GridLayoutItem> GridLayout<T>::getStarboardItemsImpactedByResize(int position, bool isHorizontal)
{
    if (isHorizontal) return getItemsAboveLine(position);

    return getItemsLeftOfLine(position);
}

template<typename T>
void GridLayout<T>::mouseMove(const juce::MouseEvent& event)
{
    for (ResizableLine resizableLine : _resizableLines)
    {
        const bool isHorizontal = resizableLine.direction == HORIZONTAL;
        const auto lines = isHorizontal ? getHorizontalLines(resizableLine.position) : getVerticalLines(resizableLine.position);
        constexpr float padding = 5.f;
        for (auto line : lines)
        {
            const auto width = isHorizontal ? line.getLength() : padding * 2.f;
            const auto height = isHorizontal ? padding * 2.f : line.getLength();

            if (const auto hoverZone = juce::Rectangle<float>(line.getStartX() - padding, line.getStartY() - padding, width, height);
                hoverZone.contains(event.position)) {
                _currentResizable = resizableLine;
                return;
            }
        }
    }
    
    _currentResizable.clear();

    if (event.mouseWasDraggedSinceMouseDown()) return;

    for (const auto&[_, item] : _itemsById)
    {
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

template<typename T>
void GridLayout<T>::mouseDrag(const juce::MouseEvent& event)
{
    if (!_currentResizable.isEmpty())
    {
        const bool isHorizontal = _currentResizable.direction == HORIZONTAL;
        
        const auto position = static_cast<float>(isHorizontal ? event.position.getY() : event.position.getX());
        const auto length = static_cast<float>(isHorizontal ? _component.getHeight() : _component.getWidth());
        const float ratio = 1.f / (length / position);
        auto& storedRatio = isHorizontal ? _gridResizedRatioRows[static_cast<std::size_t>(_currentResizable.position)] : _gridResizedRatioColumns[static_cast<std::size_t>(_currentResizable.position)];
        const auto previousRatio = isHorizontal ? _gridResizedRatioRows[static_cast<std::size_t>(_currentResizable.position - 1)] : _gridResizedRatioColumns[static_cast<std::size_t>(_currentResizable.position - 1)];
        const auto nextRatio = isHorizontal ? _gridResizedRatioRows[static_cast<std::size_t>(_currentResizable.position + 1)] : _gridResizedRatioColumns[static_cast<std::size_t>(_currentResizable.position + 1)];
        
        const float delta = storedRatio - ratio;

        if (const float absDelta = std::abs(delta); absDelta > .005f && ratio > previousRatio && ratio < nextRatio)
        {
            storedRatio = ratio;
            _component.resized();
            _component.repaint();
        }
    }

    if (!_currentMovableItemId.empty())
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

template<typename T>
void GridLayout<T>::mouseUp(const juce::MouseEvent& event)
{
    (void) event;

    if (!_currentMovableItemId.empty())
    {
        for (const auto&[_, item] : _itemsById)
        {
            if (!item.isMovable() || item.getID() == _currentMovableItemId) continue;

            if (const auto relativeMovableZone = juce::Rectangle<float>(item.getBounds().getX() - _margin.left, item.getBounds().getY() - _margin.top, item.getBounds().getWidth(), item.getBounds().getHeight());
                relativeMovableZone.contains(_dropableModeMousePosition))
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

                itemToSwap.setRowPosition(static_cast<int>(newItemRowPosition));
                itemToSwap.setColumnPosition(static_cast<int>(newItemColumnPosition));
                itemToSwap.setWidth(static_cast<int>(newItemWidth));
                itemToSwap.setHeight(static_cast<int>(newItemHeight));
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

template<typename T>
void GridLayout<T>::mouseDoubleClick(const juce::MouseEvent& event)
{
    (void) event;

    if (_currentResizable.isEmpty()) return;

    const bool isHorizontal = _currentResizable.direction == HORIZONTAL;
    auto& storedRatio = isHorizontal ? _gridResizedRatioRows[static_cast<std::size_t>(_currentResizable.position)] : _gridResizedRatioColumns[static_cast<std::size_t>(_currentResizable.position)];

    storedRatio = isHorizontal ? _gridRatioRows[static_cast<std::size_t>(_currentResizable.position)] : _gridRatioColumns[static_cast<std::size_t>(_currentResizable.position)];
    _component.resized();
    _component.repaint();
}

template<typename T>
bool GridLayout<T>::keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent)
{
    (void) originatingComponent;

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

template<typename T>
bool GridLayout<T>::canBeMovedInto(const GridLayoutItem& itemToMove, const GridLayoutItem& itemToMoveInto)
{
    if (!itemToMove.isMovable() || !itemToMoveInto.isMovable()) return false;

    return itemToMove.getMovableGroup() == itemToMoveInto.getMovableGroup();
}

template<typename T>
juce::Line<float> GridLayout<T>::getBottom(const float distanceFromBottom)
{
    return juce::Line<float>(getColumn(0), getRow(static_cast<int>(_gridRows.size()) - 1) + distanceFromBottom, getColumn(static_cast<int>(_gridColumns.size()) - 1), getRow(static_cast<int>(_gridRows.size()) - 1));
}

template<typename T>
juce::Rectangle<float> GridLayout<T>::getRectangleAtBottom(const float height, const float distanceFromBottom)
{
    return juce::Rectangle<float>(getBottom(distanceFromBottom).getStartX(), getBottom(distanceFromBottom).getStartY(), getBottom(distanceFromBottom).getEndX() - getBottom(distanceFromBottom).getStartX() + 1.f, height);
}

template<typename T>
void GridLayout<T>::setVisible(const bool isVisible)
{
    _isVisible = isVisible;
    for (auto& component : _componentsById)
    {
        component.second.setVisible(isVisible);
    }
}

template GridLayout<juce::Component>::GridLayout(juce::Component& component);
template GridLayout<Component>::GridLayout(Component& component);
template GridLayout<juce::Component>::GridLayout(juce::Component& component, const std::vector<int> &gridRows, const std::vector<int> &gridColumns);
template GridLayout<Component>::GridLayout(Component& component, const std::vector<int> &gridRows, const std::vector<int> &gridColumns);

template void GridLayout<juce::Component>::init(const std::vector<int>& gridRows, const std::vector<int>& gridColumns);
template void GridLayout<Component>::init(const std::vector<int>& gridRows, const std::vector<int>& gridColumns);
template juce::Rectangle<float> GridLayout<juce::Component>::operator()(int row, int column, int width, int height) const;
template juce::Rectangle<float> GridLayout<Component>::operator()(int row, int column, int width, int height) const;
template void GridLayout<juce::Component>::setMargin(Spacing<float> margin);
template void GridLayout<Component>::setMargin(Spacing<float> margin);
template void GridLayout<juce::Component>::setMargin(float marginLeft, float marginTop, float marginRight, float marginBottom);
template void GridLayout<Component>::setMargin(float marginLeft, float marginTop, float marginRight, float marginBottom);

template void GridLayout<juce::Component>::setMargin(float horizontalMargin, float verticalMargin);
template void GridLayout<Component>::setMargin(float horizontalMargin, float verticalMargin);
template void GridLayout<juce::Component>::setMargin(float value);
template void GridLayout<Component>::setMargin(float value);
template void GridLayout<juce::Component>::setGap(float gap);
template void GridLayout<Component>::setGap(float gap);
template void GridLayout<juce::Component>::setDisplayGrid(bool displayGrid);
template void GridLayout<Component>::setDisplayGrid(bool displayGrid);
template void GridLayout<juce::Component>::setResizableLineConfiguration(ResizableLineConfiguration configuration);
template void GridLayout<Component>::setResizableLineConfiguration(ResizableLineConfiguration configuration);
template void GridLayout<juce::Component>::setMovableConfiguration(MovableConfiguration configuration);
template void GridLayout<Component>::setMovableConfiguration(MovableConfiguration configuration);
template void GridLayout<juce::Component>::paint(juce::Graphics& g);
template void GridLayout<Component>::paint(juce::Graphics& g);
template void GridLayout<juce::Component>::resized() noexcept;
template void GridLayout<Component>::resized() noexcept;
template void GridLayout<juce::Component>::addComponent(const std::string& identifier, juce::Component& component, int rowPosition, int columnPosition, int width, int height, Alignment alignment);
template void GridLayout<Component>::addComponent(const std::string& identifier, juce::Component& component, int rowPosition, int columnPosition, int width, int height, Alignment alignment);
template void GridLayout<juce::Component>::replace(const std::string& identifier);
template void GridLayout<Component>::replace(const std::string& identifier);
template float GridLayout<juce::Component>::getColumn(int position) const;
template float GridLayout<Component>::getColumn(int position) const;
template float GridLayout<juce::Component>::getRow(int position) const;
template float GridLayout<Component>::getRow(int position) const;
template bool GridLayout<juce::Component>::isTop(int rowPosition) const;
template bool GridLayout<Component>::isTop(int rowPosition) const;
template bool GridLayout<juce::Component>::isBottom(int rowPosition, int height) const;
template bool GridLayout<Component>::isBottom(int rowPosition, int height) const;
template bool GridLayout<juce::Component>::isLeft(int columnPosition) const;
template bool GridLayout<Component>::isLeft(int columnPosition) const;
template bool GridLayout<juce::Component>::isRight(int columnPosition, int width) const;
template bool GridLayout<Component>::isRight(int columnPosition, int width) const;
template juce::Rectangle<float> GridLayout<juce::Component>::getBounds(const std::string& identifier) const;
template juce::Rectangle<float> GridLayout<Component>::getBounds(const std::string& identifier) const;
template juce::Line<float> GridLayout<juce::Component>::getLeft(const std::string& identifier) const;
template juce::Line<float> GridLayout<Component>::getLeft(const std::string& identifier) const;
template juce::Line<float> GridLayout<juce::Component>::getTop(const std::string& identifier) const;
template juce::Line<float> GridLayout<Component>::getTop(const std::string& identifier) const;
template juce::Line<float> GridLayout<juce::Component>::getRight(const std::string& identifier) const;
template juce::Line<float> GridLayout<Component>::getRight(const std::string& identifier) const;
template juce::Line<float> GridLayout<juce::Component>::getBottom(const std::string& identifier) const;
template juce::Line<float> GridLayout<Component>::getBottom(const std::string& identifier) const;
template void GridLayout<juce::Component>::setLeftBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<Component>::setLeftBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<juce::Component>::setTopBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<Component>::setTopBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<juce::Component>::setRightBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<Component>::setRightBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<juce::Component>::setBottomBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<Component>::setBottomBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<juce::Component>::setBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<Component>::setBorder(const std::string& identifier, juce::Colour color, float thickness);
template void GridLayout<juce::Component>::setMinResizableHeight(const std::string& identifier, float minHeight);
template void GridLayout<Component>::setMinResizableHeight(const std::string& identifier, float minHeight);
template void GridLayout<juce::Component>::setMinResizableWidth(const std::string& identifier, float minWidth);
template void GridLayout<Component>::setMinResizableWidth(const std::string& identifier, float minWidth);
template void GridLayout<juce::Component>::setMaxResizableHeight(const std::string& identifier, float maxHeight);
template void GridLayout<Component>::setMaxResizableHeight(const std::string& identifier, float maxHeight);
template void GridLayout<juce::Component>::setMaxResizableWidth(const std::string& identifier, float maxWidth);
template void GridLayout<Component>::setMaxResizableWidth(const std::string& identifier, float maxWidth);
template void GridLayout<juce::Component>::setMovable(const std::string& identifier, bool isMovable, int movableGroup);
template void GridLayout<Component>::setMovable(const std::string& identifier, bool isMovable, int movableGroup);

template void GridLayout<juce::Component>::setResizableLine(ResizableLine resizableLine);
template void GridLayout<Component>::setResizableLine(ResizableLine resizableLine);
template juce::Line<float> GridLayout<juce::Component>::getHorizontalLine(int position);
template juce::Line<float> GridLayout<Component>::getHorizontalLine(int position);
template juce::Line<float> GridLayout<juce::Component>::getVerticalLine(int position);
template juce::Line<float> GridLayout<Component>::getVerticalLine(int position);

template void GridLayout<juce::Component>::mouseMove(const juce::MouseEvent& event);
template void GridLayout<Component>::mouseMove(const juce::MouseEvent& event);
template void GridLayout<juce::Component>::mouseDrag(const juce::MouseEvent& event);
template void GridLayout<Component>::mouseDrag(const juce::MouseEvent& event);
template void GridLayout<juce::Component>::mouseUp(const juce::MouseEvent& event);
template void GridLayout<Component>::mouseUp(const juce::MouseEvent& event);
template void GridLayout<juce::Component>::mouseDoubleClick(const juce::MouseEvent& event);
template void GridLayout<Component>::mouseDoubleClick(const juce::MouseEvent& event);
template bool GridLayout<juce::Component>::keyPressed(const juce::KeyPress& key, juce::Component* originatingComponent);
template juce::Line<float> GridLayout<juce::Component>::getBottom(float distanceFromBottom);
template juce::Line<float> GridLayout<Component>::getBottom(float distanceFromBottom);
template juce::Rectangle<float> GridLayout<juce::Component>::getRectangleAtBottom(float height, float distanceFromBottom);
template juce::Rectangle<float> GridLayout<Component>::getRectangleAtBottom(float height, float distanceFromBottom);

template void GridLayout<juce::Component>::replace(GridLayoutItem& item);
template void GridLayout<Component>::replace(GridLayoutItem& item);
template void GridLayout<juce::Component>::replaceAll();
template void GridLayout<Component>::replaceAll();
template void GridLayout<juce::Component>::paintItemDrawables(juce::Graphics& g);
template void GridLayout<Component>::paintItemDrawables(juce::Graphics& g);
template void GridLayout<juce::Component>::paintItemBorders(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<Component>::paintItemBorders(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<juce::Component>::paintItemMovableZones(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<Component>::paintItemMovableZones(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<juce::Component>::paintItemMovableZones(juce::Graphics& g, const juce::Rectangle<float>& bounds);
template void GridLayout<Component>::paintItemMovableZones(juce::Graphics& g, const juce::Rectangle<float>& bounds);
template void GridLayout<juce::Component>::paintResizableLines(juce::Graphics& g);
template void GridLayout<Component>::paintResizableLines(juce::Graphics& g);
template void GridLayout<juce::Component>::paintResizableLine(juce::Graphics& g, juce::Line<float> line);
template void GridLayout<Component>::paintResizableLine(juce::Graphics& g, juce::Line<float> line);
template void GridLayout<juce::Component>::paintHandle(juce::Graphics& g, juce::Line<float> line, bool isHorizontal);
template void GridLayout<Component>::paintHandle(juce::Graphics& g, juce::Line<float> line, bool isHorizontal);
template void GridLayout<juce::Component>::paintHandle(juce::Graphics& g, juce::Point<float> center, float dotSize, bool isHorizontal);
template void GridLayout<Component>::paintHandle(juce::Graphics& g, juce::Point<float> center, float dotSize, bool isHorizontal);
template void GridLayout<juce::Component>::paintDropableZone(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<Component>::paintDropableZone(juce::Graphics& g, const GridLayoutItem& item);
template void GridLayout<juce::Component>::paintDropableZone(juce::Graphics& g, const juce::Rectangle<float>& bounds, float borderAlpha, float fillAlpha);
template void GridLayout<Component>::paintDropableZone(juce::Graphics& g, const juce::Rectangle<float>& bounds, float borderAlpha, float fillAlpha);
template juce::Point<float> GridLayout<juce::Component>::getLineCenter(juce::Line<float> line, float offset);
template juce::Point<float> GridLayout<Component>::getLineCenter(juce::Line<float> line, float offset);
template std::tuple<juce::Line<float>, juce::Line<float>> GridLayout<juce::Component>::splitLine(juce::Line<float> line, float offset);
template std::tuple<juce::Line<float>, juce::Line<float>> GridLayout<Component>::splitLine(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<juce::Component>::getLineWithStartOffset(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<Component>::getLineWithStartOffset(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<juce::Component>::getLineWithEndOffset(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<Component>::getLineWithEndOffset(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<juce::Component>::getLineWithOffset(juce::Line<float> line, float offset);
template juce::Line<float> GridLayout<Component>::getLineWithOffset(juce::Line<float> line, float offset);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getItemsBelowLine(int position);
template std::vector<GridLayoutItem> GridLayout<Component>::getItemsBelowLine(int position);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getItemsAboveLine(int position);
template std::vector<GridLayoutItem> GridLayout<Component>::getItemsAboveLine(int position);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getItemsRightOfLine(int position);
template std::vector<GridLayoutItem> GridLayout<Component>::getItemsRightOfLine(int position);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getItemsLeftOfLine(int position);
template std::vector<GridLayoutItem> GridLayout<Component>::getItemsLeftOfLine(int position);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getItemsAcrossLine(int position, bool isHorizontal);
template std::vector<GridLayoutItem> GridLayout<Component>::getItemsAcrossLine(int position, bool isHorizontal);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getPortItemsImpactedByResize(int position, bool isHorizontal);
template std::vector<GridLayoutItem> GridLayout<Component>::getPortItemsImpactedByResize(int position, bool isHorizontal);
template std::vector<GridLayoutItem> GridLayout<juce::Component>::getStarboardItemsImpactedByResize(int position, bool isHorizontal);
template std::vector<GridLayoutItem> GridLayout<Component>::getStarboardItemsImpactedByResize(int position, bool isHorizontal);
template bool GridLayout<juce::Component>::canBeMovedInto(const GridLayoutItem& itemToMove, const GridLayoutItem& itemToMoveInto);
template bool GridLayout<Component>::canBeMovedInto(const GridLayoutItem& itemToMove, const GridLayoutItem& itemToMoveInto);

template void GridLayout<juce::Component>::reset();
template void GridLayout<Component>::reset();

}
