# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`nierika_dsp` is a **JUCE module** (not a standalone app) providing reusable GUI and DSP building blocks shared across the author's JUCE plugins/apps. It has no `main()`, no CMakeLists.txt, and no test suite of its own — it's consumed by host projects via Projucer or CMake (`juce_add_module(...)`, see the README's "Installation" section for the exact CMake/CPM snippets). The module manifest (`nierika_dsp.h`'s `BEGIN_JUCE_MODULE_DECLARATION` block) declares `minimumCppStandard: 20`.

Everything lives under the `nierika::` namespace, split into three sub-namespaces: `nierika::gui`, `nierika::dsp`, `nierika::utils`. Common aliases (defined at the bottom of `nierika_dsp.h`) are the idiomatic way host code refers to them: `nui`, `nelement`, `nlaf`, `nlayout`, `ndsp`, `nutils`.

## Building / verifying changes

There is nothing to build or test in isolation here. `nierika_dsp.cpp` is a single JUCE-style **unity build** translation unit that `#include`s every `.cpp` in the module in dependency order — it can only be compiled as part of a host JUCE project that adds this module via `juce_add_module(...)`. To verify a change actually compiles and behaves correctly, build/run whatever host plugin or app project has this module wired in as a dependency.

When adding a new file:
- New public headers must be added to `nierika_dsp.h`'s include list (in the right dependency position).
- New `.cpp` files must be added to `nierika_dsp.cpp`'s include list the same way, or they will silently never be compiled by any consumer.

### Embedding binary assets (fonts / SVG icons)

Don't hand-edit `include/NierikaDSPBinaryData.h/.cpp` — use the scripts, which replicate Projucer's `BinaryData` generator and also wire up a typed accessor:

```shell
python3 script/add_font.py path/to/Font.otf [--name MyFont] [--dry-run]
python3 script/add_svg_icon.py path/to/icon.svg [--name Settings] [--dry-run]
```

`add_font.py` registers the font and (unless `--no-getter`) adds an `EmbeddedFonts::get<Name>()` accessor. `add_svg_icon.py` does the same for `Icons::get<Name>()`. `--dry-run` previews the diff without writing.

## Architecture

### GUI element / LookAndFeel pairing

Most interactive widgets under `include/gui/element/*.h` (+ matching `source/gui/element/*.cpp`) wrap one or more inner `juce::Component`s (`juce::TextEditor`, `juce::ComboBox`, `juce::TextButton`, ...) and own a paired `juce::LookAndFeel_V4` subclass under `include/gui/laf/`. The LAF class takes `const element::X& parent` by reference in its constructor, is installed via `_inner.setLookAndFeel(&_lookAndFeel)`, and reads all visual state (colours, radius, sizing) back off `_parent` instead of duplicating it — so an element's public getters (`getBackgroundColour()`, `getBorderRadius()`, ...) double as the LAF's data source.

A few elements paint themselves directly instead of wrapping a JUCE control (e.g. `MultiWaySwitch`, the shared base for `TwoWaySwitch`/`ThreeWaySwitch`) — these have no `laf/` counterpart and do all sizing/drawing logic inline in `paint()`.

Every GUI element inherits from `nierika::gui::Component` (`include/gui/Component.h`), not `juce::Component` directly. It adds:
- margin/padding (`setMargin`/`setPadding`), consumed via `getLocalBounds()`/`getOuterLocalBounds()` (which subtract padding/margin) instead of raw `juce::Component::getLocalBounds()`.
- declarative `displayBorder(...)`/`displayBackground(...)` helpers (colour or `Theme::ThemeColor`-driven, with optional radius/alpha/gradient).
- tooltip support (`setTooltip`/`setTooltipEnabled`).
- auto-subscription to `Theme`'s change broadcaster, so every element repaints when the theme changes without extra wiring (`Component::changeListenerCallback`).

### Theme (`include/gui/Theme.h` / `source/gui/Theme.cpp`)

`Theme` is a fully static class — colours, fonts, border radius, and element heights are all resolved through it, never hardcoded at the component level. Host apps configure it once via `Theme::configure(...)` (before constructing any `nierika` GUI component) or through fine-grained setters (`setColor`, `setFont`, `setBorderRadius`, `setThinHeight`/`setLargeHeight`, `setMode`, ...). Every setter composes onto `Theme::_lastConfig` (previous overrides are preserved, never reset) and re-derives all active state via the private `applyConfig()`, which ends by firing `Theme::getChangeBroadcaster()` synchronously.

`Theme::HeightType` (`THIN` / `LARGE` / `AUTO`) lets an element opt into a Theme-configured fixed pixel height via `element.setHeightType(...)`, resolved through `Theme::resolveHeight(type, autoHeight)`. `AUTO` (the default) preserves the historical "fill whatever bounds the host layout assigns" behavior.

See the README's "Theming" section for the full host-facing configuration API and examples — it's the source of truth for how a consuming plugin re-themes the whole GUI from one place.

### Layout system (`include/gui/layout`)

- `layout::Section` (`include/gui/Section.h`) is the mid-level building block: a self-contained panel with its own named `GridLayout` "panels" (switchable via `element::Tabs`), optional header/footer, bypass toggle, and FX-sequencer-activation toggle. Plugin UIs are typically composed of several `Section`s.
- `layout::AppLayout` extends `Section` and is the usual top-level editor root a host plugin's own layout builds on top of.
- `layout::GridLayout<T>` is the templated row/column grid engine underneath every `Section`: resizable dividers, drag-to-move cells, per-cell borders, placement via `addComponent(...)` + bounds lookup via `getBounds(identifier)`.
- `layout::WindowsManager` / `layout::Window` / `layout::Dialog` manage floating overlay windows within a host editor (e.g. a settings panel).

### Parameter management (`include/dsp`)

`dsp::ParameterManager` wraps `juce::AudioProcessorValueTreeState`. Host `AudioProcessor`s own one and register parameters through its typed `registerParameter(...)` overloads (bool/int/float, with tooltip/display-name variants) rather than building a `juce::AudioProcessorValueTreeState::ParameterLayout` by hand; it also owns `getStateInformation`/`setStateInformation`. `dsp::Parameter`/`IParameter` model an individual registered parameter (type, tooltip, default/min/max).

## Conventions

- Enums nested in their owning class are unscoped by default for "theme-like" categorical values consumed as e.g. `Theme::TEXT` (`Theme::ThemeColor`, `Theme::FontWeight`, `Theme::FontSize`) — but `enum class` where an unscoped enum would collide with another enum already injected into the same class scope (e.g. `Theme::HeightType` next to `Theme::FontWeight::THIN`), or where scoping is otherwise wanted (`Theme::Mode`, `Theme::FontFamily`, `TextButton::IconPosition`).
- Widgets communicate via listener interfaces declared as nested structs (`OnValueChangedListener`, `OnClickListener`, ...) with `addXListener`/`removeXListener` pairs, not `juce::Button::Listener`-style inheritance.
- Per-instance visual overrides follow a `set.../reset.../get...` triplet, backed by either a sentinel value (e.g. `_borderRadiusOverride = -1`) or `std::optional`, where the getter falls back to a `Theme`-derived default when unset — see `TextInput::getBorderRadius()` / `ComboBox::getBackgroundColour()`.
- Every class wrapping a `juce::Component` uses `JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR`.
