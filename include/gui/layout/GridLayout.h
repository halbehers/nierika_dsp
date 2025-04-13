#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Margin.h"
#include "GridLayoutItem.h"

namespace nierika::gui::layout
{

class GridLayout : juce::MouseListener, juce::KeyListener
{
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
        juce::Colour color = nierika::gui::Theme::getInstance().getColor(nierika::gui::Theme::ThemeColor::EMPTY_SHADE).asJuce();
    };
    
    struct MovableConfiguration
    {
        bool displayHandle = true;
        float handleAlpha = 1.f;
        float handleDotSize = 2.f;
        float movableZoneHeight = 20.f;
        float dropableZoneRadius = 17.f;
        juce::Colour handleColor = nierika::gui::Theme::getInstance().getColor(nierika::gui::Theme::ThemeColor::EMPTY_SHADE).asJuce();
        juce::Colour dropableZoneColor = nierika::gui::Theme::getInstance().getColor(nierika::gui::Theme::ThemeColor::ACCENT).asJuce();
        juce::Colour dropableOnZoneColor = nierika::gui::Theme::getInstance().getColor(nierika::gui::Theme::ThemeColor::WARNING).asJuce();
    };
    
    GridLayout(juce::Component& component);
    GridLayout(juce::Component& component, std::vector<int> gridRows, std::vector<int> gridColumns);
    ~GridLayout();

    void init(std::vector<int> gridRows, std::vector<int> gridColumns);

    juce::Rectangle<float> operator()(const int row, const int column, const int width, const int height) const;
    
    template<typename T>
    void setMargin(Margin<T> margins);
    template<typename T>
    void setMargin(const T marginLeft, const T marginTop, const T marginRight, const T marginBottom);
    template<typename T>
    void setMargin(const T horizontalMargin, const T verticalMargin);
    template<typename T>
    void setMargin(const T value);

    void setGap(const float gap);
    
    void setDisplayGrid(bool displayGrid);
    void setResizableLineConfiguration(ResizableLineConfiguration configuration);
    void setMovableConfiguration(MovableConfiguration configuration);

    void paint(juce::Graphics& g);
    void resized() noexcept;

    void addComponent(const std::string& identifier, juce::Component& component, const int rowPosition, const int columnPosition, const int width, const int height);
    void place(const std::string& label, const int xPosition, const int yPosition, const int width, const int height);
    void replace(const std::string& identifier);
    
    float getColumn(int position) const;
    float getRow(int position) const;
    
    bool isTop(const int rowPosition) const;
    bool isBottom(const int rowPosition, const int height) const;
    bool isLeft(const int columnPosition) const;
    bool isRight(const int columnPosition, const int width) const;

    juce::Rectangle<float> getBounds(const std::string& identifier);
    
    juce::Line<float> getLeft(const std::string& identifier);
    juce::Line<float> getTop(const std::string& identifier);
    juce::Line<float> getRight(const std::string& identifier);
    juce::Line<float> getBottom(const std::string& identifier);

    void setLeftBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setTopBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setRightBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setBottomBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    void setBorder(const std::string& identifier, juce::Colour color, float thickness = 1.f);
    
    void setMinResizableHeight(const std::string& identifier, const float minHeight);
    void setMinResizableWidth(const std::string& identifier, const float minWidth);
    void setMaxResizableHeight(const std::string& identifier, const float minHeight);
    void setMaxResizableWidth(const std::string& identifier, const float minWidth);
    
    void setMovable(const std::string& identifier, const bool isMovable, const int movableGroup = 1);

    struct ResizableLine
    {
        enum Direction
        {
            HORIZONTAL,
            VERTICAL
        };
        
        int position = 0;
        Direction direction = HORIZONTAL;
        float portOffsetRatio = .01f;
        float starboardOffsetRatio = .1f;
        
        void clear() { position = 0; }
        bool isEmpty() const { return position == 0; }
    };
    
    
    void setResizableLine(ResizableLine resizableLine);
    
    juce::Line<float> getHorizontalLine(const int position);
    std::vector<juce::Line<float>> getHorizontalLines(const int position);
    juce::Line<float> getVerticalLine(const int position);
    std::vector<juce::Line<float>> getVerticalLines(const int position);
    
    void mouseMove(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    
    bool keyPressed(const juce::KeyPress& key,
                    juce::Component* originatingComponent) override;


protected:
    juce::Component& _component;

    std::vector<float> _gridRatioRows, _gridRatioColumns;
    std::vector<float> _gridRows, _gridColumns;
    std::vector<float> _gridResizedRatioRows, _gridResizedRatioColumns;
    
    std::vector<ResizableLine> _resizableLines;

    int _columnSum, _rowSum;
    float _width, _height;
    float _rowsRatio = 0.f;
    float _columnsRatio = 0.f;
    
    ResizableLineConfiguration _resizableLineConfiguration;
    MovableConfiguration _movableConfiguration;
    
    Margin<float> _margin { 0.f, 0.f, 0.f, 0.f };
    bool _displayGrid = false;
    float _gap = 0;

    ResizableLine _currentResizable;

    bool _paintDropableZones = false;
    juce::Point<float> _dropableModeMousePosition;
    std::string _currentMovableItemId = "";
    
    std::unordered_map<std::string, GridLayoutItem> _itemsById;
    std::unordered_map<std::string, juce::Component&> _componentsById;
    
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
};

}
