#pragma once

namespace nierika::dsp
{

class ParameterManager: public juce::AudioProcessorValueTreeState::Listener
{
public:
    ParameterManager(juce::AudioProcessor &processorToConnectTo, std::function<juce::AudioProcessorValueTreeState::ParameterLayout()> layoutBuilder);
    ~ParameterManager() override;

    template<typename T>
    void registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, IParameter::Type type, T defaultValue, T minValue, T maxValue, std::function<void(T)> onChange, const std::string& tooltip = "");
    template<typename T>
    void registerParameter(std::unique_ptr<juce::RangedAudioParameter> parameter, IParameter::Type type, const std::string& displayName, T defaultValue, T minValue, T maxValue, std::function<void(T)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, bool defaultValue, std::function<void(bool)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, float defaultValue, float minValue, float maxValue, std::function<void(float)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, float defaultValue, float minValue, float maxValue, std::function<void(float)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, int defaultValue, int minValue, int maxValue, std::function<void(int)> onChange, const std::string& tooltip = "");
    void registerParameter(const std::string& id, const std::string& parameterName, const std::string& displayName, int defaultValue, int minValue, int maxValue, std::function<void(int)> onChange, const std::string& tooltip = "");

    void clearParameters();

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    static void parameterChanged(const std::shared_ptr<IParameter>& parameter, float newValue);
    void parameterChanged(const std::shared_ptr<IParameter>& parameter);

    virtual void setStateInformation (const void* data, int sizeInBytes);

    virtual void getStateInformation(juce::MemoryBlock& destData);

    juce::AudioProcessorValueTreeState& getState() { return _treeState; }
    
    std::string getParameterName(const std::string& identifier, const std::string& orValue = "") const;
    std::string getParameterDisplayName(const std::string& identifier, const std::string& orValue = "") const;
    std::string getParameterTooltip(const std::string& identifier, const std::string& orValue = "") const;
    template<typename T>
    T getParameterDefaultValue(const std::string& identifier, T orValue) const;
    template<typename T>
    T getParameterMinValue(const std::string& identifier, T orValue) const;
    template<typename T>
    T getParameterMaxValue(const std::string& identifier, T orValue) const;

protected:
    juce::AudioProcessorValueTreeState::ParameterLayout buildParameterLayout();

private:
    std::unordered_map<std::string, std::shared_ptr<IParameter>> _parameterByID;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> _parameters;
    juce::AudioProcessorValueTreeState _treeState;

    std::shared_ptr<IParameter> getParameter(const std::string& identifier) const;
    bool isParameter(const std::string& identifier) const;
};

}
