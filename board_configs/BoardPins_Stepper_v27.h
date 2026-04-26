/*
 *  Pin definitions for LCC RPi Pico Stepper board v2.7
 *
 *  Do not include this file directly — it is selected automatically
 *  by BoardSettings.h based on the LCC_BOARD_* define in the .ino file.
 *
 *  Key differences from v2.4:
 *    - Stepper Enable/Step moved to gp6/gp7 (freed gp14/gp15 for I/O-2)
 *    - NeoPixel moved to gp3 (freed gp21 for Blue Button)
 *    - Touch moved to gp12/gp13 (I2C0), no dedicated INT pin
 *    - Display reset on gp14
 *    - Display uses SPI1 via I/O-2 IDC connector (TFT_eSPI_RA8876)
 *    - Blue/Gold buttons on dedicated pins gp21/gp22
 *    - CAN (gp16-20) and I2C storage (gp26/27) unchanged
 */

#ifndef BOARDPINS_STEPPER_V27_H
#define BOARDPINS_STEPPER_V27_H

// --------------------------------------------
//  Sensors
// --------------------------------------------
#define BRIDGE_SENSOR_PIN   0   // Bridge position sensor
#define HOME_SENSOR_PIN     1   // Home position sensor

// --------------------------------------------
//  Stepper driver (A4988 / DRV8825 style)
// --------------------------------------------
#define STEPPER_INTERFACE       1
#define STEPPER_DIR_PIN         2
#define STEPPER_ENABLE_PIN      6
#define STEPPER_STEP_PIN        7

// --------------------------------------------
//  NeoPixel
// --------------------------------------------
#define NeoPixel_PinA   3

// --------------------------------------------
//  Touch controller (I2C0 = Wire, gp12/gp13)
//  Connected via I/O-2 IDC connector pins 7/8
// --------------------------------------------
#define TOUCH_SDA   12
#define TOUCH_SCL   13
#define TOUCH_INT   -1      // not connected on v2.7
#define TOUCH_RST   -1
#define TOUCH_WIRE  Wire

// --------------------------------------------
//  I2C storage — EEPROM (I2C1 = Wire1, gp26/gp27)
// --------------------------------------------
#define I2C_SDA     26
#define I2C_SCL     27
#define STOR_WIRE   Wire1

// --------------------------------------------
//  CAN transceiver — MCP2517/18 via SPI0 (unchanged from v2.4)
// --------------------------------------------
#define MCP2517_SPI SPI
#define MCP2517_SDO 16  // MISO from MCP2517
#define MCP2517_CS  17
#define MCP2517_SCK 18
#define MCP2517_SDI 19  // MOSI to MCP2517
#define MCP2517_INT 20

// --------------------------------------------
//  Relay / LED / accessory outputs
// --------------------------------------------
const uint8_t relay1Pin = 25;
const uint8_t relay2Pin = 25;
const uint8_t ledPin    = 25;
const uint8_t accPin    = 25;

// --------------------------------------------
//  Buttons (dedicated pins, not shared with I2C)
// --------------------------------------------
#define BLUE_BUTTON_PIN     21
#define GOLD_BUTTON_PIN     22

// --------------------------------------------
//  Display geometry & orientation — TFTM101 10.1" (RA8876, 1024×600)
// --------------------------------------------
#define HRES           1024   // landscape pixel width
#define VRES            600   // landscape pixel height
// Rotation conventions for RA8876:
//   The TFTM101 panel is natively 1024×600 landscape. RA8876 rotation maps
//   directly to DPCR scan-direction bits — it does NOT do a row/column swap
//   like MIPI DCS controllers (ILI9488, ST7796, etc.).
//
//   Rotation 0 → DPCR=0xC0: L→R, T→B — normal landscape scan. CORRECT.
//   Rotation 3 → DPCR=0xC8: L→R, B→T (VSCAN inverted) — image appears blank
//     or vertically corrupted. WRONG for this panel.
//
//   Both TFT_eSPI_RA8876 and RA8876_RP2040 native use rotation 0.
//   If the image appears 180°-rotated use rotation 2 (DPCR=0xD8: R→L, B→T).
#define ROTATION  0   // both drivers: RA8876 natively-landscape panel, normal scan
#define TOUCH_ROTATION  180   // my_bb_captouch / GT9271 orientation (180° = panel mounted inverted)
// Touch controller: Goodix GT9271 (10-point capacitive, I2C on gp12/gp13)
// my_bb_captouch probes GT911_ADDR1 (0x5D) / GT911_ADDR2 (0x14) and detects
// GT9271 as CT_TYPE_GT911 — same I2C protocol, no code change needed.

// --------------------------------------------
//  Display — SPI1 via I/O-2 IDC connector
//    SPI1 SDI (display input)  = gp8  (I/O-2 Pin 1) — RP2040 SPI1 RX / MISO
//    SPI1 CS                   = gp9  (I/O-2 Pin 2)
//    SPI1 CLK                  = gp10 (I/O-2 Pin 3)
//    SPI1 SDO (display output) = gp11 (I/O-2 Pin 4) — RP2040 SPI1 TX / MOSI
//    Display Reset             = gp14 (I/O-2 Pin 9)
//
//  NOTE: SDI/SDO labels are from the *display's* perspective.
//  From the RP2040's SPI1 perspective: TX(MOSI)=GPIO11, RX(MISO)=GPIO8.
//  DISPLAY_DECLARE_TFT passes SDO as mosi and SDI as miso accordingly.
// --------------------------------------------
#define DISPLAY_SPI     SPI1
#define DISPLAY_SDI     8   // display Serial Data In  — RP2040 SPI1 RX (MISO)
#define DISPLAY_CS      9
#define DISPLAY_SCK     10
#define DISPLAY_SDO     11  // display Serial Data Out — RP2040 SPI1 TX (MOSI)
#define DISPLAY_RST     14

// --------------------------------------------
//  Display driver selection
//
//  Default: DISPLAY_DRIVER_RA8876_TFTESPI (TFT_eSPI_RA8876 library, SPI RA8876)
//  Override: define DISPLAY_DRIVER_RA8876_NATIVE in the .ino file BEFORE the
//  board #define to use the native RA8876_RP2040 library instead.  The native
//  mode unlocks layer compositing and BTE acceleration (see DisplayDriver.h).
//
//  DISPLAY_DRIVER_RA8876_NATIVE also requires alternate UserInterface paths —
//  these are handled automatically via the same define.
// --------------------------------------------
#if defined(DISPLAY_DRIVER_RA8876_NATIVE)
  // Native RA8876_RP2040 library — no TFT_eSPI USER_SETUP_LOADED needed;
  // SPI1 pin defines above are used directly by DisplayDriver.cpp.
#else
  // TFT_eSPI_RA8876 (default): matching DisplayConfig header included by
  // BoardSettings.h (sketch root) so "display_configs/" resolves correctly.
  #if !defined(DISPLAY_DRIVER_RA8876_TFTESPI)
    #define DISPLAY_DRIVER_RA8876_TFTESPI
  #endif
#endif

#endif  // BOARDPINS_STEPPER_V27_H
