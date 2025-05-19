namespace nierika::gui::element
{

TimingComboBox::TimingComboBox(const juce::String& name):
    ComboBox(name)
{
    for (auto timing : dsp::Timing::getAvailableNoteTimings())
    {
        addItem(timing.second, timing.first);
    }
    setSelectedId(dsp::Timing::NoteTiming::NOTE_4);
}

}
