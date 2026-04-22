/*
 *  DisplayDriver.h — Unified display abstraction for LCC RPi Pico Turntable
 *
 *  Three display modes are supported, selected by a DISPLAY_DRIVER_* define
 *  in the active board_configs/BoardPins_*.h (set automatically) or overridden
 *  in the .ino file before the board #define:
 *
 *  DISPLAY_DRIVER_SSD1963_PARALLEL   (v2.4 board, default)
 *      TFT_eSPI_RA8876 library, Setup104a_RP2040_SSD1963_parallel
 *      8-bit parallel bus, gp6-13 data, gp22 WR, gp28 DC
 *      No wrapper — TT_Display is a typedef for TFT_eSPI.
 *
 *  DISPLAY_DRIVER_RA8876_TFTESPI     (v2.7 board, default)
 *      TFT_eSPI_RA8876 library, RP2040_RA8876 SPI setup
 *      SPI1 bus: MOSI=gp8, SCK=gp10, MISO=gp11, CS=gp9, RST=gp14
 *      No wrapper — TT_Display is a typedef for TFT_eSPI.
 *      Note: caller must init SPI1 pins before tft.init() — see setupDisplay().
 *
 *  DISPLAY_DRIVER_RA8876_NATIVE      (v2.7 board, optional)
 *      RA8876_RP2040 library (native, not TFT_eSPI wrapper)
 *      Same SPI1 pins as above.
 *      TT_Display is a compatibility wrapper class that presents the same
 *      interface as TFT_eSPI so UserInterface.cpp compiles unchanged.
 *
 *      New capabilities exposed by the RA8876 in this mode:
 *        selectCanvas(layer) / freezeBackground() / clearCanvas()
 *        See "RA8876 LAYER COMPOSITING" note at the bottom of this file.
 *
 *  NOTE: verify RA8876_RP2040 library method names against your installed
 *  version — stubs are marked with "// NOTE: verify" where uncertain.
 */

#pragma once

// Arduino.h must come first — it provides uint8_t/uint16_t used by the
// board pin headers that BoardSettings.h will pull in.
#include <Arduino.h>

// BoardSettings.h chains: ProjectConfig.h → board pin header → DISPLAY_DRIVER_* define.
// Including it here ensures any file that includes DisplayDriver.h directly
// (e.g. DisplayDriver.cpp) gets the full define chain regardless of include order.
// BoardSettings.h has its own include guard (DEFINES_H) so this is a no-op
// when it has already been included by the including translation unit.
#include "BoardSettings.h"

// ---------------------------------------------------------------------------
//  TFT_eSPI modes — no wrapper required
// ---------------------------------------------------------------------------
#if defined(DISPLAY_DRIVER_SSD1963_PARALLEL) || defined(DISPLAY_DRIVER_RA8876_TFTESPI)

#include <SPI.h>
#include <TFT_eSPI_RA8876.h>

// TT_Display is simply TFT_eSPI_RA8876; DISPLAY_DECLARE_TFT creates the object
#define TT_Display          TFT_eSPI_RA8876
#define DISPLAY_DECLARE_TFT TFT_eSPI_RA8876 tft

// ---------------------------------------------------------------------------
//  RA8876 native library mode — compatibility wrapper
// ---------------------------------------------------------------------------
#elif defined(DISPLAY_DRIVER_RA8876_NATIVE)

#include <SPI.h>
#include <RA8876_RP2040.h>  // NOTE: verify include path for your library install

// ---------------------------------------------------------------------------
//  Color constants — match TFT_eSPI RGB565 values so UserInterface.cpp
//  colour literals compile without change.
// ---------------------------------------------------------------------------
#ifndef TFT_BLACK
  #define TFT_BLACK       0x0000
  #define TFT_NAVY        0x000F
  #define TFT_DARKGREEN   0x03E0
  #define TFT_DARKCYAN    0x03EF
  #define TFT_MAROON      0x7800
  #define TFT_PURPLE      0x780F
  #define TFT_OLIVE       0x7BE0
  #define TFT_LIGHTGREY   0xD69A
  #define TFT_DARKGREY    0x7BEF
  #define TFT_BLUE        0x001F
  #define TFT_GREEN       0x07E0
  #define TFT_CYAN        0x07FF
  #define TFT_RED         0xF800
  #define TFT_MAGENTA     0xF81F
  #define TFT_YELLOW      0xFFE0
  #define TFT_WHITE       0xFFFF
  #define TFT_ORANGE      0xFDA0
  #define TFT_GREENYELLOW 0xB7E0
  #define TFT_PINK        0xFE19
  #define TFT_BROWN       0x9A60
  #define TFT_GOLD        0xFEA0
  #define TFT_SILVER      0xC618
  #define TFT_SKYBLUE     0x867D
  #define TFT_VIOLET      0x915C
#endif

// ---------------------------------------------------------------------------
//  Text datum constants — match TFT_eSPI values
// ---------------------------------------------------------------------------
#ifndef TL_DATUM
  #define TL_DATUM   0   // Top Left
  #define TC_DATUM   1   // Top Centre
  #define TR_DATUM   2   // Top Right
  #define ML_DATUM   3   // Middle Left
  #define MC_DATUM   4   // Middle Centre
  #define MR_DATUM   5   // Middle Right
  #define BL_DATUM   6   // Bottom Left
  #define BC_DATUM   7   // Bottom Centre
  #define BR_DATUM   8   // Bottom Right
#endif

// ---------------------------------------------------------------------------
//  TT_Display — TFT_eSPI-compatible wrapper around RA8876_RP2040
//
//  Inheriting from RA8876_RP2040 gives us print()/println(), fillCircle(),
//  drawLine(), fillRect(), fillRoundRect() etc. for free.  We add the
//  TFT_eSPI-specific methods that RA8876_RP2040 does not provide.
// ---------------------------------------------------------------------------
class TT_Display : public RA8876_RP2040 {
public:
    // Constructor: all SPI pins passed explicitly.
    // DISPLAY_DECLARE_TFT (below) passes DISPLAY_CS/RST/SDI/SCK/SDO from
    // BoardPins_Stepper_v27.h — those macros are NOT referenced here so that
    // this class declaration compiles before BoardSettings.h is processed.
    TT_Display(uint8_t cs, uint8_t rst, uint8_t mosi, uint8_t sclk, uint8_t miso)
        : RA8876_RP2040(cs, rst, mosi, sclk, miso) {}

    // ---- Initialisation -----------------------------------------------
    // TFT_eSPI uses init(); RA8876_RP2040 uses begin().
    // Returns true on success, false if the chip did not respond.
    //
    // Why 20 MHz: TFT_eSPI_RA8876 used 20 MHz (DisplayConfig_RA8876_SPI.h
    // SPI_FREQUENCY = 20000000); matching it avoids marginal-timing glitches.
    //
    // Why re-apply timing registers: ra8876Initialize() calls RA8876_SW_Reset()
    // AFTER writing the display-timing registers (HDWR, VDHR, HNDR, HSTR, HPWR,
    // VNDR, VSTR, VPWR, DPCR, PCSR).  If SW_Reset clears configuration registers
    // back to their hardware-reset defaults (all-zeros), the panel never receives
    // valid sync and the display stays blank.  Re-applying them here is harmless
    // if they survived the reset and critical if they did not.
    //
    // Values match RA8876Registers.h for the TFTM101 10.1" 1024×600 panel
    // (SCAN_FREQ=50 MHz, XPCLK_INV=1, XHSYNC_INV=1, XVSYNC_INV=1, XDE_INV=0).
    bool init() {
        if (!RA8876_RP2040::begin(20000000))
            return false;

        // Re-apply sync-polarity register (may be zeroed by SW_Reset inside begin())
        // PCSR (0x13): XHSYNC_INV=1 (bit7), XVSYNC_INV=1 (bit6), XDE_INV=0 (bit5) = 0xC0
        lcdRegDataWrite(0x13, 0xC0);

        // Re-apply display-timing registers
        lcdHorizontalWidthVerticalHeight(1024, 600);  // HDWR / VDHR
        lcdHorizontalNonDisplay(160);                 // HNDR  (HND=160)
        lcdHsyncStartPosition(160);                   // HSTR  (HST=160)
        lcdHsyncPulseWidth(70);                       // HPWR  (HPW=70)
        lcdVerticalNonDisplay(23);                    // VNDR  (VND=23)
        lcdVsyncStartPosition(12);                    // VSTR  (VST=12)
        lcdVsyncPulseWidth(10);                       // VPWR  (VPW=10)

        // Ensure display output is enabled with correct pixel-clock polarity.
        // DPCR (0x12): XPCLK_INV=1 (bit7), DISPLAY_ON=1 (bit6), RGB=0 (bits1:0)
        displayOn(true);
        return true;
    }

    // ---- Rotation (track locally so getRotation() is always available) --
    void    setRotation(uint8_t r) { _rot = r; RA8876_RP2040::setRotation(r); }
    uint8_t getRotation()          { return _rot; }

    // ---- Text colour (track fg/bg for background-clearing in drawString) -
    void setTextColor(uint16_t fg, uint16_t bg = 0x0000) {
        _fg = fg; _bg = bg;
        RA8876_RP2040::setTextColor(fg, bg);  // NOTE: verify signature
    }

    // ---- Text datum & padding (TFT_eSPI additions) ----------------------
    void    setTextDatum(uint8_t datum)  { _datum = datum; }
    uint8_t getTextDatum()               { return _datum; }
    void    setTextPadding(int16_t pad)  { _padding = pad; }

    // ---- Cursor overloads -------------------------------------------------
    // The base RA8876_RP2040::setCursor(int16_t, int16_t, bool) is deliberately
    // hidden here to avoid an ambiguous-overload error when callers pass plain
    // int arguments (both bool and uint8_t require the same conversion rank).
    //
    // 2-arg: inline wrapper that calls base class directly.
    // 3-arg: selects the TFT_eSPI font then sets cursor (TFT_eSPI-compatible).
    void setCursor(int32_t x, int32_t y) {
        RA8876_RP2040::setCursor((int16_t)x, (int16_t)y);
    }
    void setCursor(int32_t x, int32_t y, uint8_t font);

    // ---- getCursorY: use base class if available, else tracked value -----
    // Remove this override if RA8876_RP2040 already provides getCursorY().
    int16_t getCursorY() { return RA8876_RP2040::getCursorY(); }  // NOTE: verify

    // ---- Text metrics — TFT_eSPI-compatible public interface -------------
    int16_t textWidth(const char* str, uint8_t font);
    void    drawString(const char* str, int32_t x, int32_t y, uint8_t font);
    // fontHeight(font): returns pixel height of the named font.
    // Mirrors the TFT_eSPI method of the same name.
    int16_t fontHeight(uint8_t font) { return _fontHeight(font); }

    // ---- fillRect guard — rejects degenerate dimensions before the GE ------
    // The RA8876 DRAW_SQUARE_FILL GE (DCR1 = 0xE0) hangs when:
    //   • w == 1  → x_end == x_start (equal coordinates hang the engine)
    //   • w == 0  → x_end = x - 1 < x_start (inverted → hangs)
    //   • h == 0  → y_end = y - 1 < y_start (inverted → hangs)
    // These degenerate cases arise in _fillRoundedRect when a button's height
    // exactly equals 2×r (centre-strip height = 0) or width exactly equals 2×r
    // (top/bottom-strip width = 0).  The base-class fillRect has no such guard.
    //
    // w == 1 is handled via drawPixel (no GE) so single-pixel columns render
    // correctly.  w ≤ 0 or h ≤ 0 are silent no-ops (nothing to draw).
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    // ---- Triangle override — software scanline fill -----------------------
    // DRAW_TRIANGLE_FILL (DCR0 0xA2) hangs on this chip for specific coordinate
    // combinations that arise during drawWideLine calls (bridge/track rendering).
    // This software implementation uses horizontal fillRect scanlines (same as
    // fillCircle), which use only DRAW_SQUARE_FILL (0xE0) — reliably safe.
    void fillTriangle(int16_t x0, int16_t y0,
                      int16_t x1, int16_t y1,
                      int16_t x2, int16_t y2, uint16_t color);

    // ---- Circle overrides — software implementations ----------------------
    // The RA8876 DCR1 circle/ellipse engine (commands 0x80 and 0xC0) hangs on
    // this chip: after writing to DCR1 the GE busy bit never clears.  Only
    // DRAW_SQUARE_FILL (0xE0) and the DCR0 triangle engine work correctly.
    // 1×1 fillRect also hangs (DRAW_SQUARE_FILL requires x_end > x_start).
    //
    // fillCircle  — horizontal scanline fill using fillRect (skips dx==0 rows).
    // drawCircle  — Bresenham midpoint outline using drawPixel (no GE at all).
    //
    // These hide the base-class methods so every call through TT_Display
    // (drawSmoothCircle, drawWideLine end-caps, drawSmoothRoundRect corners,
    // and direct tft.fillCircle / tft.drawCircle calls in UserInterface.cpp)
    // automatically uses the software path.
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

    // ---- Anti-aliased / smooth drawing — approximated on RA8876 ---------
    // drawSmoothRoundRect: draws a filled border ring.
    // Built from fillRect + fillCircle (both software-safe on this chip).
    void drawSmoothRoundRect(int32_t x, int32_t y,
                             int32_t r, int32_t ir,
                             int32_t w, int32_t h,
                             uint32_t fg_color,
                             uint32_t bg_color = 0x00FFFFFF,
                             uint8_t  quadrants = 0xF);

    // drawSmoothCircle: maps to hardware drawCircle (no AA on RA8876).
    void drawSmoothCircle(int32_t x, int32_t y, int32_t r,
                          uint32_t fg_color, uint32_t bg_color);

    // drawWideLine: hardware-accelerated parallelogram + end caps.
    // Equivalent to TFT_eSPI drawWideLine but without sub-pixel AA.
    // bg_color is accepted for API compatibility with TFT_eSPI callers that
    // pass TFT_BLACK to suppress pixel-read AA; it is ignored on RA8876.
    void drawWideLine(float ax, float ay, float bx, float by,
                      float wd, uint32_t color,
                      uint32_t bg_color = TFT_BLACK);

    // ---- RA8876-specific: canvas layer helpers --------------------------
    // Currently single-layer stubs (see DisplayDriver.cpp for why).
    // selectCanvas / freezeBackground are no-ops; clearCanvas erases only
    // the inner turntable circle — same as the TFT_eSPI legacy erase path.
    void selectCanvas(uint8_t layer);                      // no-op (single-layer)
    void freezeBackground();                               // no-op (single-layer)
    void clearCanvas(uint16_t color = TFT_BLACK);          // fillCircle erase

private:
    uint8_t  _rot     = 0;
    uint8_t  _datum   = TL_DATUM;
    int16_t  _padding = 0;
    uint16_t _fg      = TFT_WHITE;
    uint16_t _bg      = TFT_BLACK;

    // Font metric helpers
    void    _selectFont(uint8_t font);
    int16_t _fontHeight(uint8_t font);
    int16_t _fontCharWidth(uint8_t font);
};

// DISPLAY_CS, DISPLAY_RST, DISPLAY_SDI, DISPLAY_SCK, DISPLAY_SDO are
// defined in board_configs/BoardPins_Stepper_v27.h via BoardSettings.h.
// This macro is expanded only in UserInterface.cpp, which has those defines.
//
// Pin-swap note: BoardPins_Stepper_v27.h labels DISPLAY_SDI=GPIO8 as "MOSI"
// and DISPLAY_SDO=GPIO11 as "MISO" from the *display's* perspective.
// On RP2040, GPIO8 is SPI1 RX and GPIO11 is SPI1 TX (hardware-fixed).
// Philhower panics if setTX() is called with a non-TX GPIO, so we pass
// DISPLAY_SDO (GPIO11, SPI1 TX) as the mosi argument and
// DISPLAY_SDI (GPIO8,  SPI1 RX) as the miso argument.
// The physical wiring (GPIO11→display SDI, GPIO8→display SDO) is correct.
#define DISPLAY_DECLARE_TFT TT_Display tft(DISPLAY_CS, DISPLAY_RST, DISPLAY_SDO, DISPLAY_SCK, DISPLAY_SDI)

// ---------------------------------------------------------------------------
#else
  #error "No display driver defined. Set DISPLAY_DRIVER_SSD1963_PARALLEL, \
DISPLAY_DRIVER_RA8876_TFTESPI, or DISPLAY_DRIVER_RA8876_NATIVE before including \
BoardSettings.h."
#endif  // display mode selection

/*
 * ============================================================================
 *  RA8876 LAYER COMPOSITING — SUGGESTED ENHANCEMENT
 * ============================================================================
 *
 *  The RA8876 provides two independent canvas layers that the chip can
 *  hardware-composite to the display.  This is the single most impactful
 *  enhancement available over the SSD1963 parallel path:
 *
 *  Layer 0 — Static background
 *      Drawn once at startup: outer turntable ring, all track lines, track
 *      labels.  Never erased again.
 *
 *  Layer 1 — Dynamic animation
 *      The bridge and shack only.  Cleared and redrawn on every position
 *      update.
 *
 *  Today, drawBridge() calls:
 *      tft.fillCircle(TT_CX, TT_CY, TT_RAD-2, TFT_BLACK);   // erase
 *  which forces drawTracks() to redraw every track line.  With layers:
 *      tft.selectCanvas(1);
 *      tft.clearCanvas();            // wipe only Layer 1 — Layer 0 untouched
 *      drawBridge(angle);
 *  The chip automatically composites both layers to the display panel.
 *
 *  Suggested code change in Turntable.cpp / UserInterface.cpp:
 *
 *    void drawTurnTable() {
 *  #if defined(DISPLAY_DRIVER_RA8876_NATIVE)
 *      tft.selectCanvas(0);          // draw static content to Layer 0
 *  #endif
 *      tft.fillCircle(...);          // outer ring
 *      drawTracks();                 // all track lines
 *  #if defined(DISPLAY_DRIVER_RA8876_NATIVE)
 *      tft.freezeBackground();       // lock Layer 0
 *      tft.selectCanvas(1);          // switch to animation layer
 *  #endif
 *    }
 *
 *    void drawBridge(float angle) {
 *  #if defined(DISPLAY_DRIVER_RA8876_NATIVE)
 *      tft.clearCanvas();            // wipe bridge layer only
 *  #else
 *      tft.fillCircle(TT_CX, TT_CY, TT_RAD-2, TFT_BLACK);  // legacy erase
 *  #endif
 *      // ... rest of drawBridge unchanged
 *    }
 *
 *  Additional BTE benefit: the hardware BTE fill used in clearCanvas() is
 *  significantly faster than a CPU-driven fillCircle() at full resolution.
 * ============================================================================
 */
