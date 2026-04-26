/*
 * Parts of this © 2022 Peter Cole, 2023-5 Bob Gamble
 *
 *  This file is a part of the LCCt Turntable project
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  For a copy of the GNU General Public License see <https://www.gnu.org/licenses/>.
*/

#ifndef IOFUNCTIONS_H
#define IOFUNCTIONS_H

#include <Arduino.h>
#include <Wire.h>
#include "Turntable.h"
#include "TTvariables.h"
// #include "EEPROMFunctions.h"
#include "version.h"

#include "DisplayDriver.h"   // selects TFT_eSPI or RA8876_RP2040 wrapper
#include "src/application_drivers/my_bb_captouch.h"


void setupDisplay();
void displayConfig();
void notice(const char *string);
void drawButton(int box, int32_t x, int32_t y, uint32_t fg_color,const char *string);
void drawHomePage();
void drawButtonPage();
void drawButtons();
void drawDoorButton(int track);
void drawFastClock(int hour, int minute);
// void drawSetting(int i);
void drawConfigPage();
// void drawSteps();
// void drawServos();
// void drawServo(int i);
void listServos(int col);
void listReverences(int col);
void drawDiagnosticPage();
void drawDiagnostics();
void drawTrackMatrix(int col);
void drawTurnTable();
void drawTracks();
void drawBridge(float angle);
void drawShack(float angle);
void drawTrack(int track, float angle);
int HotSpotBox(int X, int Y);
void setHotSpot(int box, int X1, int Y1, int X2, int Y2);
void setHotSpot8(int box, int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4);
void drawCross(int x, int y, unsigned int color);
void touchIO();
void rotatePoints(int x, int y);
void TurnOnPixels();
void TurnOffPixels();
void TogglePixels();
void DimmerHigh();
void DimmerLow();
void ScreenPrint(String text, int col, int row, int size);
void updateBridgeAnimation();

// ===========================================================================
//  Display dirty flags
//
//  Rather than redrawing immediately inside event callbacks, callers set one
//  or more of these flags.  updateDisplay() drains them once per loop()
//  iteration, coalescing rapid bursts (e.g. login-time door-state sync) into
//  a single draw pass and keeping event callbacks fast.
//
//  Categories
//    DISP_DIRTY_TRACKS  one or more radial track lines (home page, screen 1)
//    DISP_DIRTY_BRIDGE  bridge interior — power-state rails or centre loco
//    DISP_DIRTY_DOORS   one or more door buttons  (button page, screen 2)
//
//  Per-element masks
//    _track_dirty_mask  bit n set → drawTrack(n) pending
//    _door_dirty_mask   bit n set → drawDoorButton(n) pending
// ===========================================================================
#define DISP_DIRTY_NONE    0x00
#define DISP_DIRTY_TRACKS  0x01
#define DISP_DIRTY_BRIDGE  0x02
#define DISP_DIRTY_DOORS   0x04

extern bool              _displayOK;            // true only if tft.init() succeeded
extern volatile bool     _display_page_drawing; // true while Core 1 is doing a full page redraw
extern volatile uint8_t  _display_dirty_flags;
extern volatile uint32_t _track_dirty_mask;
extern volatile uint32_t _door_dirty_mask;

// Setters — call instead of drawing directly inside callbacks
void markTrackDirty(int track);   // DISP_DIRTY_TRACKS + bit in _track_dirty_mask
void markDoorDirty(int track);    // DISP_DIRTY_DOORS  + bit in _door_dirty_mask
void markBridgeDirty();           // DISP_DIRTY_BRIDGE

// Drain all dirty flags — call once per loop() iteration after updateBridgeAnimation()
void updateDisplay();

#endif
