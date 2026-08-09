#include "../../../include/gui/element/OscillatorCurveWithParameters.h"

namespace nierika::gui::element
{

OscillatorCurveWithParameters::OscillatorCurveWithParameters(dsp::ParameterManager& parameterManager, const std::string& identifier,
                                                               const std::string& shapeParameterID, const std::string& warpParameterID,
                                                               const std::string& foldParameterID, const std::string& shapeNoiseParameterID,
                                                               const std::string& phasePercentParameterID, const std::string& phaseRandomizeParameterID):
    Component(identifier),
    _curve(parameterManager, identifier + "-curve", shapeParameterID, warpParameterID, foldParameterID,
           shapeNoiseParameterID, phasePercentParameterID, phaseRandomizeParameterID),
    _phaseSetter(parameterManager, phasePercentParameterID, "%"),
    _noiseSetter(parameterManager, shapeNoiseParameterID, "%"),
    _warpSetter(parameterManager, warpParameterID, "%"),
    _foldSetter(parameterManager, foldParameterID, "%")
{
    _layout.init({ 1, 1 }, { 1, 1, 1, 1, 1, 1 });
    _layout.setFixedRowHeight(1, kValueRowHeight);
    _layout.setFixedColumnWidth(0, kValueParametersPadding);
    _layout.setFixedColumnWidth(5, kValueParametersPadding);
    _layout.setMargin(0.f, 0.f, 0.f, kValueParametersPadding);

    _layout.addComponent(_curve, 0, 0, 6, 1);
    _layout.addComponent(_phaseSetter, 1, 1, 1, 1);
    _layout.addComponent(_noiseSetter, 1, 2, 1, 1);
    _layout.addComponent(_warpSetter, 1, 3, 1, 1);
    _layout.addComponent(_foldSetter, 1, 4, 1, 1);
}

OscillatorCurveWithParameters::~OscillatorCurveWithParameters() = default;

void OscillatorCurveWithParameters::paint(juce::Graphics& g)
{
    Component::paint(g);
    _layout.paint(g);
}

void OscillatorCurveWithParameters::resized()
{
    Component::resized();
    _layout.resized();
}

void OscillatorCurveWithParameters::setValueSettersFontSize(Theme::FontSize size)
{
    _phaseSetter.setFontSize(size);
    _noiseSetter.setFontSize(size);
    _warpSetter.setFontSize(size);
    _foldSetter.setFontSize(size);
}

}
