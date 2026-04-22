/*
 *  TFT_eSPI_RA8876 configuration for v2.7 Stepper board
 *  Panel  : TFTM101 10.1" 1024×600 capacitive touch (BuyDisplay ER-TFTM101-1)
 *  Driver : RA8876 (or LT7683 drop-in), SPI1 interface (RP2040_RA8876 setup)
 *  Touch  : Goodix GT9271, I2C on gp12/gp13 (Wire / I2C0)
 *  Display connected via I/O-2 IDC connector
 *
 *  Defining USER_SETUP_LOADED here prevents TFT_eSPI_RA8876 from reading
 *  its own User_Setup.h.  All required parameters are set below.
 *
 *  Used for DISPLAY_DRIVER_RA8876_TFTESPI mode only.
 *  For DISPLAY_DRIVER_RA8876_NATIVE, this file is NOT included — the
 *  RA8876_RP2040 library is configured programmatically via DisplayDriver.
 *
 *  Included automatically from board_configs/BoardPins_Stepper_v27.h —
 *  do not include directly.
 */

#ifndef DISPLAYCONFIG_RA8876_SPI_H
#define DISPLAYCONFIG_RA8876_SPI_H

#define USER_SETUP_LOADED   // tell TFT_eSPI_RA8876 to skip its own User_Setup.h
#define USER_SETUP_ID 105   // matches RP2040_RA8876 setup

// ---------------------------------------------------------------------------
//  Display driver — TFTM101 uses RA8876 (or LT7683 pin-compatible substitute)
//  Both respond identically to the same SPI command set.
// ---------------------------------------------------------------------------
#define RA8876_DRIVER       // RA8876 / LT7683 controller (SPI protocol, no DC line)

// ---------------------------------------------------------------------------
//  SPI interface — SPI1 via I/O-2 IDC connector
//  NOTE: For RP2040 the SPI port must also be configured in code before
//        tft.init().  setupDisplay() handles this via the
//        DISPLAY_DRIVER_RA8876_TFTESPI guard in UserInterface.cpp.
// ---------------------------------------------------------------------------
#define TFT_SPI_PORT  1     // use hardware SPI1

// Pin names from the *RP2040* perspective (not the display's perspective):
//   GPIO11 = SPI1 TX = MOSI (RP2040 drives data to RA8876 SDI)
//   GPIO8  = SPI1 RX = MISO (RP2040 reads data from RA8876 SDO)
// TFT_eSPI passes TFT_MOSI to spi.setTX() — must be a valid SPI1 TX pin.
#define TFT_MOSI  11        // gp11 — I/O-2 Pin 4 : SPI1 TX → RA8876 SDI
#define TFT_SCLK  10        // gp10 — I/O-2 Pin 3 : SPI1 SCK
#define TFT_MISO  8         // gp8  — I/O-2 Pin 1 : SPI1 RX ← RA8876 SDO
#define TFT_CS    9         // gp9  — I/O-2 Pin 2 : SPI1 CS
#define TFT_RST   14        // gp14 — I/O-2 Pin 9 : Display Reset
#define TFT_DC    -1        // RA8876 uses SPI protocol bytes for cmd/data; no DC pin

// ---------------------------------------------------------------------------
//  Touch — handled separately via my_bb_captouch over I2C0 (gp12/gp13)
//  Controller: Goodix GT9271 — detected as CT_TYPE_GT911 by my_bb_captouch
// ---------------------------------------------------------------------------
#define TOUCH_CS -1

// ---------------------------------------------------------------------------
//  Resolution — TFTM101 10.1" panel, native 1024×600
// ---------------------------------------------------------------------------
#define TFT_WIDTH  1024
#define TFT_HEIGHT  600

// ---------------------------------------------------------------------------
//  Fonts
// ---------------------------------------------------------------------------
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ---------------------------------------------------------------------------
//  SPI clock frequency for RA8876 (max ~40 MHz; start at 20 MHz if unstable)
// ---------------------------------------------------------------------------
#define SPI_FREQUENCY  20000000

#endif  // DISPLAYCONFIG_RA8876_SPI_H
