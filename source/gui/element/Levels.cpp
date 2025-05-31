namespace nierika::gui::element
{

Levels::Levels(const dsp::RMSProcessor& rmsProcessor):
    _rmsProcessor(rmsProcessor),
    _layout(*this)
{
    init();
}

Levels::Levels(const std::string& identifier, const dsp::RMSProcessor& rmsProcessor):
    Component(identifier),
    _rmsProcessor(rmsProcessor),
    _layout(*this)
{
    init();
}

void Levels::init()
{
    setName("Levels");
    startTimerHz(24);

    _layout.init({ 1 }, { 1, 1, 1 });

    _layout.addComponent("left", _levelMeterLeft, 0, 0, 1, 1);
    _layout.addComponent("right", _levelMeterRight, 0, 2, 1, 1);
}

Levels::~Levels()
{
    stopTimer();
}

void Levels::timerCallback()
{
    _levelMeterLeft.setDbLevel(_rmsProcessor.getRMSLevel(dsp::Channel::LEFT));
    _levelMeterLeft.repaint();
    _levelMeterRight.setDbLevel(_rmsProcessor.getRMSLevel(dsp::Channel::RIGHT));
    _levelMeterRight.repaint();
}

void Levels::paint (juce::Graphics& g)
{
    Component::paint(g);
    _layout.paint(g);
}

void Levels::resized()
{
    Component::resized();

    _layout.resized();
}

}
