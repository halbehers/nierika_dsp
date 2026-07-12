#include "../../../include/gui/element/TwoWaySwitch.h"

namespace nierika::gui::element
{

TwoWaySwitch::TwoWaySwitch(const std::string& identifier, const std::string& label0, const std::string& label1):
    MultiWaySwitch(identifier, { label0, label1 })
{
}

}
