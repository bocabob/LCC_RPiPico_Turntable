/*
 *  TFT_eSPI_RA8876 configuration for v3.0 Node board + Parallel Display
 *  (Capacitive Touch) breakout — combo with TMC2209 stepper breakout.
 *  Driver : SSD1963 800x480 (Setup104a_RP2040_SSD1963_parallel)
 *  Interface: 8-bit parallel, PIO-driven write strobe
 *
 *  Pin assignments per LCC_NODE_STANDARD.md §6.1 "Parallel Display —
 *  Capacitive Touch" breakout table, corrected per the v3.0 board's actual
 *  I/O-3 wiring (D_D/C is I/O-3:Pin5 / gp28; I/O-3:Pin4 is VREF, not a GPIO).
 *
 *  Defining USER_SETUP_LOADED here prevents TFT_eSPI_RA8876 from reading
 *  its own User_Setup.h.  All required parameters are set below.
 *
 *  Included automatically from NodeConfig.h when
 *  TURNTABLE_BREAKOUT_PARALLEL_TMC2209 is selected — do not include directly.
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
#define SSD1963_800BD_DRIVER  // SSD1963 800×480 (BuyDisplay / compatible panel)

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
