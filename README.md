# LCC_RPiPico_Turntable

An OpenLCB (LCC) node for a model railroad turntable, running on a Raspberry Pi Pico 2. It drives a stepper motor to position the bridge, auto-homes via Hall sensors, switches track phase via relays, and provides a full-color touch display for local control. It also consumes door-state events from a companion Roundhouse node to keep the display synchronized.

---

## Features

- **OpenLCB (LCC) node** — full CAN bus integration using the MustangPeak OpenLcbCLib
- **Stepper motor control** — A4988 (or DRV8825) driver, configurable micro-stepping, acceleration/deceleration via AccelStepper
- **Automatic homing** — Hall effect sensor on the home position; bridge position sensor for 180° awareness
- **Automatic phase switching** — relay(s) invert track polarity based on bridge angle (configurable AUTO or MANUAL)
- **Up to 20 tracks** — individual Front/Back/Occupancy/RailCom event IDs per track
- **Roundhouse door sync** — consumes door-toggle events from the Roundhouse node; door status shown on the display
- **Live bridge animation** — bridge redraws every 1° of stepper rotation during movement
- **Multi-board support** — single `#define` in the `.ino` selects v2.4 or v2.7 Stepper board pin layout
- **Multi-display support** — three compile-time display modes covering 800×480 parallel and 1024×600 SPI panels
- **RA8876 layer compositing** (v2.7 native mode) — static track background on hardware Layer 0; animated bridge on Layer 1; no track-line erase/redraw on each animation frame
- **LCC Fast Clock consumer** — fast clock displayed on the screen
- **NeoPixel bridge lighting** (optional)
- **Persistent state** — track positions, step count, current track, bridge orientation stored in external I2C EEPROM
- **Deferred EEPROM writes** — state changes coalesce into a single write ~3 seconds after the last event
- **OTA firmware update** via LCC datagram/stream protocol (PicoOTA + LittleFS)
- **Serial CLI** for NVM management and diagnostic logging
- **Dual-core operation** — Core 0 handles LCC/CAN/display/touch; Core 1 drives the stepper in real time

---

## Board Selection

Set exactly one define in [`LCC_RPiPico_Turntable.ino`](LCC_RPiPico_Turntable.ino) before the first `#include`:

```cpp
// Uncomment exactly one:
#define LCC_BOARD_STEPPER_V24
// #define LCC_BOARD_STEPPER_V27
```

[`BoardSettings.h`](BoardSettings.h) dispatches to the correct pin header automatically. All pin definitions live in `board_configs/`:

| Header | Board |
|---|---|
| `board_configs/BoardPins_Stepper_v24.h` | v2.4 Stepper — 800×480 SSD1963 8-bit parallel display |
| `board_configs/BoardPins_Stepper_v27.h` | v2.7 Stepper — 1024×600 RA8876 SPI display (BuyDisplay TFTM101) |

---

## Display Driver Selection

Three display modes are available, selected at compile time via `DISPLAY_DRIVER_*` defines. The correct default is set automatically by the board config header; the only override needed is for the RA8876 native mode on v2.7.

| Define | Board | Library | Panel |
|---|---|---|---|
| `DISPLAY_DRIVER_SSD1963_PARALLEL` | v2.4 (fixed) | `TFT_eSPI_RA8876` | 800×480, 8-bit parallel |
| `DISPLAY_DRIVER_RA8876_TFTESPI` | v2.7 (default) | `TFT_eSPI_RA8876` | 1024×600 (TFTM101), SPI1 |
| `DISPLAY_DRIVER_RA8876_NATIVE` | v2.7 (optional) | `RA8876_RP2040` | 1024×600 (TFTM101), SPI1 |

To use the native RA8876 driver (unlocks layer compositing and BTE acceleration), add this define **before** the board `#define` in the `.ino`:

```cpp
#define DISPLAY_DRIVER_RA8876_NATIVE
#define LCC_BOARD_STEPPER_V27
```

All display abstraction is handled by [`DisplayDriver.h`](DisplayDriver.h) / [`DisplayDriver.cpp`](DisplayDriver.cpp). For the two TFT_eSPI modes `TT_Display` is a typedef for `TFT_eSPI`. For native mode it is a compatibility wrapper class that presents the same API so `UserInterface.cpp` compiles unchanged, while also exposing `selectCanvas()`, `freezeBackground()`, and `clearCanvas()` for layer compositing.

**No `User_Setup.h` edits are required.** All TFT_eSPI pin macros and `USER_SETUP_LOADED` are set in `display_configs/`:

| Config header | Mode |
|---|---|
| `display_configs/DisplayConfig_SSD1963_parallel.h` | SSD1963 8-bit parallel (Setup104a) |
| `display_configs/DisplayConfig_RA8876_SPI.h` | RA8876 / LT7683 SPI (RP2040_RA8876) |

---

## Hardware

### v2.4 Stepper board

| Component | Details |
|---|---|
| **MCU** | Raspberry Pi Pico 2 |
| **CAN controller** | MCP2517/2518FD on SPI0 (gp16–20) |
| **Stepper driver** | A4988 / DRV8825 — STEP=gp15, DIR=gp2, EN=gp14 |
| **EEPROM** | 24LC512 on I2C1/Wire1 (gp26/gp27) |
| **Home sensor** | Hall effect on gp1 (active LOW) |
| **Bridge sensor** | Hall effect on gp0 (active LOW) |
| **TFT display** | 800×480, SSD1963 controller, 8-bit parallel bus (gp6–13 data, gp22 WR, gp28 DC) |
| **Touch controller** | I2C capacitive, Wire/I2C0 on gp4/gp5 |
| **NeoPixel** | gp21 |
| **Buttons** | Blue=gp26, Gold=gp27 (shared with EEPROM I2C) |

### v2.7 Stepper board

| Component | Details |
|---|---|
| **MCU** | Raspberry Pi Pico 2 |
| **CAN controller** | MCP2517/2518FD on SPI0 (gp16–20) — unchanged from v2.4 |
| **Stepper driver** | A4988 / DRV8825 — STEP=gp7, DIR=gp2, EN=gp6 |
| **EEPROM** | 24LC512 on I2C1/Wire1 (gp26/gp27) — unchanged |
| **Home sensor** | Hall effect on gp1 (active LOW) |
| **Bridge sensor** | Hall effect on gp0 (active LOW) |
| **TFT display** | BuyDisplay ER-TFTM101-1 — 10.1" 1024×600, RA8876 (or LT7683) controller, SPI1 (gp8–11), RST=gp14 |
| **Touch controller** | Goodix GT9271 capacitive (10-point), I2C0/Wire on gp12/gp13 — detected as CT_TYPE_GT911 by `my_bb_captouch` |
| **NeoPixel** | gp3 |
| **Buttons** | Blue=gp21, Gold=gp22 |

---

## Pin Assignments

### v2.4 Stepper board

| Signal | GPIO |
|---|---|
| Bridge sensor | 0 |
| Home sensor | 1 |
| Stepper DIR | 2 |
| Touch SDA (Wire/I2C0) | 4 |
| Touch SCL (Wire/I2C0) | 5 |
| TFT D0–D7 (PIO) | 6–13 |
| Stepper ENABLE | 14 |
| Stepper STEP | 15 |
| MCP2517 SDO (MISO) | 16 |
| MCP2517 CS | 17 |
| MCP2517 SCK | 18 |
| MCP2517 SDI (MOSI) | 19 |
| MCP2517 INT | 20 |
| NeoPixel | 21 |
| TFT WR (PIO write strobe) | 22 |
| Onboard LED | 25 |
| EEPROM SDA / Blue Button (Wire1/I2C1) | 26 |
| EEPROM SCL / Gold Button (Wire1/I2C1) | 27 |
| TFT DC | 28 |

### v2.7 Stepper board

| Signal | GPIO |
|---|---|
| Bridge sensor | 0 |
| Home sensor | 1 |
| Stepper DIR | 2 |
| NeoPixel | 3 |
| Stepper ENABLE | 6 |
| Stepper STEP | 7 |
| TFT SDI / SPI1 MOSI | 8 |
| TFT CS / SPI1 CS | 9 |
| TFT SCK / SPI1 CLK | 10 |
| TFT SDO / SPI1 MISO | 11 |
| Touch SDA (Wire/I2C0) | 12 |
| Touch SCL (Wire/I2C0) | 13 |
| TFT RST | 14 |
| MCP2517 SDO (MISO) | 16 |
| MCP2517 CS | 17 |
| MCP2517 SCK | 18 |
| MCP2517 SDI (MOSI) | 19 |
| MCP2517 INT | 20 |
| Blue Button | 21 |
| Gold Button | 22 |
| Onboard LED | 25 |
| EEPROM SDA (Wire1/I2C1) | 26 |
| EEPROM SCL (Wire1/I2C1) | 27 |

---

## LCC Node ID

The node ID is set by `#define NODE_ID` in [`LCC_RPiPico_Turntable.ino`](LCC_RPiPico_Turntable.ino).
The default (`0x050101019411`) is in the Southern Piedmont range assigned to Bob Gamble. Change this to a unique ID from your assigned range before deploying.

---

## Dependencies

Install the following via **Arduino Library Manager** or manually:

| Library | Purpose |
|---|---|
| `ACAN2517` by Pierre Molinaro | MCP2517/2518 CAN transceiver driver |
| `TFT_eSPI_RA8876` (modified TFT_eSPI) | Display driver for SSD1963 parallel and RA8876 SPI modes |
| `RA8876_RP2040` | Native RA8876 driver — required only for `DISPLAY_DRIVER_RA8876_NATIVE` |
| `I2C_eeprom` by Rob Tillaart | 24LC512 EEPROM read/write |
| `NeoPixelBus` | NeoPixel LED control |
| `LibPrintf` | `printf()` support over Serial |
| `PicoOTA` | Over-the-air firmware update (Philhower core) |
| `LittleFS` | Flash filesystem for OTA image staging |
| `Wire` | I2C (built into Arduino core) |
| `SPI` | SPI (built into Arduino core) |

**Local libraries (do not replace with stock versions):**

| Library | Location | Notes |
|---|---|---|
| `AccelStepper` | `src/application_drivers/AccelStepper.cpp/.h` | Modified local copy — contains Pico-specific changes |
| `my_bb_captouch` | `src/application_drivers/my_bb_captouch.cpp/.h` | Modified bb_captouch by Bob Gamble — configurable Wire instance, RP2040-compatible |

The OpenLCB stack (`src/openlcb/`, `src/drivers/canbus/`) is included in the repository as the MustangPeak OpenLcbCLib — no separate install needed.

> **Board package:** Use [Earle Philhower's RP2040 package](https://github.com/earlephilhower/arduino-pico#installation) — **not** the Mbed package.
> Board target: `rp2040:rp2040:rpipico2`

---

## Build Configuration

See [`sketch.yaml`](sketch.yaml) for the full build profile:

- Flash: 4 MB — 2 MB filesystem space for firmware updates
- Optimization: `Small`
- C++ standard: `gnu++17`

The display driver is selected entirely at compile time via `DISPLAY_DRIVER_*` defines. No edits to the `TFT_eSPI` library's `User_Setup.h` are needed — `USER_SETUP_LOADED` and all pin macros are set in the `display_configs/` headers included automatically by the board config.

---

## Configuration Memory (CDI)

Node configuration (track count, track positions, step count, home track, door count, event IDs, etc.) is stored in the external EEPROM and described by the CDI XML in [`Documentation/openlcb-config-2026-01-30.xml`](Documentation/openlcb-config-2026-01-30.xml). Use any LCC configuration tool (e.g., JMRI's PanelPro) to edit these settings over the LCC bus.

The memory layout is auto-generated — do not hand-edit [`Documentation/config_mem_map.h`](Documentation/config_mem_map.h).

---

## Stepper Configuration

Key parameters in [`BoardSettings.h`](BoardSettings.h):

| Define | Default | Description |
|---|---|---|
| `STEPPER_DRIVER` | `A4988_INV` | Driver type (A4988, DRV8825, ULN2003 variants) |
| `FULLSTEPS` | `400` | Full steps per motor revolution |
| `DRIVER_BOARD_STEPPING` | `16` | Microstepping factor (1, 2, 4, 8, or 16) |
| `STEPPER_GEARING_FACTOR` | `1` | Mechanical gear ratio |
| `STEPPER_MAX_SPEED` | `1800` | Steps/sec maximum |
| `STEPPER_ACCELERATION` | `180` | Steps/sec² |
| `DISABLE_OUTPUTS_IDLE` | defined | Disable stepper driver when not moving |
| `PHASE_SWITCHING` | `AUTO` | AUTO or MANUAL phase relay control |

Calculated: `FULL_TURN_STEPS = FULLSTEPS × STEPPER_GEARING_FACTOR × DRIVER_BOARD_STEPPING`

---

## Serial CLI Commands

Connect at **115200 baud**. Available commands:

| Key | Action |
|---|---|
| `h` | Print help |
| `c` | Clear NVM to `0x00` |
| `i` | Reset NVM to CDI default values |
| `r` | Reset NVM to `0xFF` (factory fresh) |
| `p` | Toggle LCC message logging |
| `m` | Toggle config memory read/write logging |
| `x` | Load application defaults |
| `z` | Re-apply config values from NVM |

---

## Architecture

The Pico runs two cores:

- **Core 0** (`setup()` / `loop()`): OpenLCB protocol stack, CAN comms, event handling, touch input (`touchIO()`), bridge animation (`updateBridgeAnimation()`), serial CLI
- **Core 1** (`setup1()` / `loop1()`): Real-time stepper control (`runStepper()`), homing, calibration, position checking, final bridge draw on motor stop

Key modules:

| File | Role |
|---|---|
| `LCC_RPiPico_Turntable.ino` | Entry point, board/display selection, node init, event registration, serial CLI |
| `BoardSettings.h` | Dispatches to the correct board pin header; holds shared parameters (NVM sizes, stepper config, UI constants) |
| `board_configs/BoardPins_Stepper_v24.h` | v2.4 pin definitions + SSD1963 parallel display config |
| `board_configs/BoardPins_Stepper_v27.h` | v2.7 pin definitions + RA8876 SPI display config / native mode guard |
| `display_configs/DisplayConfig_SSD1963_parallel.h` | TFT_eSPI USER_SETUP for SSD1963 8-bit parallel |
| `display_configs/DisplayConfig_RA8876_SPI.h` | TFT_eSPI USER_SETUP for RA8876 SPI (TFTM101) |
| `DisplayDriver.h` / `DisplayDriver.cpp` | Display abstraction: `TT_Display` typedef (TFT_eSPI modes) or wrapper class (native mode); layer compositing helpers |
| `Turntable.cpp` | Stepper movement, homing, calibration, phase switching, LED |
| `UserInterface.cpp` | TFT rendering, touch input, turntable diagram, bridge animation, track/door display |
| `callbacks.cpp` | OpenLCB event handlers, 100ms timer, CAN Rx/Tx, OTA firmware |
| `config_mem_helper.cpp` | EEPROM config storage, CDI memory map, track/step persistence |
| `TTvariables.h` | Shared type definitions (`TrackAddress`, `LightAddress`, `npStrings`, `HotBox`) |

### Bridge animation

`updateBridgeAnimation()` is called from `loop()` on every Core 0 iteration. It fires a bridge redraw whenever the stepper has moved ≥ 1° since the last frame. This is entirely non-blocking.

- **RA8876 native mode**: `drawBridge()` calls `clearCanvas()` to BTE-wipe Layer 1 only. Track lines on Layer 0 are hardware-composited by the RA8876 chip and are never touched by bridge animation frames.
- **TFT_eSPI modes**: `drawBridge()` erases the TT interior with `fillCircle()`, then `updateBridgeAnimation()` immediately calls `drawTracks()` to restore the track lines on the single framebuffer.

### RA8876 layer compositing (native mode only)

`drawTurnTable()` sets up two layers once at startup:

1. Calls `tft.selectCanvas(0)` — draws the outer ring and all track lines onto Layer 0
2. Calls `tft.freezeBackground()` — locks Layer 0 as the compositor's read-only source
3. Calls `tft.selectCanvas(1)` + `tft.clearCanvas()` — switches to Layer 1 for all subsequent bridge and shack draws

The chip hardware-composites both layers to the panel on every scan line. Animation only ever writes to Layer 1, so track redraws are completely eliminated during movement.

---

## License

BSD 2-Clause — see [`LICENSE`](LICENSE).
