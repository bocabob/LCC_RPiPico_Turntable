/*
 *  SUPERSEDED / UNUSED — kept only for historical reference.
 *
 *  This file is no longer included from anywhere. The v3.0 parallel-display
 *  combo now uses vanilla TFT_eSPI (see DisplayDriver.h) configured via
 *  tft_setup.h (sketch root) and Setup104b_RP2040_SSD1963_parallel.h in the
 *  TFT_eSPI library itself — not TFT_eSPI_RA8876 as this file assumes.
 *  BoardSettings.h explicitly does not include this file for either
 *  DISPLAY_DRIVER_SSD1963_PARALLEL or DISPLAY_DRIVER_SSD1963_PARALLEL_V30
 *  (see the comment there). The SSD1963_DRIVER-vs-SSD1963_800BD_DRIVER
 *  root-cause note below was accurate for the TFT_eSPI_RA8876 library at the
 *  time it was written, but is not relevant to the current vanilla-TFT_eSPI
 *  setup, which never reads this file.
 *
 *  Original header (TFT_eSPI_RA8876 configuration for v3.0 Node board +
 *  Parallel Display / Capacitive Touch breakout, TMC2209 stepper combo,
 *  SSD1963 800x480, 8-bit parallel PIO-driven write strobe) follows:
 */

#ifndef DISPLAYCONFIG_SSD1963_PARALLEL_V30_H
#define DISPLAYCONFIG_SSD1963_PARALLEL_V30_H

#define USER_SETUP_LOADED   // tell TFT_eSPI_RA8876 to skip its own User_Setup.h
#define USER_SETUP_ID 104   // matches Setup104a_RP2040_SSD1963_parallel

// ---------------------------------------------------------------------------
//  Display driver
// ---------------------------------------------------------------------------
// Must use one of the recognised _DRIVER variants — the library's User_Setup_Select.h
// (which maps bare names like SSD1963_800 → SSD1963_800_DRIVER) is bypassed entirely
// when USER_SETUP_LOADED is defined.  SSD1963_800BD_DRIVER matches the timing in
// Setup104a_RP2040_SSD1963_parallel.h that this config was derived from.
//
// IMPORTANT: TFT_eSPI_RA8876.cpp's init()/setRotation() only branch on the bare
// "SSD1963_DRIVER" macro (not "_800BD_DRIVER" or any other variant) to decide
// whether to run the SSD1963 init command sequence at all. User_Setup_Select.h
// only ever maps SSD1963_800BD_DRIVER to a cosmetic TFT_DRIVER ID — it never
// defines SSD1963_DRIVER itself. Without it, init() sends zero SSD1963 commands,
// the PIO parallel bus never receives data, and the CS_H macro's WAIT_FOR_STALL
// busy-wait (Processors/TFT_eSPI_RP2040.h) then blocks forever — this was the
// root cause of the silent hang inside tft.init() on the v3.0 parallel breakout.
#define SSD1963_800BD_DRIVER  // SSD1963 800×480 (BuyDisplay / compatible panel)
#define SSD1963_DRIVER        // required: this is what TFT_eSPI_RA8876.cpp actually checks

// ---------------------------------------------------------------------------
//  Interface — 8-bit parallel via RP2040 PIO
// ---------------------------------------------------------------------------
#define TFT_PARALLEL_8_BIT  // enable 8-bit parallel data bus
// #define RP2040_PIO_INTERFACE  // uncomment if required by your library version

// ---------------------------------------------------------------------------
//  Control pins — I/O-3:Pin2 (D_WR) / I/O-3:Pin5 (D_D/C)
// ---------------------------------------------------------------------------
#define TFT_WR   27   // I/O-3:Pin2 — Write strobe (PIO state machine output)
#define TFT_DC   28   // I/O-3:Pin5 — Data / Command select (shared with GOLD_BUTTON_PIN — unavailable on this combo)
#define TFT_CS   -1   // CS tied to GND on board
#define TFT_RST  -1   // RST not connected (use board reset)
#define TFT_RD   -1   // RD tied to 3V3 on board

// ---------------------------------------------------------------------------
//  8-bit parallel data bus — I/O-1:Pin1-4,7-10 (gp8-15) — sequential on RP2040
// ---------------------------------------------------------------------------
#define TFT_D0    8   // I/O-1:Pin1
#define TFT_D1    9   // I/O-1:Pin2
#define TFT_D2   10   // I/O-1:Pin3
#define TFT_D3   11   // I/O-1:Pin4
#define TFT_D4   12   // I/O-1:Pin7
#define TFT_D5   13   // I/O-1:Pin8
#define TFT_D6   14   // I/O-1:Pin9
#define TFT_D7   15   // I/O-1:Pin10

// ---------------------------------------------------------------------------
//  Touch — capacitive, I2C via I/O-2:Pin1/2 (T_SDA/T_SCL); not driven by TFT_eSPI.
//  TOUCH_CS = -1 disables the TFT_eSPI touch driver without triggering pin macros.
// ---------------------------------------------------------------------------
#define TOUCH_CS -1

// ---------------------------------------------------------------------------
//  Fonts
// ---------------------------------------------------------------------------
#define LOAD_GLCD    // Font 1 —  8px
#define LOAD_FONT2   // Font 2 — 16px
#define LOAD_FONT4   // Font 4 — 26px
#define LOAD_FONT6   // Font 6 — 48px
#define LOAD_FONT7   // Font 7 — 7-segment 48px
#define LOAD_FONT8   // Font 8 — 75px
#define LOAD_GFXFF   // Adafruit GFX free fonts
#define SMOOTH_FONT  // enable anti-aliased smooth fonts

// ---------------------------------------------------------------------------
//  Resolution (SSD1963_800 sets 800×480; explicit here for clarity)
// ---------------------------------------------------------------------------
#define TFT_WIDTH  800
#define TFT_HEIGHT 480

// ---------------------------------------------------------------------------
//  SPI frequency (not used for parallel but may be required by the library)
// ---------------------------------------------------------------------------
#define SPI_FREQUENCY  40000000

#endif  // DISPLAYCONFIG_SSD1963_PARALLEL_V30_H
