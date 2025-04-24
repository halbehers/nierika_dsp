namespace nierika::gui::element
{

Levels::Levels(const dsp::RMSProcessor& rmsProcessor):
    _rmsProcessor(rmsProcessor)
{
    init();
}

Levels::Levels(const std::string& identifier, const dsp::RMSProcessor& rmsProcessor):
    Component(identifier),
    _rmsProcessor(rmsProcessor)
{
    init();
}

void Levels::init()
{
    setName("Levels");
    addAndMakeVisible(_levelMeterLeft);
    addAndMakeVisible(_levelMeterRight);
    startTimerHz(24);
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
}

void Levels::resized()
{
    juce::Grid grid;
    
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    
    grid.templateRows = { Track (Fr (1)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (1)) };
    grid.setGap(juce::Grid::Px(4));
    grid.items = { juce::GridItem(_levelMeterLeft), juce::GridItem(_levelMeterRight) };
    
    grid.performLayout(getLocalBounds());
    Component::resized();
}

}
