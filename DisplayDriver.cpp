/*
 *  DisplayDriver.cpp — TT_Display wrapper implementations (RA8876_NATIVE only)
 *
 *  Compiled only when DISPLAY_DRIVER_RA8876_NATIVE is defined.
 *  For the two TFT_eSPI modes no code is generated here.
 */

#include "DisplayDriver.h"
#include "BoardSettings.h"

#ifdef DISPLAY_DRIVER_RA8876_NATIVE

#include <math.h>   // sqrtf

// ===========================================================================
//  Font metric tables
//  Maps TFT_eSPI font numbers to actual RA8876 CGROM pixel dimensions.
//
//  RA8876 internal CGROM fonts (fixed-width):
//    8×16  → selected via RA8876_CHAR_HEIGHT_16 (size_select = 0)
//   12×24  → selected via RA8876_CHAR_HEIGHT_24 (size_select = 1)
//   16×32  → selected via RA8876_CHAR_HEIGHT_32 (size_select = 2)
//  16×32 with 2× scale → 32×64 (for large TFT_eSPI font equivalents)
//
//  TFT_eSPI → RA8876 mapping used in _selectFont():
//    font 1 (GLCD ~8px)    → 8×16  (closest available)
//    font 2 (16px)         → 8×16  (exact height match)
//    font 4 (26px)         → 12×24 (closest available)
//    font 6/7/8 (48-75px)  → 16×32 × 2 = 32×64
// ===========================================================================

// Character cell width in pixels
int16_t TT_Display::_fontCharWidth(uint8_t font) {
    switch (font) {
        case 1:
        case 2:  return  8;   // 8×16 CGROM font
        case 4:  return 12;   // 12×24 CGROM font
        case 6:
        case 7:
        case 8:  return 32;   // 16×32 CGROM, 2× scale → 32 px wide
        default: return  8;
    }
}

// Character cell height in pixels
int16_t TT_Display::_fontHeight(uint8_t font) {
    switch (font) {
        case 1:
        case 2:  return 16;   // 8×16
        case 4:  return 24;   // 12×24
        case 6:
        case 7:
        case 8:  return 64;   // 16×32 × 2
        default: return 16;
    }
}

// Select the closest RA8876 CGROM font for a given TFT_eSPI font number.
//
// setTextParameter1(source, size, iso):
//   source = RA8876_SELECT_INTERNAL_CGROM (0) — use on-chip CGROM
//   size   = RA8876_CHAR_HEIGHT_16/24/32   (0/1/2)
//   iso    = RA8876_SELECT_8859_1          (0) — Latin-1
//
// setTextParameter2(align, chroma_key, width_enlarge, height_enlarge):
//   align/chroma_key = 0 (disabled)
//   width/height     = RA8876_TEXT_WIDTH_ENLARGEMENT_X1 (0) = 1×
//                      RA8876_TEXT_WIDTH_ENLARGEMENT_X2 (1) = 2×
void TT_Display::_selectFont(uint8_t font) {
    switch (font) {
        case 1:
        case 2:
            setTextParameter1(RA8876_SELECT_INTERNAL_CGROM,
                              RA8876_CHAR_HEIGHT_16,
                              RA8876_SELECT_8859_1);
            setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE,
                              RA8876_TEXT_CHROMA_KEY_DISABLE,
                              RA8876_TEXT_WIDTH_ENLARGEMENT_X1,
                              RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
            break;
        case 4:
            setTextParameter1(RA8876_SELECT_INTERNAL_CGROM,
                              RA8876_CHAR_HEIGHT_24,
                              RA8876_SELECT_8859_1);
            setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE,
                              RA8876_TEXT_CHROMA_KEY_DISABLE,
                              RA8876_TEXT_WIDTH_ENLARGEMENT_X1,
                              RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
            break;
        case 6:
        case 7:
        case 8:
            setTextParameter1(RA8876_SELECT_INTERNAL_CGROM,
                              RA8876_CHAR_HEIGHT_32,
                              RA8876_SELECT_8859_1);
            setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE,
                              RA8876_TEXT_CHROMA_KEY_DISABLE,
                              RA8876_TEXT_WIDTH_ENLARGEMENT_X2,
                              RA8876_TEXT_HEIGHT_ENLARGEMENT_X2);
            break;
        default:
            setTextParameter1(RA8876_SELECT_INTERNAL_CGROM,
                              RA8876_CHAR_HEIGHT_16,
                              RA8876_SELECT_8859_1);
            setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE,
                              RA8876_TEXT_CHROMA_KEY_DISABLE,
                              RA8876_TEXT_WIDTH_ENLARGEMENT_X1,
                              RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);
            break;
    }
}

// ===========================================================================
//  setCursor — 3-argument TFT_eSPI overload (x, y, fontNumber)
// ===========================================================================
void TT_Display::setCursor(int32_t x, int32_t y, uint8_t font) {
    _selectFont(font);
    RA8876_RP2040::setCursor((int16_t)x, (int16_t)y);
}

// ===========================================================================
//  textWidth — estimated pixel width of string in given font
// ===========================================================================
int16_t TT_Display::textWidth(const char* str, uint8_t font) {
    if (!str) return 0;
    return (int16_t)(strlen(str) * _fontCharWidth(font));
}

// ===========================================================================
//  drawString — draw text at (x,y) respecting current text datum.
//  The area covered by _padding pixels is cleared before drawing so that
//  refreshing a label removes old content.
// ===========================================================================
void TT_Display::drawString(const char* str, int32_t x, int32_t y, uint8_t font) {
    if (!str) return;

    int16_t tw = textWidth(str, font);
    int16_t th = _fontHeight(font);
    int16_t ax = (int16_t)x;
    int16_t ay = (int16_t)y;

    // Adjust origin for datum
    switch (_datum) {
        case TC_DATUM: ax -= tw / 2;                break;
        case TR_DATUM: ax -= tw;                    break;
        case ML_DATUM:              ay -= th / 2;   break;
        case MC_DATUM: ax -= tw / 2; ay -= th / 2;  break;
        case MR_DATUM: ax -= tw;     ay -= th / 2;  break;
        case BL_DATUM:              ay -= th;        break;
        case BC_DATUM: ax -= tw / 2; ay -= th;       break;
        case BR_DATUM: ax -= tw;     ay -= th;       break;
        default: break;  // TL_DATUM: no adjustment
    }

    // Clear padding strip (used for numeric / updating labels)
    if (_padding > 0) {
        int16_t clearW = (_padding > tw) ? _padding : tw;
        fillRect(ax, ay, clearW, th, _bg);
    }

    _selectFont(font);
    setTextColor(_fg, _bg);
    RA8876_RP2040::setCursor(ax, ay);
    print(str);
}

// ===========================================================================
//  fillRect — GE-safe wrapper
//
//  The RA8876 DRAW_SQUARE_FILL (DCR1 = 0xE0) GE hangs indefinitely when
//  x_end == x_start (w = 1) or when x_end/y_end < x_start/y_start (w or h = 0,
//  giving an inverted range).  These degenerate cases arise naturally in
//  _fillRoundedRect: the centre strip has height (h - 2r), which is exactly 0
//  when the button height equals 2r; the top/bottom strips have width (w - 2r),
//  which is exactly 0 when the button width equals 2r.  The base-class fillRect
//  has no guard for these cases before submitting to the GE.
//
//  This override intercepts degenerate calls before they reach the GE:
//    w ≤ 0 or h ≤ 0 → silent no-op (nothing to draw)
//    w == 1          → single-pixel column via drawPixel (no GE)
//    all other       → delegate to base-class GE rect fill
// ===========================================================================
void TT_Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (w <= 0 || h <= 0) {
        return;   // degenerate dimensions would invert GE coordinates → hang
    }

    if (w == 1) {
        // w=1 → x_end == x_start → GE hang.  Fall back to direct SDRAM pixel writes.
        check2dBusy();   // flush any prior GE op before switching to direct write mode
        for (int16_t i = 0; i < h; i++)
            drawPixel((uint16_t)x, (uint16_t)(y + i), color);
        return;
    }

    RA8876_RP2040::fillRect(x, y, w, h, color);
}

// ===========================================================================
//  fillTriangle — software scanline fill
//
//  RA8876 DRAW_TRIANGLE_FILL (DCR0 = 0xA2) hangs indefinitely on this chip
//  for specific coordinate combinations that arise during drawWideLine calls
//  (bridge and track rendering).  We substitute a CPU scanline fill that uses
//  only horizontal fillRect (DRAW_SQUARE_FILL, DCR1 = 0xE0) which is reliably
//  stable on this chip.
//
//  Each scanline calls fillRect with h=1.  We skip any scanline where the
//  computed width is < 2 (w=1 → x_end == x_start → GE hang; w≤0 → empty).
//  For the wide lines drawn in this application these degenerate scanlines
//  appear only at isolated tip pixels and are visually imperceptible.
//
//  The algorithm is a standard integer-arithmetic scanline split:
//    • Sort vertices so y0 ≤ y1 ≤ y2.
//    • Upper half (y0..y1): interpolate along edges 0→2 and 0→1.
//    • Lower half (y1+1..y2): interpolate along edges 0→2 and 1→2.
// ===========================================================================
void TT_Display::fillTriangle(int16_t x0, int16_t y0,
                               int16_t x1, int16_t y1,
                               int16_t x2, int16_t y2, uint16_t color) {
    // Sort vertices ascending by y (bubble-sort three elements)
    if (y0 > y1) { int16_t t; t=x0; x0=x1; x1=t; t=y0; y0=y1; y1=t; }
    if (y1 > y2) { int16_t t; t=x1; x1=x2; x2=t; t=y1; y1=y2; y2=t; }
    if (y0 > y1) { int16_t t; t=x0; x0=x1; x1=t; t=y0; y0=y1; y1=t; }

    // Degenerate: all three vertices on the same row
    if (y0 == y2) {
        int16_t xa = min(min(x0, x1), x2);
        int16_t xb = max(max(x0, x1), x2);
        int16_t w  = xb - xa + 1;
        if (w >= 2) fillRect(xa, y0, w, 1, color);
        return;
    }

    int16_t dy02 = y2 - y0;   // always > 0 (guaranteed by sort + degenerate check)
    int16_t dy01 = y1 - y0;   // >= 0
    int16_t dy12 = y2 - y1;   // >= 0

    // Upper half: y0 .. y1 — interpolate edges 0→2 and 0→1
    for (int16_t y = y0; y <= y1; y++) {
        // Long edge 0→2 always spans the full height
        int16_t xa = x0 + (int16_t)((int32_t)(x2 - x0) * (y - y0) / dy02);
        // Short edge 0→1 (guard: when dy01==0 the loop runs only at y==y0==y1,
        // and both endpoints are valid choices — use x1 for the full span)
        int16_t xb = (dy01 > 0) ? x0 + (int16_t)((int32_t)(x1 - x0) * (y - y0) / dy01)
                                 : x1;
        if (xa > xb) { int16_t t = xa; xa = xb; xb = t; }
        int16_t w = xb - xa + 1;
        if (w >= 2) fillRect(xa, y, w, 1, color);   // skip w=1 (GE hang) and w≤0
    }

    // Lower half: y1+1 .. y2 — interpolate edges 0→2 and 1→2
    for (int16_t y = y1 + 1; y <= y2; y++) {
        int16_t xa = x0 + (int16_t)((int32_t)(x2 - x0) * (y - y0) / dy02);
        // Short edge 1→2 (dy12==0 means y1==y2, so this loop never executes)
        int16_t xb = (dy12 > 0) ? x1 + (int16_t)((int32_t)(x2 - x1) * (y - y1) / dy12)
                                 : x2;
        if (xa > xb) { int16_t t = xa; xa = xb; xb = t; }
        int16_t w = xb - xa + 1;
        if (w >= 2) fillRect(xa, y, w, 1, color);   // skip w=1 (GE hang) and w≤0
    }
}

// ===========================================================================
//  fillCircle — software scanline fill
//
//  RA8876 DRAW_CIRCLE_FILL (DCR1 = 0xC0) uses ELL_A/B registers and hangs
//  indefinitely on this chip.  We substitute 2r+1 horizontal fillRect strips.
//  fillRect (DCR1 = 0xE0, pure rectangle) works correctly.
//
//  All code paths that call fillCircle — drawWideLine end-caps, turntable
//  erase calls, shack indicator dots, _fillRoundedRect corners — hit this
//  override automatically because the call site type is TT_Display.
// ===========================================================================
void TT_Display::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    if (r <= 0) {
        // Single-pixel case: use drawPixel (no GE) to avoid degenerate 1×1 rect
        check2dBusy();  // flush any pending GE before switching to direct write
        drawPixel((uint16_t)x0, (uint16_t)y0, color);
        return;
    }
    int32_t r2 = (int32_t)r * r;
    for (int16_t dy = -r; dy <= r; dy++) {
        int16_t dx = (int16_t)sqrtf((float)(r2 - (int32_t)dy * dy));
        // Skip degenerate scanlines (dx==0 → w=1 → x_end==x_start).
        // The RA8876 DRAW_SQUARE_FILL GE hangs when x_end==x_start.
        // These occur only at the very top/bottom pixel of the circle and
        // are visually imperceptible for r≥2.
        if (dx == 0) continue;
        fillRect(x0 - dx, y0 + dy, (int16_t)(2 * dx + 1), 1, color);
    }
}

// ===========================================================================
//  drawCircle — software Bresenham midpoint outline
//
//  RA8876 DRAW_CIRCLE (DCR1 = 0x80) uses the ELL_A/B registers and hangs
//  indefinitely on this chip.  The previous approach of using 1×1 fillRect
//  calls for each pixel also hangs: DRAW_SQUARE_FILL (0xE0) requires
//  x_end > x_start, but a 1×1 rect has x_end == x_start.
//
//  Fix: use drawPixel() for each boundary pixel.  drawPixel() writes directly
//  to SDRAM via setPixelCursor + ramAccessPrepare + lcdDataWrite — no GE
//  involved, always safe.
//
//  check2dBusy() is called once at the top so that any GE op that preceded
//  this call (e.g. fillCircle, fillRect) has fully completed before drawPixel
//  calls graphicMode(true) and begins direct SDRAM writes.
// ===========================================================================
void TT_Display::drawCircle(uint16_t ux0, uint16_t uy0, uint16_t ur, uint16_t color) {
    // Flush any pending GE before switching to direct SDRAM pixel writes
    check2dBusy();

    int16_t x0 = (int16_t)ux0, y0 = (int16_t)uy0, r = (int16_t)ur;
    if (r <= 0) {
        drawPixel((uint16_t)x0, (uint16_t)y0, color);
        return;
    }
    int16_t f     =  1 - r;
    int16_t ddF_x =  0;
    int16_t ddF_y = -2 * r;
    int16_t x     =  0;
    int16_t y     =  r;

    drawPixel((uint16_t)x0,       (uint16_t)(y0 + r), color);
    drawPixel((uint16_t)x0,       (uint16_t)(y0 - r), color);
    drawPixel((uint16_t)(x0 + r), (uint16_t)y0,       color);
    drawPixel((uint16_t)(x0 - r), (uint16_t)y0,       color);

    while (x < y) {
        if (f >= 0) { y--; ddF_y += 2; f += ddF_y; }
        x++; ddF_x += 2; f += ddF_x + 1;
        drawPixel((uint16_t)(x0 + x), (uint16_t)(y0 + y), color);
        drawPixel((uint16_t)(x0 - x), (uint16_t)(y0 + y), color);
        drawPixel((uint16_t)(x0 + x), (uint16_t)(y0 - y), color);
        drawPixel((uint16_t)(x0 - x), (uint16_t)(y0 - y), color);
        drawPixel((uint16_t)(x0 + y), (uint16_t)(y0 + x), color);
        drawPixel((uint16_t)(x0 - y), (uint16_t)(y0 + x), color);
        drawPixel((uint16_t)(x0 + y), (uint16_t)(y0 - x), color);
        drawPixel((uint16_t)(x0 - y), (uint16_t)(y0 - x), color);
    }
}

// ===========================================================================
//  drawSmoothRoundRect — software rounded rectangle border (fillRect + fillCircle)
//
//  TFT_eSPI signature:
//    drawSmoothRoundRect(x, y, r, ir, w, h, fg, bg, quadrants)
//  where r = outer radius, ir = inner radius, border thickness = r-ir+1 px.
//  Anti-aliasing is not replicated; shape and dimensions are correct.
//  The `quadrants` mask is ignored — all four corners are always drawn.
//
//  NOTE: The RA8876 DRAW_CIRCLE_SQUARE_FILL (0xF0) GE command was tried and
//  discarded — on this chip the GE bit (status bit 3) never clears after DCR1
//  is written, causing check2dBusy() to time out on every call.  fillRect
//  (0xE0) and fillCircle both work correctly, so we use them instead.
// ===========================================================================
// Software rounded-rect fill using fillRect + fillCircle.
//
// The RA8876 DRAW_CIRCLE_SQUARE_FILL (0xF0) GE command hangs indefinitely on
// this chip — bit 3 of the status register never clears after DCR1 is written.
// fillRect (DRAW_SQUARE_FILL, 0xE0) and fillCircle both work correctly.
// We implement the rounded rect by compositing three strips and four corner
// discs, exactly as TFT_eSPI does in its software-fallback path.
//
// Helper: fill one rounded rect region using only working primitives.
static void _fillRoundedRect(TT_Display& d,
                              int32_t x, int32_t y, int32_t w, int32_t h,
                              int32_t r, uint16_t color) {
    if (w <= 0 || h <= 0) return;
    // Clamp radius so it can't exceed half of either dimension
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;
    if (r < 1)     r = 0;

    if (r == 0) {
        d.fillRect((int16_t)x, (int16_t)y, (int16_t)w, (int16_t)h, color);
        return;
    }

    // Horizontal centre strip (full width, inner height).
    // h - 2r can be 0 when h == 2r exactly; fillRect override handles w/h <= 0.
    d.fillRect((int16_t)x,           (int16_t)(y + r),
               (int16_t)w,           (int16_t)(h - 2*r), color);
    // Top strip (inner width, radius height).
    // w - 2r is always >= 2 for the button dimensions used in this project
    // (minimum w=90, r=10 → inner w=70).  Guard at call site if w can be small.
    d.fillRect((int16_t)(x + r),     (int16_t)y,
               (int16_t)(w - 2*r),   (int16_t)r,         color);
    // Bottom strip (same dimensions as top strip).
    d.fillRect((int16_t)(x + r),     (int16_t)(y + h - r),
               (int16_t)(w - 2*r),   (int16_t)r,         color);

    // Four corner discs
    int16_t cx_l = (int16_t)(x + r);
    int16_t cx_r = (int16_t)(x + w - r - 1);
    int16_t cy_t = (int16_t)(y + r);
    int16_t cy_b = (int16_t)(y + h - r - 1);
    int16_t rv   = (int16_t)r;
    d.fillCircle(cx_l, cy_t, rv, color);   // top-left
    d.fillCircle(cx_r, cy_t, rv, color);   // top-right
    d.fillCircle(cx_l, cy_b, rv, color);   // bottom-left
    d.fillCircle(cx_r, cy_b, rv, color);   // bottom-right
}

void TT_Display::drawSmoothRoundRect(int32_t x, int32_t y,
                                     int32_t r, int32_t ir,
                                     int32_t w, int32_t h,
                                     uint32_t fg_color,
                                     uint32_t bg_color,
                                     uint8_t  /*quadrants*/) {
    // Fill outer rounded rect with border colour
    _fillRoundedRect(*this, x, y, w, h, r, (uint16_t)fg_color);

    // Punch out interior with background colour
    int32_t thickness = r - ir + 1;
    int32_t iw = w - 2 * thickness;
    int32_t ih = h - 2 * thickness;
    if (iw > 0 && ih > 0) {
        _fillRoundedRect(*this, x + thickness, y + thickness,
                         iw, ih, ir, (uint16_t)bg_color);
    }
}

// ===========================================================================
//  drawSmoothCircle — software Bresenham outline (no sub-pixel AA on RA8876)
// ===========================================================================
void TT_Display::drawSmoothCircle(int32_t x, int32_t y, int32_t r,
                                   uint32_t fg_color, uint32_t /*bg_color*/) {
    drawCircle((int16_t)x, (int16_t)y, (int16_t)r, (uint16_t)fg_color);
}

// ===========================================================================
//  drawWideLine — parallelogram body + circular end caps
//
//  Equivalent to TFT_eSPI drawWideLine without sub-pixel AA.
//  Uses software fillTriangle (scanline fill) for the body and software
//  fillCircle (scanline fill) for end caps — both safe on this chip.
// ===========================================================================
void TT_Display::drawWideLine(float ax, float ay, float bx, float by,
                               float wd, uint32_t color,
                               uint32_t /*bg_color*/) {
    float dx = bx - ax;
    float dy = by - ay;
    float len = sqrtf(dx * dx + dy * dy);

    if (len < 0.5f) {
        // Degenerate: just draw a filled circle
        fillCircle((int16_t)(ax + 0.5f), (int16_t)(ay + 0.5f),
                   (int16_t)(wd * 0.5f + 0.5f), (uint16_t)color);
        return;
    }

    // Perpendicular half-width vector
    float hw = wd * 0.5f;
    float px = -dy / len * hw;
    float py =  dx / len * hw;

    // Four corners of the parallelogram
    int16_t x0 = (int16_t)(ax + px + 0.5f), y0 = (int16_t)(ay + py + 0.5f);
    int16_t x1 = (int16_t)(ax - px + 0.5f), y1 = (int16_t)(ay - py + 0.5f);
    int16_t x2 = (int16_t)(bx - px + 0.5f), y2 = (int16_t)(by - py + 0.5f);
    int16_t x3 = (int16_t)(bx + px + 0.5f), y3 = (int16_t)(by + py + 0.5f);

    // Fill body as two triangles
    fillTriangle(x0, y0, x1, y1, x2, y2, (uint16_t)color);
    fillTriangle(x0, y0, x2, y2, x3, y3, (uint16_t)color);

    // Rounded end caps
    int16_t capR = (int16_t)(hw + 0.5f);
    fillCircle((int16_t)(ax + 0.5f), (int16_t)(ay + 0.5f), capR, (uint16_t)color);
    fillCircle((int16_t)(bx + 0.5f), (int16_t)(by + 0.5f), capR, (uint16_t)color);
}

// ===========================================================================
//  RA8876 Layer / canvas helpers
//
//  NOTE ON LAYER COMPOSITING:
//  selectScreen() in RA8876_common calls displayImageStartAddress(currentPage),
//  which switches the chip's display output to the new page.  There is no
//  separate "draw canvas" register — page-select is all-or-nothing.  Calling
//  selectScreen(PAGE2) while static content lives on PAGE1 produces a blank
//  display because PAGE2 starts uninitialised.
//
//  True hardware compositing requires the RA8876 BTE (Block Transfer Engine)
//  registers, which the library does not currently expose at a high level.
//  For now we operate in single-layer mode: all drawing goes to the default
//  page (PAGE1), and bridge animation erases only the inner circle — exactly
//  the same as the legacy TFT_eSPI path.
//
//  When the BTE compositor is wired up in a future revision, selectCanvas()
//  and freezeBackground() can be given real implementations without changing
//  any call sites in UserInterface.cpp.
// ===========================================================================

// selectCanvas — no-op in single-layer mode.
// The layer parameter is accepted for API compatibility with UserInterface.cpp.
void TT_Display::selectCanvas(uint8_t /*layer*/) {
    // Intentional no-op: see NOTE ON LAYER COMPOSITING above.
}

// freezeBackground — no-op in single-layer mode.
void TT_Display::freezeBackground() {
    // Intentional no-op: see NOTE ON LAYER COMPOSITING above.
}

// clearCanvas — erase the bridge/shack area (inside the turntable ring).
// Equivalent to the TFT_eSPI legacy path:
//   tft.fillCircle(TT_CX, TT_CY, TT_RAD-2, TFT_BLACK)
// TT_CX, TT_CY, TT_RAD are defined in BoardSettings.h (included above).
void TT_Display::clearCanvas(uint16_t color) {
    fillCircle(TT_CX, TT_CY, (int16_t)(TT_RAD - 2), color);
}

#endif  // DISPLAY_DRIVER_RA8876_NATIVE
