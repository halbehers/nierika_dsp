#ifdef NIERIKA_MODULES_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#include "nierika_dsp.h"

#include "source/NierikaDSPBinaryData.cpp"

// DSP
#include "source/dsp/CircularAudioBuffer.cpp"
#include "source/dsp/Timing.cpp"
#include "source/dsp/delay/SimpleDelay.cpp"
#include "source/dsp/filter/Filter.cpp"
#include "source/dsp/filter/HighPassFilter.cpp"
#include "source/dsp/filter/LowPassFilter.cpp"
#include "source/dsp/sequencer/FXSequencer.cpp"
#include "source/dsp/RMSProcessor.cpp"

// GUI
#include "source/gui/EmbeddedFonts.cpp"
#include "source/gui/Formatter.cpp"
#include "source/gui/Helpers.cpp"
#include "source/gui/Theme.cpp"
#include "source/gui/Section.cpp"
#include "source/gui/Component.cpp"
#include "source/gui/TooltipManager.cpp"
#include "source/gui/element/BoxToggle.cpp"
#include "source/gui/element/ComboBox.cpp"
#include "source/gui/element/Dial.cpp"
#include "source/gui/element/FrequencyDial.cpp"
#include "source/gui/element/HorizontalSlider.cpp"
#include "source/gui/element/IncrementalButton.cpp"
#include "source/gui/element/IncrementalButtons.cpp"
#include "source/gui/element/Level.cpp"
#include "source/gui/element/Levels.cpp"
#include "source/gui/element/PercentageDial.cpp"
#include "source/gui/element/SpectrumAnalyzer.cpp"
#include "source/gui/element/Slider.cpp"
#include "source/gui/element/Stars.cpp"
#include "source/gui/element/SVGButton.cpp"
#include "source/gui/element/SVGToggle.cpp"
#include "source/gui/element/TimeInMsDial.cpp"
#include "source/gui/element/TimingComboBox.cpp"
#include "source/gui/element/TimingDial.cpp"
#include "source/gui/element/Tooltip.cpp"
#include "source/gui/element/Title.cpp"
#include "source/gui/element/Text.cpp"
#include "source/gui/element/Value.cpp"
#include "source/gui/laf/BoxToggle.cpp"
#include "source/gui/laf/ComboBox.cpp"
#include "source/gui/laf/Dial.cpp"
#include "source/gui/laf/IncrementalButtons.cpp"
#include "source/gui/laf/SVGButton.cpp"
#include "source/gui/laf/SVGToggle.cpp"
#include "source/gui/layout/GridLayout.cpp"
#include "source/gui/layout/GridLayoutItem.cpp"
#include "source/gui/Spacing.cpp"
#include "source/gui/Border.cpp"

// Utilities
#include "source/utils/Logger.cpp"
