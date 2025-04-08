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
#include "includes/dsp/Channel.h"
#include "includes/dsp/CircularAudioBuffer.h"
#include "includes/dsp/FFTDataGenerator.h"
#include "includes/dsp/FIFO.h"
#include "includes/dsp/SingleChannelSampleFIFO.h"
#include "includes/dsp/Timing.h"
#include "includes/dsp/delay/SimpleDelay.h"
#include "includes/dsp/filter/Filter.h"
#include "includes/dsp/filter/HighPassFilter.h"
#include "includes/dsp/filter/LowPassFilter.h"
#include "includes/dsp/sequencer/FXSequencer.h"

// GUI
#include "includes/gui/AnalyzerPathGenerator.h"
#include "includes/gui/EmbeddedFonts.h"
#include "includes/gui/Formatter.h"
#include "includes/gui/Helpers.h"
#include "includes/gui/Spacing.h"
#include "includes/gui/element/BoxToggle.h"
#include "includes/gui/element/ComboBox.h"
#include "includes/gui/element/Dial.h"
#include "includes/gui/element/FrequencyDial.h"
#include "includes/gui/element/HorizontalSlider.h"
#include "includes/gui/element/IncrementalButton.h"
#include "includes/gui/element/IncrementalButtons.h"
#include "includes/gui/element/Level.h"
#include "includes/gui/element/PercentageDial.h"
#include "includes/gui/element/Slider.h"
#include "includes/gui/element/Stars.h"
#include "includes/gui/element/SVGButton.h"
#include "includes/gui/element/SVGToggle.h"
#include "includes/gui/element/TimeInMsDial.h"
#include "includes/gui/element/TimingComboBox.h"
#include "includes/gui/element/TimingDial.h"
#include "includes/gui/laf/BoxToggle.h"
#include "includes/gui/laf/ComboBox.h"
#include "includes/gui/laf/Dial.h"
#include "includes/gui/laf/IncrementalButtons.h"
#include "includes/gui/laf/SVGButton.h"
#include "includes/gui/laf/SVGToggle.h"
