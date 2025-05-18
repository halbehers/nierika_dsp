# Nierika DSP - JUCE module

A JUCE module containing a collection of DSP &amp; GUI classes made to homegenize and facilitate Juce plugin development

---

## Table of Contents

- [About](#about)
- [Requirements & Dependencies](#requirements--dependencies)
- [Installation](#installation)
  - [Using Projucer](#using-projucer)
- [Usage](#usage)
- [Developers](#developers)

---

## About

This JUCE module is initially made for personnal use in order to make reusable GUI components and DSP processing classes to be used in all of my plugins.

---

## Requirements & Dependencies

**Minimum C++ Standard Version**: 17

**JUCE Modules Dependencies**:

`juce_core` `juce_dsp` `juce_graphics` `juce_gui_basics` `juce_gui_extra` `juce_audio_basics` `juce_audio_processors`

---

## Installation

### Using Projucer

1. Clone the repository locally
```shell
git clone git@github.com:halbehers/nierika_dsp.git
```

2. Add the module in the Projucer app:

<span>
<img width="400" alt="Screenshot 2025-04-08 at 14 12 11" src="https://github.com/user-attachments/assets/85a46d77-a872-47b7-ac43-5f47a4e2fc05" />
</span>
<span>
<img width="400" alt="Screenshot 2025-04-08 at 14 13 45" src="https://github.com/user-attachments/assets/a9a5efda-7401-41c6-ab8f-505cf325f38f" />
</span>

### Using CMake

Just as with the Projucer, you will need a local clone of the repository.
Here you can either clone it somewhere on your machine:
```shell
git clone git@github.com:halbehers/nierika_dsp.git
```

Or, add it as a git submodule into you project:
```shell
cd path/to/your/projects/libs/folder
git submodule add git@github.com:halbehers/nierika_dsp.git
```


Then add the following to your `CMakeLists.txt` file:

```CMake
# This line must appear BEFORE juce_add_plugin
juce_add_module("path/to/nierika_dsp" ALIAS_NAMESPACE nierika)

juce_add_plugin(
    ...
)

# This line must appear BEFORE target_compile_definitions
target_link_libraries(${PROJECT_NAME} PRIVATE nierika::nierika_dsp)

target_compile_definitions(
    ...
)
```

### Using CMake & CPM

Add the following to your `CMakeLists.txt` file:

```CMake
# replace with the version you want to use
set(NIERIKA_DSP_VERSION 0.1.0)
# replace with your lib folder location
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libs")

cpmaddpackage(
        NAME NIERIKA_DSP
        GIT_TAG "v${NIERIKA_DSP_VERSION}"
        VERSION ${NIERIKA_DSP_VERSION}
        GITHUB_REPOSITORY halbehers/nierika_dsp
        SOURCE_DIR ${LIB_DIR}/nierika_dsp
)

# This line must appear BEFORE juce_add_plugin
juce_add_module("${LIB_DIR}/nierika_dsp" ALIAS_NAMESPACE nierika)

juce_add_plugin(
    ...
)

# This line must appear BEFORE target_compile_definitions
target_link_libraries(${PROJECT_NAME} PRIVATE nierika::nierika_dsp)

target_compile_definitions(
        ...
)
```

---

## Usage

Every class is included in the namespace `nierika`.

This module is divided into 3 main namespaces: `gui`, `dsp` and `utils`.

So to include a dial for example, you want to do something like this:

```c++
nierika::gui::element::Dial _outputGain { "Output Gain", -60.f, 8.f, -8.f, "dB" };
```

> [!NOTE]
> For ease of use, a few namespace aliases has been added that you can use:
> ```c++
> namespace nui = nierika::gui;
> namespace nelement = nierika::gui::element;
> namespace nlaf = nierika::gui::laf;
> namespace nlayout = nierika::gui::layout;
> namespace ndsp = nierika::dsp;
> namespace nutils = nierika::utils;
> ```
> So the line of code above will become:
> ```c++
> nelement::Dial _outputGain { "Output Gain", -60.f, 8.f, -8.f, "dB" };
> ```

---

## Developers

- Sebastien Halbeher
