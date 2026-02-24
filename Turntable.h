/*
 * Parts of this © 2022 Peter Cole, 2023-5 Bob Gamble
 *
 *  This file is a part of the LocoNet Turntable project
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
// #include "AccelStepper.h"
#include <I2C_eeprom.h>
#include <SPI.h>    // Call up the TFT driver library
#include <TFT_eSPI.h>      // Hardware-specific library
// #include "src/application_drivers/my_bb_captouch.h"
// #include "TAMC_GT911.h"


// #include <Adafruit_PWMServoDriver.h>

bool isStepperRunning();
long getCurrentPosition();
void runStepper();
void disableStepper();
void initializeHardware();
// void setupServos();
// void MoveServo(int i, int dir);
void LightSwitch(int Light, int dir);
void ToggleLight(int Light);
// void driveServos();
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
void Case8();
void touchCommand(int boxCode);
void setTrack(int track, long position, bool reverse);
void setReferences(int track, long position, bool reverse);
void positionCheck();
long absPosition(long position);
void startChecking();

void setTrackDefaults();
// void setServoDefaults();
#endif
