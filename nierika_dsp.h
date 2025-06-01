/*******************************************************************************

 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 nierika_dsp
  vendor:             Nierika
  version:            0.1.0
  name:               Nierika DSP
  description:        Classes for GUI creation and DSP processing.
  website:            http://www.juce.com/juce
  license:            MIT
  minimumCppStandard: 20

  dependencies:       juce_core juce_data_structures juce_events juce_dsp juce_graphics juce_gui_basics juce_gui_extra juce_audio_basics juce_audio_formats juce_audio_utils juce_audio_processors

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

#pragma once
#define NIERIKA_MODULES_H_INCLUDED

// JUCE
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include "include/NierikaDSPBinaryData.h"

// DSP
#include "include/dsp/Channel.h"
#include "include/dsp/CircularAudioBuffer.h"
#include "include/dsp/FFTDataGenerator.h"
#include "include/dsp/FIFO.h"
#include "include/dsp/SingleChannelSampleFIFO.h"
#include "include/dsp/Timing.h"
#include "include/dsp/sequencer/FXSequencer.h"
#include "include/dsp/RMSProcessor.h"
#include "include/dsp/ParameterManager.h"
#include "include/dsp/Parameter.h"
#include "include/dsp/DryWetUtils.h"

// GUI
#include "include/gui/AnalyzerPathGenerator.h"
#include "include/gui/EmbeddedFonts.h"
#include "include/gui/Formatter.h"
#include "include/gui/Helpers.h"
#include "include/gui/Icons.h"
#include "include/gui/Section.h"
#include "include/gui/Spacing.h"
#include "include/gui/Theme.h"
#include "include/gui/Component.h"
#include "include/gui/TooltipManager.h"
#include "include/gui/element/BoxToggle.h"
#include "include/gui/element/ComboBox.h"
#include "include/gui/element/Dial.h"
#include "include/gui/element/FrequencyDial.h"
#include "include/gui/element/HorizontalSlider.h"
#include "include/gui/element/IncrementalButton.h"
#include "include/gui/element/IncrementalButtons.h"
#include "include/gui/element/Level.h"
#include "include/gui/element/Levels.h"
#include "include/gui/element/PercentageDial.h"
#include "include/gui/element/SpectrumAnalyzer.h"
#include "include/gui/element/Slider.h"
#include "include/gui/element/Stars.h"
#include "include/gui/element/SVGButton.h"
#include "include/gui/element/SVGToggle.h"
#include "include/gui/element/TimeInMsDial.h"
#include "include/gui/element/TimingComboBox.h"
#include "include/gui/element/TimingDial.h"
#include "include/gui/element/DJFilterDial.h"
#include "include/gui/element/Tooltip.h"
#include "include/gui/element/Title.h"
#include "include/gui/element/Text.h"
#include "include/gui/element/Value.h"
#include "include/gui/element/TextButton.h"
#include "include/gui/element/Tabs.h"
#include "include/gui/element/BPMSelector.h"
#include "include/gui/element/Crossfader.h"
#include "include/gui/laf/BoxToggle.h"
#include "include/gui/laf/ComboBox.h"
#include "include/gui/laf/Dial.h"
#include "include/gui/laf/IncrementalButtons.h"
#include "include/gui/laf/SVGButton.h"
#include "include/gui/laf/SVGToggle.h"
#include "include/gui/laf/TextButton.h"
#include "include/gui/laf/HorizontalSlider.h"
#include "include/gui/laf/FollowingValueHorizontalSlider.h"
#include "include/gui/layout/GridLayout.h"
#include "include/gui/layout/GridLayoutItem.h"
#include "include/gui/layout/AppLayout.h"
#include "include/gui/Spacing.h"
#include "include/gui/Border.h"

// Utilities
#include "include/utils/Singleton.h"
#include "include/utils/Logger.h"
#include "include/utils/UID.h"
#include "include/utils/FloatingPointUtils.h"

namespace nui = nierika::gui;
namespace nelement = nierika::gui::element;
namespace nlaf = nierika::gui::laf;
namespace nlayout = nierika::gui::layout;
namespace ndsp = nierika::dsp;
namespace nutils = nierika::utils;
