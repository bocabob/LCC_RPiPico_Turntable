/*
 * NodeConfig.h — functional pin assignment layer for the v3.0 generic
 * NODE board (LCC_BOARD_NODE_V30). Maps the physical IOx_PINy connector
 * pins from board_configs/BoardPins_Node_v30.h to functional signal names,
 * depending on which breakout combination is selected below.
 *
 * Only included when LCC_BOARD_NODE_V30 is active (see BoardSettings.h).
 * STEPPER-family boards (v2.4–v2.95) define these functional macros
 * directly in their own board_configs/BoardPins_Stepper_*.h header instead —
 * this file only exists for the generic v3.0+ breakout-based architecture.
 *
 * Select exactly ONE breakout combination below (uncomment in ProjectConfig.h
 * or directly here — see comment at each #define).
 */

#ifndef NODECONFIG_H
#define NODECONFIG_H

// ---- Step 1b: breakout combination (uncomment exactly ONE) ----------------
#define TURNTABLE_BREAKOUT_SPI_TMC2209       // SPI display (cap touch) on I/O-1, TMC2209 stepper breakout on I/O-2
// #define TURNTABLE_BREAKOUT_PARALLEL_TMC2209  // Parallel display (cap touch) + TMC2209 combo card on I/O-1/2/3

#if !defined(TURNTABLE_BREAKOUT_SPI_TMC2209) && !defined(TURNTABLE_BREAKOUT_PARALLEL_TMC2209)
  #error "Define exactly one breakout combination in NodeConfig.h: TURNTABLE_BREAKOUT_SPI_TMC2209 or TURNTABLE_BREAKOUT_PARALLEL_TMC2209."
#endif

// =============================================================================
//  Combo A — SPI display (capacitive touch) on I/O-1 + TMC2209 on I/O-2
//  Per LCC_NODE_STANDARD.md §6.1 "SPI Display — Capacitive Touch" + "TMC2209
//  stepper breakout" tables. No pin conflicts on the v3.0 board.
// =============================================================================
#if defined(TURNTABLE_BREAKOUT_SPI_TMC2209)

  // ---- Display (RA8876/LT7381 native, SPI1 via I/O-1) ----
  #define DISPLAY_SPI     SPI1
  #define DISPLAY_SDI     IO1_PIN1   // gp8  — D_SDO/RX (RP2040 SPI1 RX/MISO)
  #define DISPLAY_CS      IO1_PIN2   // gp9  — D_CS
  #define DISPLAY_SCK     IO1_PIN3   // gp10 — D_CLK
  #define DISPLAY_SDO     IO1_PIN4   // gp11 — D_SDI/TX (RP2040 SPI1 TX/MOSI)
  #define DISPLAY_RST     IO1_PIN9   // gp14 — D_RST / T_RST (shared with touch reset)

  // ---- Touch (capacitive, I2C via I/O-1 pins 7/8) ----
  #define TOUCH_SDA       IO1_PIN7   // gp12 — T_SDA
  #define TOUCH_SCL       IO1_PIN8   // gp13 — T_SCL
  #define TOUCH_RST       IO1_PIN9   // gp14 — shared with DISPLAY_RST
  #define TOUCH_INT       -1         // not connected on this breakout — must be -1, not UNUSED_PIN(127): my_bb_captouch.cpp's GT911 sleep/wake path checks "_iINT != -1" specifically, and would otherwise try to drive pin 127 as a real GPIO
  #define TOUCH_WIRE      Wire       // I2C0

  // ---- Stepper (TMC2209 breakout on I/O-2) ----
  #define STEPPER_INTERFACE     1
  #define BRIDGE_SENSOR_PIN     IO2_PIN3   // gp18
  #define HOME_SENSOR_PIN       IO2_PIN4   // gp19
  #define NeoPixel_PinA         IO2_PIN7   // gp20
  #define STEPPER_ENABLE_PIN    IO2_PIN8   // gp21
  #define STEPPER_STEP_PIN      IO2_PIN9   // gp22
  #define STEPPER_DIR_PIN       IO2_PIN10  // gp5  — *** shared with BLUE_BUTTON_PIN on v3.0 *** Blue button unavailable on this combo.

  // ---- Display geometry — confirm against the populated panel ----
  #define HRES           1024
  #define VRES            600
  #define ROTATION          0
  #define TOUCH_ROTATION  180

  // ---- Display driver selection (same logic as v2.7/v2.9/v2.95) ----
  #if defined(DISPLAY_DRIVER_RA8876_NATIVE)
    // Native RA8876_RP2040 library — no TFT_eSPI USER_SETUP_LOADED needed.
  #else
    #if !defined(DISPLAY_DRIVER_RA8876_TFTESPI)
      #define DISPLAY_DRIVER_RA8876_TFTESPI
    #endif
  #endif

  // ---- Relay / LED / accessory outputs — TBD for v3.0, confirm wiring ----
  const uint8_t relay1Pin = 25;
  const uint8_t relay2Pin = 25;
  const uint8_t ledPin    = 25;
  const uint8_t accPin    = 25;

#endif  // TURNTABLE_BREAKOUT_SPI_TMC2209

// =============================================================================
//  Combo B — Parallel display (capacitive touch) + TMC2209 combo card
//  Re-derived 2026-07-01 directly from the revised breakout schematic
//  (Display090AdapterB.kicad_sch) after a board revision moved STEPPER_DIR_PIN
//  off gp5 onto I/O-3:Pin1 (gp26), freeing gp5/BLUE_BUTTON_PIN entirely.
// =============================================================================
#if defined(TURNTABLE_BREAKOUT_PARALLEL_TMC2209)

  // ---- Display (SSD1963, 8-bit parallel via I/O-1 + I/O-3) ----
  #define DISPLAY_DRIVER_SSD1963_PARALLEL_V30
  // TFT_D0-D7 / TFT_WR / TFT_DC / TFT_CS / TFT_RST / TFT_RD are defined in
  // tft_setup.h (sketch root), auto-discovered by vanilla TFT_eSPI so every
  // translation unit — including the library's own .cpp — sees the same
  // pins. See tft_setup.h for the confirmed schematic-derived values:
  // no CS or RD signal exists on this breakout at all (tied off on the
  // display module itself), and D_RST is wired to a physical switch only,
  // not to any Pico GPIO.

  // ---- Touch (capacitive, I2C via I/O-2 pins 1/2) ----
  #define TOUCH_SDA       IO2_PIN1   // gp16 — T_SDA
  #define TOUCH_SCL       IO2_PIN2   // gp17 — T_SCL
  #define TOUCH_WIRE      Wire       // I2C0
  // Not connected on this breakout — must be -1, not UNUSED_PIN(127): see
  // the matching comment on Combo A's TOUCH_INT above for why (-1 is the
  // touch library's own sentinel, used by both setupDisplay()'s tp.init()
  // call and the GT911 sleep/wake path in my_bb_captouch.cpp).
  // T_RST is wired to a physical switch on the breakout, not a GPIO — same
  // as D_RST above, confirmed via the revised schematic.
  // T_INT is wired to gp5 on the revised (production) breakout schematic,
  // but NOT soldered on this test board — left at -1 to match actual test
  // hardware. Functionally moot either way: my_bb_captouch.cpp only reads
  // TOUCH_INT for the CHSC6540 chip type, and this touch controller
  // resolves as "Unknown" — GT911's use of INT (address-select at init,
  // sleep/wake) doesn't gate touch detection. Revisit if a future touch
  // chip on this breakout needs INT and the wire gets soldered.
  #define TOUCH_RST       -1
  #define TOUCH_INT       -1

  // ---- Sensors (TMC2209 combo card, I/O-2 pins 3/4) ----
  #define BRIDGE_SENSOR_PIN     IO2_PIN3   // gp18
  #define HOME_SENSOR_PIN       IO2_PIN4   // gp19

  // ---- NeoPixel + stepper (TMC2209 combo card, I/O-2 pins 7-9 + I/O-3 pin 1) ----
  #define NeoPixel_PinA         IO2_PIN7   // gp20
  #define STEPPER_INTERFACE     1
  #define STEPPER_ENABLE_PIN    IO2_PIN8   // gp21
  #define STEPPER_STEP_PIN      IO2_PIN9   // gp22
  #define STEPPER_DIR_PIN       IO3_PIN1   // gp26 — moved off gp5 in the board revision; BLUE_BUTTON_PIN is now unshared on this combo.

  // ---- Display control (I/O-3 pins 2/5) ----
  // TFT_WR/TFT_DC are also set in tft_setup.h — listed here too since other
  // node code may reference them by these board-level names.
  #define DISPLAY_WR_PIN        IO3_PIN2   // gp27 — D_WR
  #define DISPLAY_DC_PIN        IO3_PIN5   // gp28 — D_D/C — *** shared with GOLD_BUTTON_PIN *** Standard design assumption (per LCC_NODE_STANDARD.md): Blue/Gold buttons are read pre-init for the factory-reset gesture, then may be repurposed for normal operation if a combo's wiring allows. Gold permanently conflicts with TFT_DC on this combo, so it stays pre-init-only; Blue has no conflict on the revised board and is available post-init.

  // ---- Display geometry — confirm against the populated panel ----
  // SSD1963_800BD_DRIVER's native TFT_WIDTH/TFT_HEIGHT are 480x800 (portrait)
  // per TFT_Drivers/SSD1963_Defines.h. ROTATION=3 swaps _width/_height to
  // 800x480 to match HRES/VRES below — same landscape rotation value the
  // v2.4 board uses for this identical panel/driver (BoardPins_Stepper_v24.h).
  // Confirmed working 2026-07-01 after the display init fix (Setup104b).
  #define HRES           800
  #define VRES            480
  #define ROTATION          3
  #define TOUCH_ROTATION    0

  // ---- Relay / LED / accessory outputs — TBD for v3.0, confirm wiring ----
  const uint8_t relay1Pin = 25;
  const uint8_t relay2Pin = 25;
  const uint8_t ledPin    = 25;
  const uint8_t accPin    = 25;

#endif  // TURNTABLE_BREAKOUT_PARALLEL_TMC2209

#endif  // NODECONFIG_H
