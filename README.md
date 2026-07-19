# LCC_RPiPico_Turntable

An OpenLCB (LCC) node for a model railroad turntable, running on a Raspberry Pi Pico /
Pico 2 (v3.0 generic Node board). It drives a stepper motor to position the bridge,
auto-homes via Hall sensors, switches track phase via relays, and provides a full-color
touch display for local control. It also consumes door-state events from a companion
Roundhouse node to keep the display synchronized, and displays the LCC fast clock.

This project is part of a family of node firmwares sharing a common platform — see
[`LCC_RPiPico_Common/LCC_NODE_STANDARD.md`](../LCC_RPiPico_Common/LCC_NODE_STANDARD.md)
for the cross-project conventions this README assumes, and
[`LCC_RPiPico_Common/CLAUDE.md`](../LCC_RPiPico_Common/CLAUDE.md) as the entry point for
Claude Code sessions working across the family.

**Node ID range:** `05.01.01.01.94.xx` — assigned to Bob Gamble / Southern Piedmont

---

## Features

- **OpenLCB (LCC) node** — full CAN bus integration using the MustangPeak OpenLcbCLib
- **Stepper motor control** — TMC2209 breakout (driven in STEP/DIR mode), configurable
  micro-stepping, acceleration/deceleration via a locally-modified `AccelStepper`
- **Automatic homing** — Hall effect sensor on the home position; bridge position
  sensor for 180° awareness
- **Automatic phase switching** — relay(s) invert track polarity based on bridge angle
  (configurable `AUTO` or `MANUAL`)
- **Up to 20 tracks** — individual Front/Back/Occupancy/RailCom event IDs per track
- **Roundhouse door sync** — consumes paired `DoorOpen`/`DoorClose` command+feedback
  events from the Roundhouse node; door status shown on the display, resynced live on
  every confirmed move and again at LCC login
- **Live bridge animation** — bridge redraws every 1° of stepper rotation during movement
- **LCC Fast Clock consumer** — fast clock displayed on-screen via a compiled-in
  anti-aliased smooth font, survives page redraws and bridge animation
- **NeoPixel bridge lighting** (optional)
- **Persistent state** — track positions, step count, current track, bridge orientation
  stored in external I2C EEPROM (§7 of the Node Standard)
- **Deferred EEPROM writes** — state changes coalesce into a single write ~3 seconds
  after the last event
- **Protected node identity** — node ID survives config wipes/EEPROM_VERSION bumps;
  provisioned via the serial `N`/`Y` command pair (§7.1 of the Node Standard)
- **Factory reset gesture** — hold Blue+Gold for 2s at boot to wipe config to CDI
  defaults without a serial connection (§7.2)
- **OTA firmware update** via LCC datagram/stream protocol (PicoOTA + LittleFS)
- **Serial CLI** for NVM management and diagnostic logging
- **Dual-core operation** — Core 0 handles LCC/CAN/display/touch; Core 1 drives the
  stepper in real time

---

## Board Support

The active hardware target is the **v3.0 generic Node board** — the `STEPPER` family
(v2.4–v2.95, integrated stepper+display) is legacy and frozen; new work uses the Node
board plus a breakout combination. See the Node Standard's §4 (Board Versioning) and
§6 (Breakout Board Catalog) for the full family history.

Edit [`ProjectConfig.h`](ProjectConfig.h) — the **single source of truth** for board
selection:

```cpp
#define LCC_BOARD_NODE_V30       // v3.0 generic NODE board — pick breakout combo in NodeConfig.h
```

[`NodeConfig.h`](NodeConfig.h) then selects exactly one breakout combination:

| Define | Combo | Display | Status |
|---|---|---|---|
| `TURNTABLE_BREAKOUT_SPI_TMC2209` | **Active** | RA8876/LT7381 SPI, capacitive touch, on I/O-1; TMC2209 on I/O-2 | Built and hardware-tested |
| `TURNTABLE_BREAKOUT_PARALLEL_TMC2209` | Alternate | SSD1963 8-bit parallel, capacitive touch, on I/O-1/2/3; TMC2209 combo card | Implemented, less-tested |

`BoardSettings.h` dispatches to `board_configs/BoardPins_Node_v30.h` for physical pin
topology, then `NodeConfig.h` maps that topology to functional signal names for
whichever combo is selected — see the Node Standard §3 for this layering pattern.

---

## Hardware — v3.0, SPI + TMC2209 combo (active)

| Component | Details |
|---|---|
| **MCU** | Raspberry Pi Pico / Pico 2, v3.0 generic Node carrier board |
| **CAN controller** | MCP2517/2518FD on SPI0 (gp0–4) — fixed-function on the Node board |
| **EEPROM** | 24LC256 (32 KB) on I2C1 (gp6/7), address `0x50` |
| **Stepper driver** | TMC2209 breakout on I/O-2, STEP/DIR mode (`STEPPER_DRIVER = A4988_INV`) |
| **Display** | RA8876/LT7381, SPI1 via I/O-1, 1024×600 |
| **Touch controller** | Capacitive, I2C0 (Wire) via I/O-1 pins 7/8 |
| **NeoPixel** | I/O-2 pin 7 |
| **Buttons** | Blue = gp5 (shared with `STEPPER_DIR_PIN` — unavailable on this combo), Gold = gp28 |

### Pin Assignments (SPI + TMC2209 combo)

| Signal | GPIO | Connector |
|---|---|---|
| CAN (MCP2517/18 SPI0) | gp0–4 | fixed |
| EEPROM I2C1 (SDA/SCL) | gp6/gp7 | fixed |
| Display SDI/CS/SCK/SDO (SPI1) | gp8/9/10/11 | I/O-1 pins 1–4 |
| Touch SDA/SCL (I2C0) | gp12/13 | I/O-1 pins 7/8 |
| Display/Touch RST (shared) | gp14 | I/O-1 pin 9 |
| Bridge sensor | gp18 | I/O-2 pin 3 |
| Home sensor | gp19 | I/O-2 pin 4 |
| NeoPixel | gp20 | I/O-2 pin 7 |
| Stepper Enable | gp21 | I/O-2 pin 8 |
| Stepper Step | gp22 | I/O-2 pin 9 |
| Stepper Direction / Blue Button (shared) | gp5 | I/O-2 pin 10 |
| Gold Button | gp28 | I/O-3 pin 5 |

See [`NodeConfig.h`](NodeConfig.h) for the authoritative, commented mapping (including
the alternate parallel-display combo), and the Node Standard's §6.1 for the shared-pin
gotchas found during v3.0 bring-up (shared `DISPLAY_RST`/`TOUCH_RST` resetting the
display controller; a stray-CDI-text bug from an unbounded `CurrentTrack` array index).

---

## LCC Node Identity

The node's LCC node ID is **not** a hardcoded `#define` — it lives in a protected NVM
region that survives config wipes and `EEPROM_VERSION` bumps (Node Standard §7.1). On
an unprovisioned board it falls back to a legacy default and prints a warning:

```cpp
#define NODE_ID_DEFAULT 0x050101019419   // fallback only — provision a real ID below
```

Provision (or re-provision) a node over serial, two-step with confirmation:

```
N050101019419        → node replies "Confirm with 'Y' to write 05:01:01:01:94:19"
Y                    → node writes the identity block and reboots
```

---

## Dependencies

Install the following via **Arduino Library Manager** or manually:

| Library | Purpose |
|---|---|
| `ACAN2517` by Pierre Molinaro | MCP2517/2518 CAN transceiver driver |
| `TFT_eSPI_RA8876` (modified TFT_eSPI fork) | Display driver for the SPI RA8876/LT7381 combo |
| `RA8876_RP2040` | Native RA8876 driver — required only if `DISPLAY_DRIVER_RA8876_NATIVE` is selected |
| `I2C_eeprom` by Rob Tillaart | EEPROM read/write (`USE_TILLAART`) |
| `NeoPixelConnect` | NeoPixel bridge lighting |
| `LibPrintf` | `printf()` support over Serial |
| `PicoOTA` | Over-the-air firmware update (Philhower core) |
| `LittleFS` | Flash filesystem for OTA image staging |
| `Wire`, `SPI` | Built into the Arduino core |

**Local libraries (do not replace with stock versions):**

| Library | Location | Notes |
|---|---|---|
| `AccelStepper` | `src/application_drivers/AccelStepper.cpp/.h` | Modified local copy with Pico-specific changes |
| `my_bb_captouch` | `src/application_drivers/my_bb_captouch.cpp/.h` | Modified `bb_captouch` — configurable Wire instance, RP2040-compatible |

The OpenLCB stack (`src/openlcb/`, `src/drivers/canbus/`) is vendored as the
MustangPeak OpenLcbCLib — see it as a fixed external dependency (Node Standard §10);
do not modify files under `src/`.

> **Board package:** Use [Earle Philhower's RP2040 package](https://github.com/earlephilhower/arduino-pico#installation) — **not** the Mbed package.
> Board target: `rp2040:rp2040:rpipico2`

---

## Build Configuration

See [`sketch.yaml`](sketch.yaml) for the full build profile:

- Flash: 4 MB — 2 MB filesystem space for firmware updates
- Optimization: `Small`
- C++ standard: `gnu++17`

When switching the SPI-combo's display driver between `DISPLAY_DRIVER_RA8876_TFTESPI`
and `DISPLAY_DRIVER_RA8876_NATIVE`, also check `TFT_eSPI_RA8876/User_Setup_LCC_Active.h`
in the library folder matches — Arduino IDE compiles library sources independently of
the sketch directory, so the library needs its own selector file.

---

## Configuration Memory (CDI)

Node configuration (track count, track positions, step count, home track, door count,
event IDs, etc.) is stored in external EEPROM and described by [`CDI.xml`](CDI.xml).
Edit it with any LCC configuration tool (e.g. JMRI's PanelPro) over the LCC bus.

`openlcb_user_config.c`'s compiled `_cdi_data[]` byte array must be kept in sync with
`CDI.xml` by hand any time the XML changes — regenerate it with
[`LCC_RPiPico_Common/cdi_to_c_array.py`](../LCC_RPiPico_Common/cdi_to_c_array.py)
rather than hand-editing the array (see the Node Standard §7 for the exact splicing
procedure).

---

## Serial CLI Commands

Connect at **115200 baud**. Commands common across the node family (Node Standard §11)
plus this project's own:

| Key | Action |
|---|---|
| `h` | Print help |
| `c` | Clear NVM to `0x00` |
| `i` | Reset NVM to CDI default values |
| `r` | Factory reset (NVM to `0xFF`, then reinitialize) |
| `p` | Toggle LCC message logging |
| `m` | Toggle config memory read/write logging |
| `x` | Load application defaults |
| `z` | Re-apply config values from NVM |
| `N` | Provision/re-provision node identity — two-step, confirm with `Y` |

---

## Architecture

The Pico runs two cores (Node Standard §8 dual-core contract):

- **Core 0** (`setup()` / `loop()`): OpenLCB protocol stack, CAN comms, event handling,
  touch input, bridge animation (`updateBridgeAnimation()`), serial CLI
- **Core 1** (`setup1()` / `loop1()`): real-time stepper control (homing, calibration,
  position checking) — never blocks on `delay()`, EEPROM I/O, or CAN traffic

Key modules:

| File | Role |
|---|---|
| `LCC_RPiPico_Turntable.ino` | Entry point: board/display selection, node init, event registration, serial CLI |
| `ProjectConfig.h` | Single switch: board macro + display driver macro |
| `BoardSettings.h` | Dispatches to the matching `board_configs/` header; NVM/storage sizing; global tuning constants |
| `NodeConfig.h` | Functional pin layer: breakout combo selection, display geometry, stepper/touch pin mapping |
| `board_configs/BoardPins_Node_v30.h` | v3.0 physical pin topology (current) |
| `board_configs/BoardPins_Stepper_v24/27/29/295.h` | Legacy Stepper-family pin topology (frozen) |
| `DisplayDriver.h` / `.cpp` | Display abstraction (`TT_Display`) across TFT_eSPI and native RA8876 modes; layer compositing helpers |
| `Turntable.cpp` | Stepper movement, homing, calibration, phase switching, LED |
| `UserInterface.cpp` | TFT rendering, touch input, turntable diagram, bridge animation, track/door/fast-clock display |
| `callbacks.cpp` | OpenLCB event handlers, 100ms timer, CAN Rx/Tx, OTA firmware — the only place consumers/producers are registered (Node Standard §10) |
| `config_mem_helper.cpp` | EEPROM config storage, CDI memory map, track/step persistence |
| `NodeIdentity.h` / `.cpp` | Protected-NVM node identity block (Node Standard §7.1) |
| `TTvariables.h` | Shared type definitions (`TrackAddress`, `LightAddress`, `npStrings`, `HotBox`) |

### Bridge animation

`updateBridgeAnimation()` runs on Core 0 every iteration and redraws whenever the
stepper has moved ≥1° since the last frame — entirely non-blocking. On TFT_eSPI-family
drivers, the bridge erase (`fillCircle()`) also wipes the track lines and the fast
clock's screen region, so both are explicitly redrawn afterward; on the native RA8876
driver, bridge animation only ever touches Layer 1 (hardware-composited over the
static Layer 0 track background), so nothing needs restoring.

---

## License

BSD 2-Clause — see [`LICENSE`](LICENSE).
