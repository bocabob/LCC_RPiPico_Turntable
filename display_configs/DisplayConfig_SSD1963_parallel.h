/*
 *  TFT_eSPI_RA8876 configuration for v2.4 Stepper board
 *  Driver : SSD1963 800x480 (Setup104a_RP2040_SSD1963_parallel)
 *  Interface: 8-bit parallel, PIO-driven write strobe
 *
 *  Defining USER_SETUP_LOADED here prevents TFT_eSPI_RA8876 from reading
 *  its own User_Setup.h.  All required parameters are set below.
 *
 *  Included automatically from board_configs/BoardPins_Stepper_v24.h —
 *  do not include directly.
 */

#ifndef DISPLAYCONFIG_SSD1963_PARALLEL_H
#define DISPLAYCONFIG_SSD1963_PARALLEL_H

#define USER_SETUP_LOADED   // tell TFT_eSPI_RA8876 to skip its own User_Setup.h
#define USER_SETUP_ID 104   // matches Setup104a_RP2040_SSD1963_parallel

// ---------------------------------------------------------------------------
//  Display driver
// ---------------------------------------------------------------------------
#define SSD1963_800         // SSD1963 controller, 800x480 resolution

// ---------------------------------------------------------------------------
//  Interface — 8-bit parallel via RP2040 PIO
// ---------------------------------------------------------------------------
#define TFT_PARALLEL_8_BIT  // enable 8-bit parallel data bus
// #define RP2040_PIO_INTERFACE  // uncomment if required by your library version

// ---------------------------------------------------------------------------
//  Control pins
// ---------------------------------------------------------------------------
#define TFT_WR   22   // Write strobe (PIO state machine output)
#define TFT_DC   28   // Data / Command select
#define TFT_CS   -1   // CS tied to GND on board
#define TFT_RST  -1   // RST not connected (use board reset)
#define TFT_RD   -1   // RD tied to 3V3 on board

// ---------------------------------------------------------------------------
//  8-bit parallel data bus — pins MUST be sequential on RP2040
// ---------------------------------------------------------------------------
#define TFT_D0    6
#define TFT_D1    7
#define TFT_D2    8
#define TFT_D3    9
#define TFT_D4   10
#define TFT_D5   11
#define TFT_D6   12
#define TFT_D7   13

// ---------------------------------------------------------------------------
//  Touch — handled separately via my_bb_captouch over I2C; not driven by TFT_eSPI
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

#endif  // DISPLAYCONFIG_SSD1963_PARALLEL_H
