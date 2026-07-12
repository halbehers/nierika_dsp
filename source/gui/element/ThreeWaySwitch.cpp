#include "../../../include/gui/element/ThreeWaySwitch.h"

namespace nierika::gui::element
{

ThreeWaySwitch::ThreeWaySwitch(const std::string& identifier, const std::string& label0, const std::string& label1, const std::string& label2):
    MultiWaySwitch(identifier, { label0, label1, label2 })
{
}

}
