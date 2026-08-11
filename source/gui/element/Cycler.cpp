#include "../../../include/gui/element/Cycler.h"

#include <algorithm>
#include <cmath>

#include "../../../include/gui/Icons.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Cycler::Cycler(const std::string& identifier, const std::vector<std::string>& choices, int defaultIndex):
    Component(identifier),
    _choices(choices),
    _selectedIndex(choices.empty() ? 0 : juce::jlimit(0, static_cast<int>(choices.size()) - 1, defaultIndex)),
    _leftArrow(identifier + "-left", Icons::getArrowLeft()),
    _rightArrow(identifier + "-right", Icons::getArrowRight()),
    _label(identifier + "-label")
{
    setup();
}

Cycler::Cycler(dsp::ParameterManager& parameterManager, const std::string& parameterID):
    Component(parameterID),
    _leftArrow(parameterID + "-left", Icons::getArrowLeft()),
    _rightArrow(parameterID + "-right", Icons::getArrowRight()),
    _label(parameterID + "-label")
{
    setup();

    if (auto* parameter = parameterManager.getState().getParameter(parameterID))
    {
        if (auto* choiceParameter = dynamic_cast<juce::AudioParameterChoice*>(parameter))
            for (const auto& choice : choiceParameter->choices)
                _choices.push_back(choice.toStdString());

        _attachment = std::make_unique<juce::ParameterAttachment>(*parameter,
            [this](float newValue) { setSelectedIndex(static_cast<int>(std::round(newValue))); });
        _attachment->sendInitialUpdate();
    }
}

Cycler::~Cycler()
{
    _leftArrow.removeListener(this);
    _rightArrow.removeListener(this);
}

void Cycler::setup()
{
    refreshBackgroundAndBorder();

    addAndMakeVisible(_leftArrow);
    addAndMakeVisible(_rightArrow);
    addAndMakeVisible(_label);

    _leftArrow.addOnClickListener(this);
    _leftArrow.setIconSize(12.f);
    _rightArrow.addOnClickListener(this);
    _rightArrow.setIconSize(12.f);

    _label.setJustificationType(juce::Justification::centred);
    _label.setFontSize(Theme::PARAGRAPH);

    refreshLabel();
}

void Cycler::paint(juce::Graphics& g)
{
    Component::paint(g);
}

void Cycler::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    Component::changeListenerCallback(source);

    // refreshBackgroundAndBorder() goes through displayBackground/displayBorder's concrete-Colour
    // overloads (see its own declaration comment), which clear Component's own themeColor
    // tracking - so a tracked background ThemeColor needs re-resolving here first, or it would
    // never see a live theme switch despite Component's machinery existing for exactly that.
    if (_backgroundThemeColorOverride) _backgroundOverride = Theme::newColor(*_backgroundThemeColorOverride).asJuce();

    refreshBackgroundAndBorder();
}

void Cycler::resized()
{
    Component::resized();

    auto bounds = getLocalBounds();
    // Capped, not just bounds.getHeight() - a Cycler placed in a tall row (e.g. alongside round
    // Dials) would otherwise size each arrow to the full row height, leaving little or no room
    // for the label (or the right arrow) in a narrow cell. Also capped to a third of the width so
    // two arrows can never consume the whole cell between them.
    const auto arrowSize = juce::jmin(bounds.getHeight(), 28, bounds.getWidth() / 3);
    _leftArrow.setBounds(bounds.removeFromLeft(arrowSize));
    _rightArrow.setBounds(bounds.removeFromRight(arrowSize));
    _label.setBounds(bounds);
}

void Cycler::setSelectedIndex(int index)
{
    if (_choices.empty())
        return;

    _selectedIndex = juce::jlimit(0, static_cast<int>(_choices.size()) - 1, index);
    refreshLabel();
}

std::string Cycler::getSelectedText() const
{
    return _selectedIndex >= 0 && _selectedIndex < static_cast<int>(_choices.size())
        ? _choices[static_cast<std::size_t>(_selectedIndex)]
        : "";
}

void Cycler::refreshLabel()
{
    _label.setText(getSelectedText());
}

void Cycler::addOnValueChangedListener(OnValueChangedListener* listener)
{
    _listeners.push_back(listener);
}

void Cycler::removeListener(OnValueChangedListener* listener)
{
    _listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}

void Cycler::onButtonClick(const std::string& componentID)
{
    if (_choices.empty())
        return;

    const auto count = static_cast<int>(_choices.size());

    if (componentID == _leftArrow.getID())
        applyIndexFromUI((_selectedIndex - 1 + count) % count);
    else if (componentID == _rightArrow.getID())
        applyIndexFromUI((_selectedIndex + 1) % count);
}

void Cycler::applyIndexFromUI(int newIndex)
{
    setSelectedIndex(newIndex);

    if (_attachment)
        _attachment->setValueAsCompleteGesture(static_cast<float>(_selectedIndex));

    for (auto* listener : _listeners)
        listener->onSelectionChanged(getID(), _selectedIndex);
}

}
