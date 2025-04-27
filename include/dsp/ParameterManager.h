#pragma once

#include "Parameter.h"

namespace nierika::dsp
{

class ParameterManager: public juce::AudioProcessorValueTreeState::Listener
{
public:
    ParameterManager(juce::AudioProcessor &processorToConnectTo, std::function<juce::AudioProcessorValueTreeState::ParameterLayout()> layoutBuilder);
    ~ParameterManager();

    template<typename T>
    void registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const T defaultValue, const T minValue, const T maxValue, std::function<void(T)> onChange, const std::string& tooltip = "");
    template<typename T>
    void registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, const std::string& displayName, const T defaultValue, const T minValue, const T maxValue, std::function<void(T)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, const bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, const float defaultValue, const float minValue, const float maxValue, std::function<void(float)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, const int defaultValue, const int minValue, const int maxValue, std::function<void(int)> onChange, const std::string& tooltip = "");

    void clear();

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    void setStateInformation (const void* data, int sizeInBytes);

    void getStateInformation(juce::MemoryBlock& destData);

    juce::AudioProcessorValueTreeState& getState() { return _treeState; }
    
    std::string getParameterName(const std::string& identifier) const;
    std::string getParameterDisplayName(const std::string& identifier) const;
    std::string getParameterTooltip(const std::string& identifier) const;
    template<typename T>
    T getParameterDefaultValue(const std::string& identifier) const;
    template<typename T>
    T getParameterMinValue(const std::string& identifier) const;
    template<typename T>
    T getParameterMaxValue(const std::string& identifier) const;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout buildParameterLayout();

private:
    std::unordered_map<std::string, std::shared_ptr<IParameter>> _parameterByID;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> _parameters;
    juce::AudioProcessorValueTreeState _treeState;

    std::shared_ptr<IParameter> getParameter(const std::string& identifier) const;
};

}
