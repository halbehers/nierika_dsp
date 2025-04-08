# Nierika DSP - JUCE module

A JUCE module containing a collection of DSP &amp; GUI classes made to homegenize and facilitate Juce plugin development

---

## Table of Contents

- [About](#about)
- [Installation](#installation)
  - [Using Projucer](#using-projucer)
- [Usage](#usage)
- [Developers](#developers)

---

## About

This JUCE module is initially made for personnal use in order to make reusable GUI components and DSP processing classes to be used in all of my plugins.

---

## Installation

### Using Projucer

1. Clone the repository locally
```bash
git clone git@github.com:halbehers/nierika_dsp.git
```

2. Add the module in the Projucer app:

<span>
<img width="400" alt="Screenshot 2025-04-08 at 14 12 11" src="https://github.com/user-attachments/assets/85a46d77-a872-47b7-ac43-5f47a4e2fc05" />
</span>
<span>
<img width="400" alt="Screenshot 2025-04-08 at 14 13 45" src="https://github.com/user-attachments/assets/a9a5efda-7401-41c6-ab8f-505cf325f38f" />
</span>

---

## Usage

Every classes is included in the namespace `nierika`.

This module is divided in 2 main namespaces: `gui` and `dsp`.

So to include a dial for example, you want to do something like this:

```cpp
nierika::gui::element::Dial _outputGain { "Output Gain", -60.f, 8.f, -8.f, "dB" };
```

---

## Developers

- Sebastien Halbeher
