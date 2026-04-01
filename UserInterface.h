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

#include <SPI.h>    // Call up the TFT driver library
#include <TFT_eSPI.h>      // Hardware-specific library
// #include <bb_captouch.h>
// #include "TAMC_GT911.h"
#include "src/application_drivers/my_bb_captouch.h"
//#include <SPI.h> // Not needed when using I2C


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

#endif
