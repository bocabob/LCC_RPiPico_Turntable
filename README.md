# LCC_RPiPico_Turntable

An OpenLCB (LCC) node for a model railroad turntable, running on a Raspberry Pi Pico 2. It drives a stepper motor to position the bridge, auto-homes via Hall sensors, switches track phase via relays, and provides a full-color 800×480 touch display for local control. It also consumes door-state events from a companion Roundhouse node to keep the display synchronized.

---

## Features

- **OpenLCB (LCC) node** — full CAN bus integration using the MustangPeak OpenLcbCLib
- **Stepper motor control** — A4988 (or DRV8825) driver, configurable micro-stepping, acceleration/deceleration via AccelStepper
- **Automatic homing** — Hall effect sensor on the home position; bridge position sensor for 180° awareness
- **Automatic phase switching** — relay(s) invert track polarity based on bridge angle (configurable AUTO or MANUAL)
- **Up to 20 tracks** — individual Front/Back/Occupancy/RailCom event IDs per track
- **Roundhouse door sync** — consumes door-toggle events from the Roundhouse node; door status shown on the display
- **800×480 touch TFT display** — live turntable diagram, track buttons, settings and diagnostic pages
- **LCC Fast Clock consumer** — fast clock displayed on the screen
- **NeoPixel bridge lighting** (optional)
- **Persistent state** — track positions, step count, current track, bridge orientation stored in external I2C EEPROM
- **Deferred EEPROM writes** — state changes coalesce into a single write 3 seconds after the last event
- **OTA firmware update** via LCC datagram/stream protocol (PicoOTA + LittleFS)
- **Serial CLI** for NVM management and diagnostic logging
- **Dual-core operation** — Core 0 handles LCC/CAN/display; Core 1 drives the stepper in real time

---

## Hardware

| Component | Details |
|---|---|
| **MCU** | Raspberry Pi Pico 2 (`rp2040:rp2040:rpipico2`) |
| **CAN controller** | Microchip MCP2517/2518FD on SPI |
| **Stepper driver** | A4988 or DRV8825 (Step/Dir/Enable, 3-wire) |
| **EEPROM** | 24LC512 (64 KB) at I2C address `0x50` (Wire1) |
| **Home sensor** | Hall effect sensor on GPIO 1 (active LOW) |
| **Bridge sensor** | Hall effect sensor on GPIO 0 (active LOW) |
| **Phase relays** | GPIO 25 (configure `relay1Pin` / `relay2Pin` in `BoardSettings.h`) |
| **TFT display** | 800×480 parallel 8-bit (ILI9488 or compatible), configured in `TFT_eSPI` `User_Setup.h` |
| **Touch controller** | I2C capacitive touch on GPIO 4 (SDA) / GPIO 5 (SCL) |
| **NeoPixel** | GPIO 21 (optional) |

---

## Pin Assignments

All pin definitions are in [`BoardSettings.h`](BoardSettings.h). The TFT data bus pins are configured in the `TFT_eSPI` library's `User_Setup.h` file (reference values are commented in `BoardSettings.h`).

| Signal | GPIO |
|---|---|
| MCP2517 CS | 17 |
| MCP2517 INT | 20 |
| MCP2517 SCK | 18 |
| MCP2517 SDI (MOSI) | 19 |
| MCP2517 SDO (MISO) | 16 |
| EEPROM SDA (Wire1) | 26 |
| EEPROM SCL (Wire1) | 27 |
| Touch SDA (Wire) | 4 |
| Touch SCL (Wire) | 5 |
| Home sensor | 1 |
| Bridge sensor | 0 |
| Stepper STEP | 15 |
| Stepper DIR | 2 |
| Stepper ENABLE | 14 |
| NeoPixel | 21 |
| TFT DC | 28 |
| TFT RST | 2 |
| TFT WR (PIO) | 22 |
| TFT D0–D7 (PIO) | 6–13 |

> **Note:** `relay1Pin`, `relay2Pin`, `ledPin`, and `accPin` in `BoardSettings.h` are currently set to placeholder GPIO 25. Set these to the actual relay control pins for your hardware before use.

---

## LCC Node ID

The node ID is set by `#define NODE_ID` in [`LCC_RPiPico_Turntable.ino`](LCC_RPiPico_Turntable.ino).
The default (`0x050101019411`) is in the Southern Piedmont range assigned to Bob Gamble. Change this to a unique ID from your assigned range before deploying.

---

## Dependencies

Install the following libraries via **Arduino Library Manager** or manually:

| Library | Purpose |
|---|---|
| `ACAN2517` by Pierre Molinaro | MCP2517/2518 CAN transceiver driver |
| `TFT_eSPI` by Bodmer | 800×480 parallel TFT display driver |
| `I2C_eeprom` by Rob Tillaart | 24LC512 EEPROM read/write |
| `NeoPixelBus` | NeoPixel LED control |
| `LibPrintf` | `printf()` support over Serial |
| `PicoOTA` | Over-the-air firmware update (Philhower core) |
| `LittleFS` | Flash filesystem for OTA image staging |
| `Wire` | I2C (built into Arduino core) |
| `SPI` | SPI (built into Arduino core) |

**Local library (do not replace with stock version):**

| Library | Location | Notes |
|---|---|---|
| `AccelStepper` | `src/application_drivers/AccelStepper.cpp/.h` | Modified local copy — contains Pico-specific changes |
| `my_bb_captouch` | `src/application_drivers/my_bb_captouch.cpp/.h` | Capacitive touch input handler |

The OpenLCB stack (`src/openlcb/`, `src/drivers/canbus/`) is included in the repository as the MustangPeak OpenLcbCLib — no separate install needed.

> **Board package:** Use [Earle Philhower's RP2040 package](https://github.com/earlephilhower/arduino-pico#installation) — **not** the Mbed package.
> Board target: `rp2040:rp2040:rpipico2`

---

## Build Configuration

See [`sketch.yaml`](sketch.yaml) for the full build profile:

- Flash: 4 MB - 2 MB filesystem space for firmware updates
- Optimization: `Small`
- C++ standard: `gnu++17`

The `TFT_eSPI` library **must** have a matching `User_Setup.h` configured for the parallel 8-bit bus. Reference pin values are documented in the comments at the bottom of `BoardSettings.h`.

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

- **Core 0** (`setup()` / `loop()`): OpenLCB protocol stack, CAN comms, event handling, touch input (`touchIO()`), serial CLI
- **Core 1** (`setup1()` / `loop1()`): Real-time stepper control (`runStepper()`), homing, calibration, position checking

Key modules:

| File | Role |
|---|---|
| `LCC_RPiPico_Turntable.ino` | Entry point, node init, event registration, producer/consumer functions, serial CLI |
| `Turntable.cpp` | Stepper movement, homing, calibration, phase switching, LED |
| `UserInterface.cpp` | TFT rendering, touch input, track diagram, settings pages |
| `callbacks.cpp` | OpenLCB event handlers, 100ms timer, CAN Rx/Tx, OTA firmware |
| `config_mem_helper.cpp` | EEPROM config storage, CDI memory map, track/step persistence |
| `BoardSettings.h` | All hardware pin and address definitions, stepper and display parameters |
| `TTvariables.h` | Shared type definitions (`TrackAddress`, `LightAddress`, `npStrings`, `HotBox`) |

---

## License

BSD 2-Clause — see [`LICENSE`](LICENSE).
