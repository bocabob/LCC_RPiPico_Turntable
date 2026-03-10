# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Environment

- **IDE**: Arduino IDE (primary) or VSCode with Arduino extension
- **Board**: Raspberry Pi Pico 2 (`rp2040:rp2040:rpipico2`) using [Philhower's RP2040 package](https://github.com/earlephilhower/arduino-pico#installation) — NOT the Mbed package
- **C++ Standard**: gnu++17
- **Build config**: [sketch.yaml](sketch.yaml) (4MB flash, optimization: small)

Required libraries (install via Arduino Library Manager):
- `ACAN2517` by Pierre Molinaro — CAN transceiver (MCP2517/18)
- `AccelStepper` — local customized copy in `src/application_drivers/` (do not replace with stock library)
- `TFT_eSPI`, `I2C_eeprom`, `NeoPixelBus`, `Wire`

There are no automated tests. Manual testing uses serial console commands in `loop()`:
- `'c'` clear NVM, `'i'` reset to CDI defaults, `'r'` factory reset, `'p'` toggle message logging, `'m'` toggle config memory logging, `'x'` load app defaults

## Architecture

This is an **OpenLCB (LCC) node** controlling a model railroad turntable via stepper motor, with a 320×480 TFT touch display, NeoPixel lighting, roundhouse door control, and CAN bus networking.

### Dual-Core Structure

The Pico runs two cores:
- **Core 0** (`setup()`/`loop()`): OpenLCB protocol, CAN comms, event handling, UI
- **Core 1** (`setup1()`/`loop1()`): timing-critical stepper motor control

### Key Module Responsibilities

| File | Role |
|---|---|
| [LCC_RPiPico_Turntable.ino](LCC_RPiPico_Turntable.ino) | Main sketch entry point, OpenLCB node init, serial CLI |
| [Turntable.cpp](Turntable.cpp) | Stepper movement, homing via Hall sensor, phase relay switching, calibration |
| [UserInterface.cpp](UserInterface.cpp) | TFT rendering, touch input, track matrix display, turntable diagram |
| [callbacks.cpp](callbacks.cpp) | OpenLCB event handlers (consumed/produced), 100ms timer, CAN rx/tx |
| [config_mem_helper.cpp](config_mem_helper.cpp) | I2C EEPROM config storage, CDI-driven memory map |
| [BoardSettings.h](BoardSettings.h) | All hardware pin definitions |
| [TTvariables.h](TTvariables.h) | Global state structs (`_Tracks[]`, `_Doors[]`, `_Strings[]`, etc.) |

### Configuration Memory

Config is stored in external I2C EEPROM using a CDI (XML)-generated memory map:
- [Documentation/config_mem_map.h](Documentation/config_mem_map.h) — auto-generated layout (do not hand-edit)
- [Documentation/config_mem_reset.c](Documentation/config_mem_reset.c) — auto-generated defaults
- [Documentation/openlcb-config-2026-01-30.xml](Documentation/openlcb-config-2026-01-30.xml) — CDI descriptor

Config structures use `#pragma pack(push, 1)` for exact memory layout.

### Key Data Flow

1. **Track selection**: LCC consumed event → `callbacks.cpp` → `moveToPosition()` in `Turntable.cpp` → AccelStepper → relay phase switch
2. **Touch input**: touch press → `HotSpotBox()` → `touchCommand()` → turntable move or settings change
3. **Homing**: Hall sensor interrupt → stepper rotation → `fullTurnSteps` calibration → stored to EEPROM
4. **NeoPixel effects**: `ProcessLED()` cycles on/off states (blink, flicker) → `setAccessory()`

### Naming Conventions

- Functions: `camelCase` (`moveToPosition`, `drawTurnTable`)
- Global variables: `camelCase` or `snake_case`
- Structs: `PascalCase` (`TrackAddress`, `npHead`)
- Constants/defines: `UPPER_SNAKE_CASE`

### Important Implementation Notes

- All motor, LED, and calibration logic is **non-blocking** — uses `millis()` timers, never `delay()`
- Hall sensor debounce: 50ms
- Phase switching (relay) is auto-triggered based on configurable step thresholds (`phaseSwitchStartSteps`/`StopSteps`)
- `src/application_drivers/AccelStepper` is a **modified local copy** — do not replace with the Arduino library version
- The OpenLCB stack lives entirely in `src/openlcb/` and `src/drivers/canbus/` — this is the MustangPeak OpenLcbClib C library
