#include "../../include/dsp/ParameterManager.h"
#include "../../include/utils/Logger.h"

namespace nierika::dsp
{

ParameterManager::ParameterManager(juce::AudioProcessor &processorToConnectTo, std::function<juce::AudioProcessorValueTreeState::ParameterLayout()> layoutBuilder):
    _treeState(processorToConnectTo, nullptr, "PARAMETERS", layoutBuilder())
{
    
    for (auto it : _parameterByID)
        _treeState.addParameterListener(it.first, this);
}

ParameterManager::~ParameterManager()
{
    clear();
}

juce::AudioProcessorValueTreeState::ParameterLayout ParameterManager::buildParameterLayout()
{
    return { _parameters.begin(), _parameters.end() };
}

template<typename T>
void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> rangeParameter, const T defaultValue, const T minValue, const T maxValue, std::function<void(T)> onChange, const std::string& tooltip)
{
    std::string displayName = rangeParameter->name.toStdString();
    registerParameter<T>(std::move(rangeParameter), displayName, defaultValue, minValue, maxValue, onChange, tooltip);
}

template<typename T>
void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> rangeParameter, const std::string& displayName, const T defaultValue, const T minValue, const T maxValue, std::function<void(T)> onChange, const std::string& tooltip)
{
    const std::string id = rangeParameter->paramID.toStdString();
    const std::string name = rangeParameter->name.toStdString();
    Parameter<T> parameter { id, name, displayName, tooltip, defaultValue, minValue, maxValue, onChange };
    _parameterByID[id] = std::make_shared<Parameter<T>>(parameter);

    _parameters.push_back(std::move(rangeParameter));
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip)
{
    registerParameter<bool>(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { id, 1 }, name, defaultValue), defaultValue, false, true, onChange, tooltip);
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const std::string& displayName, const bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip)
{
    registerParameter<bool>(std::make_unique<juce::AudioParameterBool>(juce::ParameterID { id, 1 }, name, defaultValue), displayName, defaultValue, false, true, onChange, tooltip);
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip)
{
    registerParameter<float>(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { id, 1 }, name, minValue, maxValue, defaultValue), defaultValue, minValue, maxValue, onChange, tooltip);
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const std::string& displayName, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip)
{
    registerParameter<float>(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID { id, 1 }, name, minValue, maxValue, defaultValue), displayName, defaultValue, minValue, maxValue, onChange, tooltip);
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip)
{
    registerParameter<int>(std::make_unique<juce::AudioParameterInt>(juce::ParameterID { id, 1 }, name, minValue, maxValue, defaultValue), defaultValue, minValue, maxValue, onChange, tooltip);
}

void ParameterManager::registerParameter(const std::string& id, const std::string& name, const std::string& displayName, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip)
{
    registerParameter<int>(std::make_unique<juce::AudioParameterInt>(juce::ParameterID { id, 1 }, name, minValue, maxValue, defaultValue), displayName, defaultValue, minValue, maxValue, onChange, tooltip);
}

void ParameterManager::clear()
{
    for (auto parameter : _parameterByID)
        _treeState.removeParameterListener(parameter.first, this);
    _parameterByID.clear();
    _parameters.clear();
}

void ParameterManager::parameterChanged(const juce::String& parameterID, float newValue)
{
    auto it = _parameterByID.find(parameterID.toStdString());
    if (it != _parameterByID.end())
    {
        auto parameter = it->second;
        float newValue = _treeState.getRawParameterValue(it->first)->load();
        auto holder = ParameterValueHolder(newValue);
        parameter->onChangeFromVariant(holder);
    }
}

void ParameterManager::setStateInformation(const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));
    
    if (tree.isValid())
    {
        _treeState.state = tree;
        for (auto it : _parameterByID)
        {
            auto parameter = it.second;
            float newValue = _treeState.getRawParameterValue(it.first)->load();
            auto holder = ParameterValueHolder(newValue);
            parameter->onChangeFromVariant(holder);
        }
    }
}

void ParameterManager::getStateInformation(juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream stream(destData, false);
    _treeState.state.writeToStream(stream);
}

std::shared_ptr<IParameter> ParameterManager::getParameter(const std::string& identifier) const
{
    auto it = _parameterByID.find(identifier);
    if (it != _parameterByID.end())
        return it->second;
    else
    {
        utils::AppLogger::get().error("Parameter not found with id: " + identifier, "ParameterManager::getParameter");
        throw std::runtime_error("Parameter not found");
    }
}

std::string ParameterManager::getParameterName(const std::string& identifier) const
{
    return getParameter(identifier)->getName();
}

std::string ParameterManager::getParameterDisplayName(const std::string& identifier) const
{
    return getParameter(identifier)->getDisplayName();
}

std::string ParameterManager::getParameterTooltip(const std::string& identifier) const
{
    return getParameter(identifier)->getTooltip();
}

template<typename T>
T ParameterManager::getParameterDefaultValue(const std::string& identifier) const
{
    return getParameter(identifier)->getDefaultValue().get<T>();
}

template<typename T>
T ParameterManager::getParameterMinValue(const std::string& identifier) const
{
    return getParameter(identifier)->getMinValue().get<T>();
}

template<typename T>
T ParameterManager::getParameterMaxValue(const std::string& identifier) const
{
    return getParameter(identifier)->getMaxValue().get<T>();
}

template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const bool defaultValue, const bool minValue, const bool maxValue, std::function<void(bool)> onChange, const std::string& tooltip);
template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip);
template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip);
template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const std::string& displayName, const bool defaultValue, const bool minValue, const bool maxValue, std::function<void(bool)> onChange, const std::string& tooltip);
template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const std::string& displayName, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip);
template void ParameterManager::registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const std::string& displayName, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip);

template int ParameterManager::getParameterDefaultValue(const std::string& identifier) const;
template float ParameterManager::getParameterDefaultValue(const std::string& identifier) const;
template bool ParameterManager::getParameterDefaultValue(const std::string& identifier) const;
template int ParameterManager::getParameterMinValue(const std::string& identifier) const;
template float ParameterManager::getParameterMinValue(const std::string& identifier) const;
template int ParameterManager::getParameterMaxValue(const std::string& identifier) const;
template float ParameterManager::getParameterMaxValue(const std::string& identifier) const;

}
