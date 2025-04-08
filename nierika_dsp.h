#pragma once
#define NIERIKA_MODULES_H_INCLUDED

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 nierikadsp
  vendor:             nierika
  version:            1.0.0
  name:               Nierka's DSP & GUI classes based on JUCE
  description:        Classes for GUI creation and DSP processing.
  website:            http://www.juce.com/juce
  license:            MIT
  minimumCppStandard: 17

  dependencies:       juce_core
  OSXFrameworks:      Accelerate
  iOSFrameworks:      Accelerate

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

// DSP
#include "include/dsp/Channel.h"
#include "include/dsp/CircularAudioBuffer.h"
#include "include/dsp/FFTDataGenerator.h"
#include "include/dsp/FIFO.h"
#include "include/dsp/SingleChannelSampleFIFO.h"
#include "include/dsp/Timing.h"
#include "include/dsp/delay/SimpleDelay.h"
#include "include/dsp/filter/Filter.h"
#include "include/dsp/filter/HighPassFilter.h"
#include "include/dsp/filter/LowPassFilter.h"
#include "include/dsp/sequencer/FXSequencer.h"

// GUI
#include "include/gui/AnalyzerPathGenerator.h"
#include "include/gui/EmbeddedFonts.h"
#include "include/gui/Formatter.h"
#include "include/gui/Helpers.h"
#include "include/gui/Spacing.h"
#include "include/gui/element/BoxToggle.h"
#include "include/gui/element/ComboBox.h"
#include "include/gui/element/Dial.h"
#include "include/gui/element/FrequencyDial.h"
#include "include/gui/element/HorizontalSlider.h"
#include "include/gui/element/IncrementalButton.h"
#include "include/gui/element/IncrementalButtons.h"
#include "include/gui/element/Level.h"
#include "include/gui/element/PercentageDial.h"
#include "include/gui/element/Slider.h"
#include "include/gui/element/Stars.h"
#include "include/gui/element/SVGButton.h"
#include "include/gui/element/SVGToggle.h"
#include "include/gui/element/TimeInMsDial.h"
#include "include/gui/element/TimingComboBox.h"
#include "include/gui/element/TimingDial.h"
#include "include/gui/laf/BoxToggle.h"
#include "include/gui/laf/ComboBox.h"
#include "include/gui/laf/Dial.h"
#include "include/gui/laf/IncrementalButtons.h"
#include "include/gui/laf/SVGButton.h"
#include "include/gui/laf/SVGToggle.h"
