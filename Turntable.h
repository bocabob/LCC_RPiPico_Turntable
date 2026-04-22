/*
 * Parts of this © 2022 Peter Cole, 2023-5 Bob Gamble
 *
 *  This file is a part of the LCC Turntable project
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

/*=============================================================
 * This file contains all functions pertinent to turntable
 * operation including stepper movements, relay phase switching,
 * and LED/accessory related functions.
=============================================================*/

#ifndef TURNTABLEFUNCTIONS_H
#define TURNTABLEFUNCTIONS_H

#include <Arduino.h>
#include "BoardSettings.h"
#include "TTvariables.h"
#include <I2C_eeprom.h>
#include "DisplayDriver.h"  // selects display library; also pulls in <SPI.h>

bool isStepperRunning();
long getCurrentPosition();
void runStepper();
void disableStepper();
void initializeHardware();
void LightSwitch(int Light, int dir);
void ToggleLight(int Light);
void setupStepperDriver();
void moveHome();
void moveToPosition(long steps, uint8_t phaseSwitch);
void setPhase(uint8_t phase);
void processLED();
void calibration();
void processAutoPhaseSwitch();
bool getHomeState();
bool getBridgeState();
void initiateHoming();
void initiateStepCount();
void initiateReferences();
void setLEDActivity(uint8_t activity);
void setAccessory(bool state);
// LN code
void MoveToTrack(int i,uint8_t Direction);    
void IncrementTrack();
void DecrementTrack();
void BumpForeward(int x);
void BumpBack(int x);
void SetHome();
void Turn180();
void touchCommand(int boxCode);
void setTrack(int track, long position, bool reverse);
void setReferences(int track, long position, bool reverse);
void positionCheck();
long absPosition(long position);
void startChecking();

void setTrackDefaults();
#endif
