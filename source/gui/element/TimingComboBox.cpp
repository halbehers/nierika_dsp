#include "../../../include/gui/element/TimingComboBox.h"
#include "../../../include/dsp/Timing.h"

namespace nierika::gui::element
{

TimingComboBox::TimingComboBox(const std::string& identifier):
    ComboBox(identifier)
{
    for (auto timing : dsp::Timing::getAvailableNoteTimings())
    {
        addItem(timing.second, timing.first);
    }
    setSelectedId(dsp::Timing::NoteTiming::NOTE_4);
}

}
