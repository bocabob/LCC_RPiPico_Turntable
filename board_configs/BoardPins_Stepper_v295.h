/*
 *  Pin definitions for LCC Node board v2.95 + TMC2209 stepper breakout board
 *
 *  Do not include this file directly — it is selected automatically
 *  by BoardSettings.h based on the LCC_BOARD_STEPPER_V295 define in ProjectConfig.h.
 *
 *  Hardware configuration:
 *    Node board v2.95  — base board; provides CAN, I2C, buttons, I/O-1/2/3 headers
 *    TMC2209 breakout  — plugs into I/O-2; provides stepper EN/STEP/DIR,
 *                        sensors, NeoPixel, and I2C passthrough (IC2 connector)
 *    Display (RA8876)  — plugs into I/O-1 (SPI1, gp8-14); same wiring as v2.7
 *
 *  Key differences from v2.9:
 *    - TMC2209 breakout on I/O-2 (not I/O-1): sensors gp18/19, NeoPixel gp20,
 *      stepper EN/STEP/DIR gp21/22/26
 *    - Display on I/O-1 (SPI1, gp8-14) — no SPI conflict with CAN
 *    - Touch controller on gp12/13 (I2C0, Wire) via I/O-1:Pin7/8
 *    - Display reset on gp14 (I/O-1:Pin9) — same as v2.7
 *    - I2C storage (EEPROM) on Node board I2C1 header gp6/gp7 (Wire1)
 *    - Breakout I2C passthrough (IC2 connector) on gp16/17 (I2C0, Wire) —
 *      not currently used by firmware; available for future I2C0 peripherals
 *
 *  *** NOTE: Blue and Gold buttons (gp21/gp22) are shared with stepper
 *      Enable and Step pins.  Both buttons are unavailable on this variant.
 */

#ifndef BOARDPINS_STEPPER_V295_H
#define BOARDPINS_STEPPER_V295_H

// --------------------------------------------
//  Sensors — TMC2209 breakout I/O-2:Pin3/4
// --------------------------------------------
#define BRIDGE_SENSOR_PIN   18  // I/O-2:Pin3
#define HOME_SENSOR_PIN     19  // I/O-2:Pin4

// --------------------------------------------
//  Stepper driver — TMC2209 (step/dir mode)
//  Connected via TMC2209 breakout I/O-2:Pin8/9/10
//  NOTE: gp21 (EN) and gp22 (STEP) are shared with the
//  Blue and Gold button pads — buttons unavailable on this variant.
// --------------------------------------------
#define STEPPER_INTERFACE       1
#define STEPPER_ENABLE_PIN      21  // I/O-2:Pin8  (shared with Blue Button pad)
#define STEPPER_STEP_PIN        22  // I/O-2:Pin9  (shared with Gold Button pad)
#define STEPPER_DIR_PIN         26  // I/O-2:Pin10

// --------------------------------------------
//  NeoPixel — TMC2209 breakout I/O-2:Pin7
// --------------------------------------------
#define NeoPixel_PinA   20

// --------------------------------------------
//  Touch controller (I2C0 = Wire, gp12/gp13)
//  Connected via display board I/O-1:Pin7/8 — same as v2.7
// --------------------------------------------
#define TOUCH_SDA   12  // I/O-1:Pin7
#define TOUCH_SCL   13  // I/O-1:Pin8
#define TOUCH_INT   -1  // not connected
#define TOUCH_RST   -1
#define TOUCH_WIRE  Wire

// --------------------------------------------
//  I2C storage — EEPROM (I2C1 = Wire1, gp6/gp7)
//  Node board dedicated I2C1 header
// --------------------------------------------
#define I2C_SDA     6
#define I2C_SCL     7
#define STOR_WIRE   Wire1

// --------------------------------------------
//  Breakout I2C passthrough (I2C0 = Wire, gp16/gp17)
//  I/O-2:Pin1 (SDA) / Pin2 (SCL) — TMC2209 breakout IC2 connector
//  Not currently used by firmware; available for future I2C0 peripherals
// --------------------------------------------
// #define BREAKOUT_I2C_SDA  16
// #define BREAKOUT_I2C_SCL  17

// --------------------------------------------
//  CAN transceiver — MCP2517/18 via SPI0 (gp0-4)
//  Same as v2.9
// --------------------------------------------
#define MCP2517_SPI SPI
#define MCP2517_SDO 0   // MISO from MCP2517 (ACAN_RX_PIN)
#define MCP2517_CS  1   // ACAN_CS_PIN
#define MCP2517_SCK 2   // ACAN_CSK_PIN
#define MCP2517_SDI 3   // MOSI to MCP2517  (ACAN_TX_PIN)
#define MCP2517_INT 4   // ACAN_INT_PIN

// --------------------------------------------
//  Relay / LED / accessory outputs
// --------------------------------------------
const uint8_t relay1Pin = 25;
const uint8_t relay2Pin = 25;
const uint8_t ledPin    = 25;
const uint8_t accPin    = 25;

// --------------------------------------------
//  Buttons — NOT AVAILABLE on this variant
//  gp21 and gp22 are used by the TMC2209 breakout
//  for stepper Enable and Step respectively.
// --------------------------------------------
// #define BLUE_BUTTON_PIN   21  // unavailable — used for STEPPER_ENABLE_PIN
// #define GOLD_BUTTON_PIN   22  // unavailable — used for STEPPER_STEP_PIN

// --------------------------------------------
//  Reserved pins
// --------------------------------------------
#define TT_PIN1     5   // gp5 — header pin, purpose TBD
// gp27: I/O-3:Pin2  — TBD
// gp28: I/O-3:Pin5  — TBD

// --------------------------------------------
//  Display geometry & orientation — TFTM101 10.1" (RA8876, 1024×600)
//  See v2.7 header for RA8876 rotation notes.
// --------------------------------------------
#define HRES           1024
#define VRES            600
#define ROTATION          0
#define TOUCH_ROTATION  180

// --------------------------------------------
//  Display — SPI1 via I/O-1 IDC connector (same as v2.7)
//    SPI1 SDI (display input)  = gp8  (I/O-1:Pin1) — RP2040 SPI1 RX / MISO
//    SPI1 CS                   = gp9  (I/O-1:Pin2)
//    SPI1 CLK                  = gp10 (I/O-1:Pin3)
//    SPI1 SDO (display output) = gp11 (I/O-1:Pin4) — RP2040 SPI1 TX / MOSI
//    Display Reset             = gp14 (I/O-1:Pin9)
//  No SPI conflict: CAN uses SPI0 (gp0-4), display uses SPI1 (gp8-11).
// --------------------------------------------
#define DISPLAY_SPI     SPI1
#define DISPLAY_SDI     8   // I/O-1:Pin1 — RP2040 SPI1 RX (MISO)
#define DISPLAY_CS      9   // I/O-1:Pin2
#define DISPLAY_SCK     10  // I/O-1:Pin3
#define DISPLAY_SDO     11  // I/O-1:Pin4 — RP2040 SPI1 TX (MOSI)
#define DISPLAY_RST     14  // I/O-1:Pin9

// --------------------------------------------
//  Display driver selection (same logic as v2.7/v2.9)
// --------------------------------------------
#if defined(DISPLAY_DRIVER_RA8876_NATIVE)
  // Native RA8876_RP2040 library — no TFT_eSPI USER_SETUP_LOADED needed.
#else
  #if !defined(DISPLAY_DRIVER_RA8876_TFTESPI)
    #define DISPLAY_DRIVER_RA8876_TFTESPI
  #endif
#endif

#endif  // BOARDPINS_STEPPER_V295_H
