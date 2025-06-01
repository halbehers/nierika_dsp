#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "../Spacing.h"
#include "GridLayoutItem.h"
#include "Alignment.h"

namespace nierika::gui::layout
{

template<class T>
class GridLayout : juce::MouseListener, juce::KeyListener {
public:
    struct ResizableLineConfiguration
    {
        float thickness = 2.f;
        bool displayHandle = true;
        bool displayLine = true;
        float lineAlpha = .2f;
        float handleAlpha = 1.f;
        float handleDotSize = 2.f;
        float lineOffset = 10.f;
        juce::Colour color = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
    };
    
    struct MovableConfiguration
    {
        bool displayHandle = true;
        float handleAlpha = 1.f;
        float handleDotSize = 2.f;
        float movableZoneHeight = 20.f;
        float dropableZoneRadius = 17.f;
        bool splitHandles = false;
        juce::Colour handleColor = Theme::newColor(Theme::ThemeColor::EMPTY_SHADE).asJuce();
        juce::Colour dropableZoneColor = Theme::newColor(Theme::ThemeColor::ACCENT).asJuce();
        juce::Colour dropableOnZoneColor = Theme::newColor(Theme::ThemeColor::WARNING).asJuce();
    };

    struct Listener
    {
        virtual ~Listener() = default;
        virtual void onItemSwaped(const std::string& firstSwapedItemID, const std::string& secondSwapedItemID) = 0;
    };
    
    enum Direction
    {
        HORIZONTAL,
        VERTICAL
    };

    explicit GridLayout(T& component);
    GridLayout(T& component, const std::vector<int> &gridRows, const std::vector<int> &gridColumns);
    
    ~GridLayout() override;

    void init(const std::vector<int>& gridRows, const std::vector<int>& gridColumns);

    juce::Rectangle<float> operator()(int row, int column, int width, int height) const;

    void setMargin(Spacing<float> margin);
    void setMargin(float marginLeft, float marginTop, float marginRight, float marginBottom);
    void setMargin(float horizontalMargin, float verticalMargin);
    void setMargin(float value);

    void setGap(float gap);
    
    void setDisplayGrid(bool displayGrid);
    void setResizableLineConfiguration(ResizableLineConfiguration configuration);
    void setMovableConfiguration(MovableConfiguration configuration);
    MovableConfiguration getMovableConfiguration() { return _movableConfiguration; }

    void paint(juce::Graphics& g);
    void resized() noexcept;

    void addComponent(const std::string& identifier, juce::Component& component, int rowPosition, int columnPosition, int width, int height, int zOrder = -1, Alignment alignment = Alignment::TOP_LEFT);
    void addComponent(Component& component, int rowPosition, int columnPosition, int width, int height, int zOrder = -1, Alignment alignment = Alignment::TOP_LEFT);
    void replace(const std::string& identifier);
    
    [[nodiscard]] float getColumn(int position) const;
    [[nodiscard]] float getRow(int position) const;
    
    [[nodiscard]] bool isTop(int rowPosition) const;
    [[nodiscard]] bool isBottom(int rowPosition, int height) const;
    [[nodiscard]] bool isLeft(int columnPosition) const;
    [[nodiscard]] bool isRight(int columnPosition, int width) const;

    [[nodiscard]] juce::Rectangle<float> getBounds(const std::string& identifier) const;
    
    [[nodiscard]] juce::Line<float> getLeft(const std::string& identifier) const;
    [[nodiscard]] juce::Line<float> getTop(const std::string& identifier) const;
    [[nodiscard]] juce::Line<float> getRight(const std::string& identifier) const;
    [[nodiscard]] juce::Line<float> getBottom(const std::string& identifier) const;

    void setLeftBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setTopBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setRightBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setBottomBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    
    void setMinResizableHeight(const std::string& identifier, float minHeight);
    void setMinResizableWidth(const std::string& identifier, float minWidth);
    void setMaxResizableHeight(const std::string& identifier, float minHeight);
    void setMaxResizableWidth(const std::string& identifier, float minWidth);
    
    void setMovable(const std::string& identifier, bool isMovable, int movableGroup = 1);

    struct ResizableLine
    {   
        int position = 0;
        Direction direction = HORIZONTAL;
        float portOffsetRatio = .01f;
        float starboardOffsetRatio = .1f;
        
        void clear() { position = 0; }
        [[nodiscard]] bool isEmpty() const { return position == 0; }
    };
    
    
    void setResizableLine(ResizableLine resizableLine);
    
    juce::Line<float> getHorizontalLine(int position);
    std::vector<juce::Line<float>> getHorizontalLines(int position);
    juce::Line<float> getVerticalLine(int position);
    std::vector<juce::Line<float>> getVerticalLines(int position);
    
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    
    bool keyPressed(const juce::KeyPress& key,
                    juce::Component* originatingComponent) override;

    juce::Line<float> getBottom(float distanceFromBottom = 0.f);
    juce::Rectangle<float> getRectangleAtBottom(float height, float distanceFromBottom = 0.f);

    void reset();

    void setVisible(bool isVisible);
    [[nodiscard]] bool isVisible() const { return _isVisible; }

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

protected:
    T& _component;

    std::vector<float> _gridRatioRows, _gridRatioColumns;
    std::vector<float> _gridRows, _gridColumns;
    std::vector<float> _gridResizedRatioRows, _gridResizedRatioColumns;

    std::vector<ResizableLine> _resizableLines;

    int _columnSum = 1, _rowSum = 1;
    float _width = 0.f, _height = 0.f;
    float _rowsRatio = 0.f;
    float _columnsRatio = 0.f;
    
    ResizableLineConfiguration _resizableLineConfiguration {};
    MovableConfiguration _movableConfiguration {};
    
    Spacing<float> _margin { 0.f, 0.f, 0.f, 0.f };
    bool _displayGrid = false;
    float _gap = 0;

    bool _isVisible = true;

    ResizableLine _currentResizable {};

    bool _paintDropableZones = false;
    juce::Point<float> _dropableModeMousePosition {};
    std::string _currentMovableItemId;
    
    std::unordered_map<std::string, GridLayoutItem> _itemsById;
    std::unordered_map<std::string, juce::Component&> _componentsById {};

    std::vector<Listener*> _listeners;
    
    void replace(GridLayoutItem& item);
    void replaceAll();
    void paintItemDrawables(juce::Graphics& g);
    void paintItemBorders(juce::Graphics& g, const GridLayoutItem& item);
    void paintItemMovableZones(juce::Graphics& g, const GridLayoutItem& item);
    void paintItemMovableZones(juce::Graphics& g, const juce::Rectangle<float>& bounds);
    void paintResizableLines(juce::Graphics& g);
    void paintResizableLine(juce::Graphics& g, juce::Line<float> line);
    void paintHandle(juce::Graphics& g, juce::Line<float> line, bool isHorizontal = true);
    void paintHandle(juce::Graphics& g, juce::Point<float> center, float dotSize = 2.f, bool isHorizontal = true);
    void paintDropableZone(juce::Graphics& g, const GridLayoutItem& item);
    void paintDropableZone(juce::Graphics& g, const juce::Rectangle<float>& bounds, float borderAlpha = .7f, float fillAlpha = .2f);
    juce::Point<float> getLineCenter(juce::Line<float> line, float offset = 0.f);
    std::tuple<juce::Line<float>, juce::Line<float>> splitLine(juce::Line<float> line, float offset = 0.f);
    juce::Line<float> getLineWithStartOffset(juce::Line<float> line, float offset);
    juce::Line<float> getLineWithEndOffset(juce::Line<float> line, float offset);
    juce::Line<float> getLineWithOffset(juce::Line<float> line, float offset);
    std::vector<GridLayoutItem> getItemsBelowLine(int position);
    std::vector<GridLayoutItem> getItemsAboveLine(int position);
    std::vector<GridLayoutItem> getItemsRightOfLine(int position);
    std::vector<GridLayoutItem> getItemsLeftOfLine(int position);
    std::vector<GridLayoutItem> getItemsAcrossLine(int position, bool isHorizontal);
    std::vector<GridLayoutItem> getPortItemsImpactedByResize(int position, bool isHorizontal);
    std::vector<GridLayoutItem> getStarboardItemsImpactedByResize(int position, bool isHorizontal);
    bool canBeMovedInto(const GridLayoutItem& itemToMove, const GridLayoutItem& itemToMoveInto);

    void notifyListenersOnItemSwaped(const std::string& firstSwapedItemID, const std::string& secondSwapedItemID);
};

}
