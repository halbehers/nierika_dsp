#include "../../../includes/gui/element/TimingComboBox.h"
#include "../../../includes/dsp/Timing.h"

namespace ui::element
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

TimingComboBox::~TimingComboBox()
{
}

}
