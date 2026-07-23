#include "../../../include/gui/element/DevicesComboBoxWithConfig.h"

#include <functional>
#include <utility>

namespace nierika::gui::element
{

namespace
{
    struct ChannelPair
    {
        juce::String displayName;
        int firstChannelIndex = 0;
        bool hasSecondChannel = false;
        bool ticked = false;
    };

    // Mirrors juce::AudioDeviceSelectorComponent::ChannelSelectorListBox::getNameForChannelPair:
    // finds the longest common leading substring (trimmed back to the last preceding space) and
    // renders "<name1> + <name2 minus common prefix>", e.g. "Line Out 1" + "Line Out 2" -> "Line
    // Out 1 + 2".
    juce::String getNameForChannelPair(const juce::String& name1, const juce::String& name2)
    {
        juce::String commonBit;

        for (int j = 0; j < name1.length(); ++j)
            if (name1.substring(0, j).equalsIgnoreCase(name2.substring(0, j)))
                commonBit = name1.substring(0, j);

        while (commonBit.isNotEmpty() && !juce::CharacterFunctions::isWhitespace(commonBit.getLastCharacter()))
            commonBit = commonBit.dropLastCharacters(1);

        return name1.trim() + " + " + name2.substring(commonBit.length()).trim();
    }

    std::vector<ChannelPair> buildChannelPairs(juce::AudioIODevice& device, const juce::BigInteger& outputChannels)
    {
        std::vector<ChannelPair> pairs;
        const auto names = device.getOutputChannelNames();

        for (int i = 0; i < names.size(); i += 2)
        {
            ChannelPair pair;
            pair.firstChannelIndex = i;

            if (i + 1 >= names.size())
            {
                pair.displayName = names[i].trim();
                pair.hasSecondChannel = false;
                pair.ticked = outputChannels[i];
            }
            else
            {
                pair.displayName = getNameForChannelPair(names[i], names[i + 1]);
                pair.hasSecondChannel = true;
                pair.ticked = outputChannels[i] || outputChannels[i + 1];
            }

            pairs.push_back(std::move(pair));
        }

        return pairs;
    }

    class ChannelPairsListContent: public juce::Component
    {
    public:
        using OnPairChosen = std::function<void(std::size_t)>;

        explicit ChannelPairsListContent(std::vector<ChannelPair> pairs):
            _pairs(std::move(pairs))
        {
            constexpr int width = 220;
            const auto rowHeight = static_cast<int>(Theme::getThinHeight()) + 4;
            setSize(width, rowHeight * juce::jmax(1, static_cast<int>(_pairs.size())));
            setMouseCursor(juce::MouseCursor::PointingHandCursor);
        }

        void setOnPairChosen(OnPairChosen onPairChosen) { _onPairChosen = std::move(onPairChosen); }

        void paint(juce::Graphics& g) override
        {
            const auto rowHeight = getHeight() / juce::jmax(1, static_cast<int>(_pairs.size()));
            constexpr int margin = 12;
            constexpr float checkboxSize = 14.f;

            for (std::size_t i = 0; i < _pairs.size(); ++i)
            {
                const auto rowBounds = juce::Rectangle<int>(0, static_cast<int>(i) * rowHeight, getWidth(), rowHeight).reduced(4);

                const auto checkboxBounds = juce::Rectangle<float>(checkboxSize, checkboxSize)
                    .withPosition(static_cast<float>(rowBounds.getX() + margin), static_cast<float>(rowBounds.getCentreY()) - checkboxSize / 2.f);

                if (_pairs[i].ticked)
                {
                    g.setColour(Theme::newColor(Theme::ThemeColor::ACCENT).asJuce());
                    g.fillRoundedRectangle(checkboxBounds, 3.f);

                    juce::Path check;
                    check.startNewSubPath(checkboxBounds.getX() + checkboxSize * 0.2f, checkboxBounds.getCentreY());
                    check.lineTo(checkboxBounds.getX() + checkboxSize * 0.42f, checkboxBounds.getBottom() - checkboxSize * 0.22f);
                    check.lineTo(checkboxBounds.getX() + checkboxSize * 0.82f, checkboxBounds.getY() + checkboxSize * 0.2f);
                    g.setColour(Theme::newColor(Theme::ThemeColor::INVERTED_TEXT).asJuce());
                    g.strokePath(check, juce::PathStrokeType(1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
                }
                else
                {
                    if (static_cast<int>(i) == _hoveredRow)
                    {
                        g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce());
                        g.fillRoundedRectangle(rowBounds.toFloat(), Theme::getBorderRadius() / 2.f);
                        g.setColour(Theme::newColor(Theme::ThemeColor::SECONDARY_BACKGROUND).asJuce());
                    }
                    else
                        g.setColour(Theme::newColor(Theme::ThemeColor::BORDER).asJuce());

                    g.drawRoundedRectangle(checkboxBounds, 3.f, 1.f);
                }

                g.setColour(Theme::newColor(Theme::ThemeColor::TEXT).asJuce());
                g.setFont(Theme::newFont(Theme::REGULAR, Theme::PARAGRAPH));

                const auto labelBounds = rowBounds.withX(rowBounds.getX() + margin * 2 + static_cast<int>(checkboxSize))
                                                   .withWidth(rowBounds.getWidth() - margin * 3 - static_cast<int>(checkboxSize));
                g.drawFittedText(_pairs[i].displayName, labelBounds, juce::Justification::centredLeft, 1);
            }
        }

        void mouseMove(const juce::MouseEvent& event) override
        {
            updateHoveredRow(event.y);
        }

        void mouseExit(const juce::MouseEvent&) override
        {
            if (_hoveredRow != -1)
            {
                _hoveredRow = -1;
                repaint();
            }
        }

        void mouseUp(const juce::MouseEvent& event) override
        {
            if (_pairs.empty() || !getLocalBounds().contains(event.getPosition()))
                return;

            const auto rowHeight = juce::jmax(1, getHeight() / static_cast<int>(_pairs.size()));
            const auto row = static_cast<std::size_t>(event.y / rowHeight);

            if (row >= _pairs.size())
                return;

            // Exactly one pair can be active (the plugin's output is fixed-stereo) - reflect that
            // immediately so the checkbox visibly flips without waiting for the popup to be
            // rebuilt (it stays open after a selection, so nothing else would repaint it).
            for (auto& pair : _pairs)
                pair.ticked = false;
            _pairs[row].ticked = true;
            repaint();

            if (_onPairChosen)
                _onPairChosen(row);
        }

    private:
        void updateHoveredRow(const int y)
        {
            if (_pairs.empty())
                return;

            const auto rowHeight = juce::jmax(1, getHeight() / static_cast<int>(_pairs.size()));
            const auto row = static_cast<int>(y / rowHeight);
            const auto clampedRow = (row >= 0 && row < static_cast<int>(_pairs.size())) ? row : -1;

            if (clampedRow != _hoveredRow)
            {
                _hoveredRow = clampedRow;
                repaint();
            }
        }

        int _hoveredRow = -1;
        std::vector<ChannelPair> _pairs;
        OnPairChosen _onPairChosen;
    };
}

DevicesComboBoxWithConfig::DevicesComboBoxWithConfig(const std::string& identifier):
    Component(identifier)
{
    addAndMakeVisible(_devicesComboBox);

    addAndMakeVisible(_settingsButton);
    _settingsButton.setIconSize(16.f);
    _settingsButton.setWidth(150);

    _settingsButton.setContentProvider([this]() -> std::unique_ptr<juce::Component>
    {
        if (_deviceManager == nullptr)
            return nullptr;

        auto* device = _deviceManager->getCurrentAudioDevice();
        if (device == nullptr)
            return nullptr;

        auto pairs = buildChannelPairs(*device, _deviceManager->getAudioDeviceSetup().outputChannels);
        if (pairs.empty())
            return nullptr;

        auto content = std::make_unique<ChannelPairsListContent>(pairs);

        content->setOnPairChosen([this, pairs](const std::size_t index)
        {
            if (index >= pairs.size() || _deviceManager == nullptr)
                return;

            const auto& pair = pairs[index];

            auto setup = _deviceManager->getAudioDeviceSetup();
            setup.useDefaultOutputChannels = false;
            setup.outputChannels.clear();
            setup.outputChannels.setBit(pair.firstChannelIndex, true);
            if (pair.hasSecondChannel)
                setup.outputChannels.setBit(pair.firstChannelIndex + 1, true);

            _deviceManager->setAudioDeviceSetup(setup, true);
        });

        return content;
    });
}

void DevicesComboBoxWithConfig::resized()
{
    Component::resized();

    const auto bounds = getLocalBounds();
    const auto buttonWidth = static_cast<int>(SETTINGS_BUTTON_WIDTH);
    const auto gap = static_cast<int>(GAP);

    _devicesComboBox.setBounds(bounds.withWidth(bounds.getWidth() - buttonWidth - gap));
    _settingsButton.setBounds(bounds.withLeft(bounds.getRight() - buttonWidth).withWidth(buttonWidth));
}

void DevicesComboBoxWithConfig::setHeightType(const Theme::HeightType type)
{
    _devicesComboBox.setHeightType(type);
}

}
