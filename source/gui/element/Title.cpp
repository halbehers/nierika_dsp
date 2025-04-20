#include "../../../include/gui/element/Title.h"
#include "../../../include/gui/Theme.h"

namespace nierika::gui::element
{

Title::Title(const std::string& title, const bool withGlowEffect):
    Component("title")
{
    addAndMakeVisible(_title);
    const auto textColor = Theme::getInstance().getColor(Theme::TEXT).asJuce();
    if (withGlowEffect)
        _glowEffect.setGlowProperties(6.0, textColor.withAlpha(0.3f));
    else 
        _glowEffect.setGlowProperties(0.0, textColor.withAlpha(0.f));
    _title.setText(title, juce::dontSendNotification);
    _title.setColour(juce::Label::textColourId, textColor);
    _title.setJustificationType(juce::Justification::centred);
    _title.setFont(Theme::getInstance().getFont(Theme::BOLD, Theme::TITLE));
    _title.setComponentEffect(&_glowEffect);
}

Title::~Title()
{
}

void Title::setTitle(const std::string& title, const bool triggerRepaint)
{
    _title.setText(title, juce::dontSendNotification);

    if (triggerRepaint) repaint();
}

std::string Title::getTitle() const
{
    return _title.getText().toStdString();
}

void Title::paint(juce::Graphics& g)
{
    Component::paint(g);
}

void Title::resized()
{
    Component::resized();
    _title.setBounds(getLocalBounds());
}

}
