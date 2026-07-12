# Nierika DSP - JUCE module

A JUCE module containing a collection of DSP &amp; GUI classes made to homegenize and facilitate Juce plugin development

---

## Table of Contents

- [About](#about)
- [Requirements & Dependencies](#requirements--dependencies)
- [Installation](#installation)
  - [Using Projucer](#using-projucer)
- [Usage](#usage)
- [Theming](#theming)
  - [How components consume the theme](#how-components-consume-the-theme)
  - [Configuring a palette and fonts](#configuring-a-palette-and-fonts)
  - [Dark & Light mode](#dark--light-mode)
  - [Fine-grained overrides](#fine-grained-overrides)
  - [Monospace fonts](#monospace-fonts)
  - [Live theme switching](#live-theme-switching)
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

## Theming

Every colour and font used across the toolkit is centralized in `nierika::gui::Theme` (`include/gui/Theme.h`). Nothing in `nierika_dsp` hardcodes a raw colour or font at the component level — every element and `LookAndFeel` class resolves its visuals through `Theme`, so configuring `Theme` once from your host application re-themes the entire GUI.

By default, `Theme` uses a hardcoded **Dark** palette and the module's embedded fonts — if you never call any of the configuration API described below, your plugin looks exactly the same as it always has.

### How components consume the theme

Components never reference hex values directly. They ask `Theme` for a semantic colour or font:

```c++
// Colours are requested by role, not by value
juce::Colour accent = nui::Theme::newColor(nui::Theme::ACCENT).asJuce();

// Fonts are requested by weight and size
juce::Font titleFont = nui::Theme::newFont(nui::Theme::BOLD, nui::Theme::TITLE);
```

The available roles are:

```c++
enum ThemeColor
{
    TRANSPARENT, EMPTY_SHADE, LIGHTER_SHADE, LIGHT_SHADE, MEDIUM_SHADE,
    DARK_SHADE, FULL_SHADE, BLACK, BACKGROUND, PRIMARY, ACCENT, TEXT, DISABLED, DANGER, WARNING
};

enum FontWeight { BOLD, MEDIUM, REGULAR, LIGHT, THIN };
enum FontSize   { TITLE, SUBTITLE, HEADING, CAPTION, PARAGRAPH, LABEL, SMALL };
```

Because every consumer goes through `Theme::newColor`/`Theme::newFont`, you never need to touch individual components to re-theme your plugin — you configure `Theme` once, and it propagates everywhere automatically.

### Configuring a palette and fonts

Call `Theme::configure(...)` once, before any `nierika` GUI component is constructed — the safest place is at the top of your `AudioProcessor` constructor (or `JUCEApplication::initialise()` for a non-plugin app), since `createEditor()` always runs after that:

```c++
PluginAudioProcessor::PluginAudioProcessor()
    : /* ...your existing initializer list... */
{
    nui::Theme::configure({
        .mode = nui::Theme::Mode::DARK,
        .colorOverrides = {
            { nui::Theme::ACCENT,  juce::Colour(0xFFFF3355) },
            { nui::Theme::PRIMARY, juce::Colour(0xFF1F1F2E) }
        },
        .fontOverrides = {
            { nui::Theme::BOLD, juce::Font(juce::FontOptions{}.withTypeface(myBoldTypeface)) }
        }
    });
}
```

`Theme::Config` fields are all optional:

| Field | Purpose |
|---|---|
| `mode` | Starts from the built-in `Dark` or `Light` preset (see below). Defaults to `Mode::DARK`. |
| `customPalette` | Fully replaces the preset with your own `ThemeColor → juce::Colour` map. Any role you don't specify falls back to the preset's value. |
| `colorOverrides` | Sparse tweaks applied on top of the preset (or `customPalette`) — the values that always win. Use this for brand colours like `ACCENT`/`PRIMARY` without redefining the whole palette. |
| `fontOverrides` | Per-`FontWeight` `juce::Font` replacements. Any weight you don't specify keeps using the module's embedded fonts. |

### Dark & Light mode

`Theme::Mode::DARK` reproduces the module's original hardcoded values byte-for-byte. `Theme::Mode::LIGHT` is a built-in alternative preset with inverted neutrals (dark text/ink on light surfaces) while keeping brand colours (`PRIMARY`, `ACCENT`, `DANGER`, `WARNING`) close to their dark-mode values for brand consistency. Pick one via `Config::mode`, or switch later:

```c++
nui::Theme::setMode(nui::Theme::Mode::LIGHT);
nui::Theme::Mode current = nui::Theme::getMode();
```

Both presets are just a starting point — layer `colorOverrides`/`fontOverrides` on top of either one to match your brand.

### Fine-grained overrides

Beyond the one-shot `configure()` call, `Theme` exposes individual setters. Each of these **composes** with whatever was last configured (previous overrides are preserved) instead of resetting everything:

```c++
nui::Theme::setColor(nui::Theme::ACCENT, juce::Colour(0xFFFF3355));
nui::Theme::setPalette(myFullPalette);                 // full custom palette
nui::Theme::setFont(nui::Theme::BOLD, myBoldFont);
nui::Theme::setFonts(myFontSet);                        // multiple weights at once
nui::Theme::resetToDefaults();                          // back to Mode::DARK, no overrides
```

### Monospace fonts

Alongside the default proportional family (selected via `FontWeight`), `Theme` supports a second, independent font family for monospace needs (code, values, timers, ...), selected via `Theme::FontFamily`:

```c++
juce::Font mono = nui::Theme::newFont(nui::Theme::FontFamily::Monospace, nui::Theme::REGULAR, nui::Theme::CAPTION);
```

Out of the box — with no font embedded — this falls back to the OS's built-in monospace font (Menlo, Consolas, ...), so it works immediately. To embed your own monospace `.otf`/`.ttf`:

```shell
python3 script/add_font.py path/to/YourMono-Regular.otf
```

This registers the font as `NierikaDSPBinaryData` (the same mechanism `script/add_svg_icon.py` uses for icons) and adds an `EmbeddedFonts::get<Name>()` accessor. Then wire it in by replacing the fallback entries in `Theme::buildDefaultMonospaceFontSet()` (`source/gui/Theme.cpp`) with calls to your new accessor, the same way `buildDefaultFontSet()` wires up the default Brandon family. Hosts can also override monospace fonts per-weight via `Theme::setMonospaceFont`/`setMonospaceFonts`, or as part of `Theme::configure(...)`'s `monospaceFontOverrides`.

### Live theme switching

`Theme` exposes a `juce::ChangeBroadcaster` that fires whenever `configure`/`setMode`/`setColor`/`setFont` is called:

```c++
nui::Theme::getChangeBroadcaster();
```

Every `nierika::gui::Component` already listens to it: switching `Theme::setMode(...)` at runtime (e.g. from a Settings toggle) immediately repaints borders, backgrounds, and everything drawn through a `LookAndFeel` — no app restart required.

> [!NOTE]
> A handful of elements bake a resolved colour/font into a child `juce::Label`/`juce::Slider` once at construction time (via `.setColour(...)`/`.setFont(...)`). These will pick up a new theme on the next app launch, but not instantly on a live toggle. If your own host component reads `Theme::newColor`/`Theme::newFont` directly in its own `paint()`, you can get the same live-refresh behavior by implementing `juce::ChangeListener` yourself and calling `repaint()` from `changeListenerCallback`.

---

## Developers

- Sebastien Halbeher
