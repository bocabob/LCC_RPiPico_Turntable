/*
 * tft_setup.h — auto-discovered by vanilla TFT_eSPI's TFT_eSPI.h via
 * __has_include(<tft_setup.h>) on the include path (the sketch folder is
 * always on that path in Arduino builds).
 *
 * Why this file exists: TFT_eSPI.cpp is its own translation unit and never
 * sees macros defined only via our sketch's BoardSettings.h include chain.
 * Without this file, the library's .cpp falls back to whatever is active
 * in its own User_Setup_Select.h (currently Setup104a_RP2040_SSD1963_parallel.h,
 * the v2.4 board's pins) regardless of which board is actually selected in
 * ProjectConfig.h — a silent mismatch that caused a real hang in tft.init()
 * on the v3.0 parallel breakout. Including ProjectConfig.h here (not the
 * full BoardSettings.h chain, to avoid re-entering display-config dispatch
 * while DisplayDriver.h's own include of <TFT_eSPI.h> is still in progress)
 * gives this file — and therefore every translation unit, library included —
 * a consistent view of which board is active.
 */

#ifndef TFT_SETUP_H
#define TFT_SETUP_H

#include "ProjectConfig.h"

#define USER_SETUP_LOADED
#define USER_SETUP_ID 104

#define TFT_PARALLEL_8_BIT
#define SSD1963_800BD_DRIVER
#define TFT_RGB_ORDER TFT_RGB

#if defined(LCC_BOARD_NODE_V30)
  // v3.0 parallel breakout — pins re-derived 2026-07-01 directly from the
  // revised schematic (Display090AdapterB.kicad_sch): no CS or RD signal
  // exists on this breakout at all (tied off on the display module itself),
  // and D_RST/T_RST are wired to a local switch only — not to any Pico
  // GPIO. TFT_RST=-1 is therefore correct here (unlike v2.4, which has a
  // real GPIO-driven reset and needs a real pin number).
  #define TFT_DC   28   // I/O-3:Pin5 — D_D/C
  #define TFT_WR   27   // I/O-3:Pin2 — D_WR
  // TFT_CS, TFT_RST, TFT_RD intentionally left undefined (-1 equivalent):
  // no GPIO drives any of them on this breakout.

  #define TFT_D0    8   // I/O-1:Pin1
  #define TFT_D1    9   // I/O-1:Pin2
  #define TFT_D2   10   // I/O-1:Pin3
  #define TFT_D3   11   // I/O-1:Pin4
  #define TFT_D4   12   // I/O-1:Pin7
  #define TFT_D5   13   // I/O-1:Pin8
  #define TFT_D6   14   // I/O-1:Pin9
  #define TFT_D7   15   // I/O-1:Pin10
#else
  // v2.4 board — same pins as Setup104a_RP2040_SSD1963_parallel.h, kept
  // here too so this file is the single consistent source for every
  // translation unit once it exists on the include path.
  #define TFT_DC   28
  #define TFT_RST   2
  #define TFT_WR   22

  #define TFT_D0    6
  #define TFT_D1    7
  #define TFT_D2    8
  #define TFT_D3    9
  #define TFT_D4   10
  #define TFT_D5   11
  #define TFT_D6   12
  #define TFT_D7   13
#endif

#define DISABLE_ALL_LIBRARY_WARNINGS

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

#endif // TFT_SETUP_H
