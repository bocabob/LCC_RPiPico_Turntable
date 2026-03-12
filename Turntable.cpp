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

#include "Turntable.h"
#include "BoardSettings.h"
#include "UserInterface.h"
// #include "AccelStepper.h"
#include "StepperSetup.h"
#include "src/drivers/canbus/can_types.h"
#include "config_mem_helper.h"

extern openlcb_node_t *OpenLcbUserConfig_node_id;

extern config_mem_t ConfigMemHelper_config_data;

extern void writeNVMdefaults();
extern void readNVM();
extern void produceLightIn();
extern void produceLightEx();
extern void produceOpenAll();
extern void produceCloseAll();
extern void produceDoor(int servo);
extern long getSteps();
extern void writeSteps(long steps);
extern void getTrack();
extern void writeTrack(uint8_t i,uint8_t Direction);
extern void writeCount();
extern void getCount();
extern void writeTracks();
extern void getTracks();
extern void writeReferences();
extern void getReferences();

// #include <Adafruit_PWMServoDriver.h>
// #include <PCA9685_servo_driver.h>
// #include <PCA9685_servo.h>


// void StartMoveHandler(uint16_t Address);	// Servo callback
// void StopMoveHandler(uint16_t Address);		// Servo callback
// void init_servo(PCA9685_servo& servo, uint8_t mode, int16_t minRange, int16_t maxRange, int16_t position, uint8_t address, uint64_t TConstDur);


const long sanitySteps = SANITY_STEPS;              // Define an arbitrary number of steps to prevent indefinite spinning if homing/calibrations fails.
const uint8_t homeSensorPin = HOME_SENSOR_PIN;                    // Define pin 3 for the home sensor.
const uint8_t bridgeSensorPin = BRIDGE_SENSOR_PIN;                    // Define pin 3 for the home sensor.
const long homeSensitivity = HOME_SENSITIVITY;      // Define the minimum number of steps required before homing sensor deactivates.
const long bridgeSensitivity = BRIDGE_SENSITIVITY;      // Define the minimum number of steps required before bridge sensor deactivates.
const int16_t totalMinutes = 21600;                 // Total minutes in one rotation (360 * 60)

uint8_t homed = 0;                                  // Flag to indicate homing state: 0 = not homed, 1 = homed, 2 = failed.
bool calibrating = false;                           // Flag to prevent other rotation activities during calibration.
uint8_t calibrationPhase = 0;                       // Flag for calibration phase.
unsigned long calMillis = 0;                        // Required for non blocking calibration pauses.

long currentStep = 0;                               // Holds the current step value for registration checks.
long lastStep = 0;                                  // Holds the last step value we moved to (enables least distance moves).
long fullTurnSteps;                                 // Assign our defined full turn steps from config.h.
long halfTurnSteps;                                 // Defines a half turn to enable moving the least distance.
long phaseSwitchStartSteps;                         // Defines the step count at which phase should automatically invert.
long phaseSwitchStopSteps;                          // Defines the step count at which phase should automatically revert.
long lastTarget = sanitySteps;                      // Holds the last step target (prevents continuous rotatins if homing fails).

uint8_t trackCount = 0;                             // count of turntable tracks
uint8_t refCount = 0;                             // count of reference points detected
uint8_t refCountDec = 0;                          // countdown of reference points detected
bool homeSensorState;                               // Stores the current home sensor state.
bool bridgeSensorState;                              // Stores the current bridge sensor state.
bool lastHomeSensorState;                           // Stores the last home sensor state.
bool lastBridgeSensorState;                          // Stores the last bridge sensor state.
unsigned long lastBridgeDebounce = 0;                // Stores the last time the bridge sensor switched for debouncing.
unsigned long lastHomeDebounce = 0;                 // Stores the last time the home sensor switched for debouncing.
uint16_t lastAddr = 0xFFFF;
uint8_t lastDirection = 0xFF;
uint8_t LastTrack = 0;
uint8_t CurrentTrack = 0;
uint8_t BridgeOrientation = 0;
uint8_t BridgeRotation = 0;
uint8_t homeTrack = 3;                                // where you want to call home

uint8_t ledState = 7;                               // Flag for the LED state: 4 on, 5 slow, 6 fast, 7 off.
bool ledOutput = LOW;                               // Boolean for the actual state of the output LED pin.
unsigned long ledMillis = 0;                        // Required for non blocking LED blink rate timing.
// char buff[buffLen];
// uint8_t bufferIndex = 0;

long outside = fullTurnSteps;
long stepAdjustment = 0;
long variance = 0;
long targetSave = 0;
uint8_t closest = 0;
uint8_t inTrack = 0;
bool inBounds = false;
bool rehome = false;

int X_Coord;
int Y_Coord;

// bool box_changed = false; // box flag
uint32_t box_started_ms = millis(); // box debounce timer
uint32_t box_last_change = millis();  // box debounce timer
uint32_t box_db_time = DEBOUNCE_BOX;	// Debounce time (ms) for box command processing.

// LocoNetBus LNbus;
// extern LocoNetDispatcher parser(&LNbus);

// The line below initialised the LocoNet interface for the correct signal polarity of the IoTT LocoNet Interface board
// See: https://myiott.org/index.php/iott-stick/communication-modules/loconet-interface
// extern LocoNetStreamRP2040 lnStream(&Serial1, LOCONET_PIN_RX, LOCONET_PIN_TX, &LNbus, true, true);

// typedef struct
// {
// 	int address;
// 	int pin;
// 	bool active;
// }
// LightAddress;
LightAddress Lights[NumOfLights];

// int LightPin[NumOfLights] {Light_A, Light_B};
// int LightAddr[NumOfLights] {700, 701};

bool lastRunningState;   // Stores last running state to allow turning the stepper off after moves.

// typedef struct
// {
// 	int address;
// 	long trackFront;
// 	long trackBack;
// }
// TrackAddress;
// TrackAddress Tracks[MAX_TRACKS];

ReferenceStep References[NumberOfReferences];

AccelStepper stepper = STEPPER_DRIVER;

bool isStepperRunning()
{
  return stepper.isRunning();
}
long getCurrentPosition()
{
  return stepper.currentPosition();
}
void runStepper()
{
  stepper.run();
}
void disableStepper()
{
  stepper.disableOutputs();
}

void setupStepperDriver()
{
#ifdef STEPPER_ENABLE_PIN
	stepper.setPinsInverted(false, false, true); // Its important that these commands are in this order
	stepper.setEnablePin(STEPPER_ENABLE_PIN);    // otherwise the Outputs are NOT enabled initially
#endif

	stepper.setMaxSpeed(STEPPER_MAX_SPEED);
	stepper.setAcceleration(STEPPER_ACCELERATION);
	stepper.setSpeed(1000);

	lastRunningState = stepper.isRunning();
}


void initializeHardware() {   
  
  // Function configure sensor pins

#if HOME_SENSOR_ACTIVE_STATE == LOW
  pinMode(homeSensorPin, INPUT_PULLUP);
#elif HOME_SENSOR_ACTIVE_STATE == HIGH
  pinMode(homeSensorPin, INPUT);
#endif

#if BRIDGE_SENSOR_ACTIVE_STATE == LOW
  pinMode(bridgeSensorPin, INPUT_PULLUP);
#elif HOME_SENSOR_ACTIVE_STATE == HIGH
  pinMode(bridgeSensorPin, INPUT);
#endif

// int Lights.pin[NumOfLights] {Light_A, Light_B};
// int LightAddr[NumOfLights] {700, 701};
ConfigMemHelper_config_data.Lights[0].pin = Light_A;
ConfigMemHelper_config_data.Lights[0].address = 700;
ConfigMemHelper_config_data.Lights[1].pin = Light_B;
ConfigMemHelper_config_data.Lights[1].address = 701;
// for (int Light=0;Light<NumOfLights;Light++){  // roundhouse lights, move to neoPixels
//     pinMode(ConfigMemHelper_config_data.Lights[Light].pin, OUTPUT);
// 	  digitalWrite(ConfigMemHelper_config_data.Lights[Light].pin, LOW); // turn off
//     ConfigMemHelper_config_data.Lights[Light].active = false;
//   }

// Get the current sensor states
  lastHomeSensorState = digitalRead(homeSensorPin);
  homeSensorState = getHomeState();
  lastHomeSensorState = digitalRead(bridgeSensorPin);
  homeSensorState = getBridgeState();

// Configure relay output pins
  // pinMode(relay1Pin, OUTPUT);
  // pinMode(relay2Pin, OUTPUT);

// Ensure relays are inactive on startup
  setPhase(0);

// Configure LED and accessory output pins
  // pinMode(ledPin, OUTPUT);
  // pinMode(accPin, OUTPUT);
  
#if PHASE_SWITCHING == AUTO
// Calculate phase invert/revert steps
  // processAutoPhaseSwitch();
#endif
// startChecking();
  // this resets all the neopixels to an off state
  // strip.Begin();
  // strip.Show();
}


void LightSwitch(int Light, int dir)  // pin driven LED
{                                              
//   if (Light > (NumOfLights - 1)) return;
//   if (dir)
//     {
//       digitalWrite(ConfigMemHelper_config_data.Lights[Light].pin, HIGH); // turn on
//     }
//     else
//     {
//       digitalWrite(ConfigMemHelper_config_data.Lights[Light].pin, LOW); // turn off
//     }
// #ifdef USE_SENSORS
//       // LN_STATUS lnStatus = LocoNet.reportSensor(LightAddr[Light], dir);
//       // reportSensor(&LNbus, ConfigMemHelper_config_data.Lights[Light].address, dir);
//       Serial.print(F("Tx: Sensor: "));
//       Serial.println(ConfigMemHelper_config_data.Lights[Light].address);
//       // Serial.print(" Status: ");
//       // Serial.println(LocoNet.getStatusStr(lnStatus));
// #endif
}    

void ToggleLight(int Light)  // pin driven LED
{                                               
// if (Light > (NumOfLights - 1)) return;
//   if (ConfigMemHelper_config_data.Lights[Light].active)
//     {
//       digitalWrite(ConfigMemHelper_config_data.Lights[Light].pin, LOW); // turn off
//       ConfigMemHelper_config_data.Lights[Light].active = false;
//     }
//     else
//     {
//       digitalWrite(ConfigMemHelper_config_data.Lights[Light].pin, HIGH); // turn on
//       ConfigMemHelper_config_data.Lights[Light].active = true;
//     }
// #ifdef DEBUG_PRINT
//       // LN_STATUS lnStatus = LocoNet.reportSensor(ConfigMemHelper_config_data.Lights[Light].address, ConfigMemHelper_config_data.Lights[Light].active);
//       // Serial.print(F("Tx: Sensor: "));
//       // Serial.print(ConfigMemHelper_config_data.Lights[Light].address);
//       // Serial.print(" Status: ");
//       // Serial.println(LocoNet.getStatusStr(lnStatus));
// #endif
}

// void MoveServo(int i, int dir)
// {     if (i > (i_max_servo - 1)) return;
//       Serial.print(F("Activating Servo : "));
//       Serial.println(i, DEC);

//       Servos[i].active = true;
//       Servos[i].Status = dir;

//       if (dir)
//       {
//         Serial.print(F("Opening : "));
//         Serial.println(Servos[i].address, DEC);
//       }
//       else
//       {
//         Serial.print(F("Closing : "));
//         Serial.println(Servos[i].address, DEC);
//       }
// }  

void moveHome() {   // Function to find the home position.
  setPhase(0);
  if (getHomeState() == HOME_SENSOR_ACTIVE_STATE) { // at home position
    stepper.stop();
    #if defined(DISABLE_OUTPUTS_IDLE)
        stepper.disableOutputs();
    #endif
    stepper.setCurrentPosition(0);    
   ConfigMemHelper_config_data.CurrentTrack = homeTrack;
   ConfigMemHelper_config_data.BridgeOrientation = 32;
    writeTrack(ConfigMemHelper_config_data.CurrentTrack,ConfigMemHelper_config_data.BridgeOrientation);
    lastStep = 0;
    homed = 1;
    Serial.println(F("Turntable homed successfully"));    
    for (int i = 0; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)); i++)
    { 
    #ifdef USE_SENSORS
      // LN_STATUS lnStatus = LocoNet.reportSensor(ConfigMemHelper_config_data.Tracks[i].address,0);    
      // reportSensor(&LNbus,ConfigMemHelper_config_data.Tracks[i].address,0);  
      Serial.print(F("Tx:  Sensor Off: "));
      Serial.println(ConfigMemHelper_config_data.Tracks[i].address, DEC);
      // Serial.print(F(" Status: "));
      // Serial.println(LocoNet.getStatusStr(lnStatus));   
    #endif       
    }
    #ifdef TT_DEBUG
        Serial.print(F("DEBUG: Stored values for lastStep/lastTarget: "));
        Serial.print(lastStep);
        Serial.print(F("/"));
        Serial.println(lastTarget);
    #endif
  } else // not at home position
    if(!stepper.isRunning()) {  // a problem if stopped
      #ifdef TT_DEBUG
          Serial.print(F("DEBUG: Recorded/last actual target: "));
          Serial.print(lastTarget);
          Serial.print(F("/"));
          Serial.println(stepper.targetPosition());
      #endif
      if ((stepper.targetPosition() > lastTarget + REHOME_THRESHOLD)||(stepper.targetPosition() < lastTarget - REHOME_THRESHOLD)) { // expand this to a range +/-
        stepper.setCurrentPosition(0);
       ConfigMemHelper_config_data.CurrentTrack = homeTrack;
       ConfigMemHelper_config_data.BridgeOrientation = 32;
        lastStep = 0;
        homed = 2;
        Serial.println(F("ERROR: Turntable failed to home, setting random home position"));
      } else {
        stepper.enableOutputs();
        stepper.move(sanitySteps);
        lastTarget = stepper.targetPosition();
        #ifdef TT_DEBUG
              Serial.print(F("DEBUG: lastTarget: "));
              Serial.println(lastTarget);
        #endif
        Serial.println(F("Homing started"));
      }
    }
}

// Function to move to the indicated position.
void moveToPosition(long steps, uint8_t phaseSwitch) {
  long moveSteps;    
  if (steps != lastStep) {
    Serial.print(F("move to step postion "));
    Serial.print(steps);
    Serial.print(F(" from "));
    Serial.println(absPosition(stepper.currentPosition()));
    Serial.print(F("Position steps: "));
    Serial.print(steps);
#if PHASE_SWITCHING == AUTO
    Serial.print(F(", Auto phase switch"));
#else
    Serial.print(F(", Phase switch flag: "));
    Serial.print(phaseSwitch);
#endif
// determine which way to go to get to the position quickest
    if ((steps - lastStep) > halfTurnSteps) {
      moveSteps = steps - fullTurnSteps - lastStep; 
    } else if ((steps - lastStep) < -halfTurnSteps) {
      moveSteps = fullTurnSteps - lastStep + steps;
    } else {
      moveSteps = steps - lastStep; 
    }
    if (moveSteps > 0) 
      BridgeRotation = 0; // go clockwise
    else
      BridgeRotation = 32; // go counter-clockwise
    Serial.print(F(" - moving "));
    Serial.print(moveSteps);
    Serial.println(F(" steps"));
#if PHASE_SWITCHING == AUTO
    if ((steps >= 0 && steps < phaseSwitchStartSteps) || (steps <= fullTurnSteps && steps >= phaseSwitchStopSteps)) {
      phaseSwitch = 0;
    } else {
      phaseSwitch = 1;
    }
#endif
    Serial.print(F("Setting phase switch flag to: "));
    Serial.println(phaseSwitch);
    setPhase(phaseSwitch);
    lastStep = steps;
    stepper.enableOutputs();
    stepper.move(moveSteps);
    lastTarget = stepper.targetPosition();
    startChecking();
#ifdef TT_DEBUG
    Serial.print(F("DEBUG: Stored values for lastStep/lastTarget: "));
    Serial.print(lastStep);
    Serial.print(F("/"));
    Serial.println(lastTarget);
#endif
  }
}

void MoveToTrack(int i,uint8_t Direction){ 
    if (i > MAX_TRACKS) return;
#ifdef USE_SENSORS      
      // LN_STATUS lnStatus = LocoNet.reportSensor(ConfigMemHelper_config_data.Tracks[ConfigMemHelper_config_data.CurrentTrack].address,0);     
      // reportSensor(&LNbus,ConfigMemHelper_config_data.Tracks[ConfigMemHelper_config_data.CurrentTrack].address,0);       
      Serial.print(F("Tx:  Sensor Off: "));
      Serial.println(ConfigMemHelper_config_data.CurrentTrack, DEC);
      // Serial.println(ConfigMemHelper_config_data.Tracks[ConfigMemHelper_config_data.CurrentTrack].address, DEC);
      // Serial.print(F(" Status: "));
      // Serial.println(LocoNet.getStatusStr(lnStatus));
#endif      
      lastAddr = ConfigMemHelper_config_data.Tracks[i].address;
      lastDirection = Direction;
      LastTrack =ConfigMemHelper_config_data.CurrentTrack;
     ConfigMemHelper_config_data.CurrentTrack = i;
     ConfigMemHelper_config_data.BridgeOrientation = Direction;
      writeTrack(ConfigMemHelper_config_data.CurrentTrack,ConfigMemHelper_config_data.BridgeOrientation);  // write track number in EEPROM
      Serial.print(F("Moving to Track : "));
      Serial.println(i, DEC);
#ifdef USE_SENSORS
      // lnStatus = LocoNet.reportSensor(ConfigMemHelper_config_data.Tracks[i].address,1);    
      // reportSensor(&LNbus,ConfigMemHelper_config_data.Tracks[i].address,1);    
      Serial.print(F("Tx:  Sensor On: "));
      // Serial.println(ConfigMemHelper_config_data.Tracks[i].address, DEC);
      // Serial.print(F(" Status: "));
      // Serial.println(LocoNet.getStatusStr(lnStatus));
#endif     
#ifdef STEPPER_ENABLE_PIN
      stepper.enableOutputs();
#endif
      if (Direction)
      {          
      //  stepper.moveTo(ConfigMemHelper_config_data.Tracks[i].trackFront);
       moveToPosition(ConfigMemHelper_config_data.Tracks[i].trackFront, Direction);
#ifdef TT_DEBUG        
        Serial.print(F("Moving to Front Position : "));
        Serial.println(ConfigMemHelper_config_data.Tracks[i].trackFront, DEC);
#endif        
 //       break;
      }
      else
      {
      //  stepper.moveTo(ConfigMemHelper_config_data.Tracks[i].trackBack);
       moveToPosition(ConfigMemHelper_config_data.Tracks[i].trackBack, Direction);
#ifdef TT_DEBUG       
        Serial.print(F("Moving to Back Position : "));
        Serial.println(ConfigMemHelper_config_data.Tracks[i].trackBack, DEC);
#endif          
      }
}

// Function to set phase.
void setPhase(uint8_t phase) {         
   return;
// #if RELAY_ACTIVE_STATE == HIGH
//   digitalWrite(relay1Pin, phase);
//   digitalWrite(relay2Pin, phase);
// #elif RELAY_ACTIVE_STATE == LOW
//   digitalWrite(relay1Pin, !phase);
//   digitalWrite(relay2Pin, !phase);
// #endif
}

// Function to set/maintain our LED state for on/blink/off.
// 4 = on, 5 = slow blink, 6 = fast blink, 7 = off.
// void processLED() {
//   uint16_t currentMillis = millis();
//   if (ledState == 4 ) {
//     ledOutput = 1;
//   } else if (ledState == 7) {
//     ledOutput = 0;
//   } else if (ledState == 5 && currentMillis - ledMillis >= LED_SLOW) {
//     ledOutput = !ledOutput;
//     ledMillis = currentMillis;
//   } else if (ledState == 6 && currentMillis - ledMillis >= LED_FAST) {
//     ledOutput = !ledOutput;
//     ledMillis = currentMillis;
//   }
//   digitalWrite(ledPin, ledOutput);  // change this to call NeoPixel
// }

/* Calibration routines
  The calibration functions are used 
    to determine the number of steps required for a single 360 degree rotation.
    set the stepper zero/home position
    locate reference points forward and backward

  These can be trigged when either there are no stored steps in EEPROM, the stored steps are invalid,
  or the calibration command has been initiated.
  Logic:
  - Move away from home if already homed and erase EEPROM.
  - Perform initial home rotation, set to 0 steps when homed.
  - Perform second home rotation, set steps to currentPosition().
  - Write steps to EEPROM.

*/
void calibration() {
  setPhase(0);  
  switch (calibrationPhase) {
  case 0:
    // start to seek home position
    if (!stepper.isRunning() && homed == 0) {  
      Serial.println(F("CALIBRATION: Phase 0, homing..."));
      calibrationPhase = 1;
      stepper.enableOutputs();
      stepper.moveTo(sanitySteps);
      lastStep = sanitySteps;
    }
    break;
  case 1: // check (did case 0) and (at home sensor) and (have moved beyond home if having started there)
    if ((lastStep == sanitySteps) && (getHomeState() == HOME_SENSOR_ACTIVE_STATE) && (stepper.currentPosition() > homeSensitivity)) {  
    // found home
      Serial.println(F("CALIBRATION: Phase 1, found home."));
      stepper.stop();
      stepper.setCurrentPosition(0);
     ConfigMemHelper_config_data.CurrentTrack = homeTrack;
      homed = 1;
      lastTarget = 0;
    }
    else // error
      if (!stepper.isRunning() && stepper.currentPosition() == sanitySteps) {
        Serial.println(F("CALIBRATION: Phase 1, failed to find home..."));
        calibrationPhase = 9;
      }
    break;
  case 2:
    if (getHomeState() == HOME_SENSOR_ACTIVE_STATE && stepper.currentPosition() > homeSensitivity) { 
    // end phase 2, full turns counted
      stepper.stop();
      fullTurnSteps = stepper.currentPosition();
      if (fullTurnSteps < 0) { fullTurnSteps = -fullTurnSteps;  }
      halfTurnSteps = fullTurnSteps / 2;
      // set home position to track zero
      // setTrack(homeTrack,halfTurnSteps,true); 
      // setTrack(homeTrack,halfTurnSteps,false);    // need
    // #if PHASE_SWITCHING == AUTO
    //     processAutoPhaseSwitch();
    // #endif
      writeSteps(fullTurnSteps); // write to EEPROM
      Serial.print(F("Step count completed: "));
      Serial.println(fullTurnSteps);
      stepper.setCurrentPosition(0);  // since we have come back home
     ConfigMemHelper_config_data.CurrentTrack = homeTrack;
      lastStep = stepper.currentPosition();
    }
    else  // error
      if (!stepper.isRunning() && stepper.currentPosition() == sanitySteps) { 
        Serial.println(F("CALIBRATION: Phase 2, failed to find home..."));
        calibrationPhase = 9;
      }
    break;
  case 3:    // phase 3, detect reference forward direction
    if ((getBridgeState() == BRIDGE_SENSOR_ACTIVE_STATE && stepper.currentPosition() > lastStep + bridgeSensitivity)||(!stepper.isRunning() ) ) { 
    //  reference detected
      if (!stepper.isRunning() ) {   
        Serial.print(F("Calibration: Phase 3 completed, reference count: "));
        Serial.println(ConfigMemHelper_config_data.attributes.ReferenceCount);
        lastStep = stepper.currentPosition();
        calibrationPhase = 4;
        refCountDec = ConfigMemHelper_config_data.attributes.ReferenceCount;
        stepper.enableOutputs();
        stepper.moveTo(0);
        lastTarget = 0;
      } else { // reference point found, log it
        lastStep = stepper.currentPosition();
        setReferences(ConfigMemHelper_config_data.attributes.ReferenceCount, lastStep, false);
        ConfigMemHelper_config_data.attributes.ReferenceCount++; // count tracks
      }
    }
    break;
  case 4:
    // phase 4, find reference in the opposite direction
    if (((getBridgeState() == BRIDGE_SENSOR_ACTIVE_STATE) && (stepper.currentPosition() < lastStep - bridgeSensitivity))||(!stepper.isRunning() ) ) { 
      // reference detected
      if (!stepper.isRunning() ) {   
        Serial.print(F("Calibration: Phase 4 completed, reference count: "));
        Serial.println(ConfigMemHelper_config_data.attributes.ReferenceCount);
        calibrationPhase = 0;
        calibrating = false;
        if (activeScreen == 1) {
          displayConfig();
          drawTurnTable();
          drawTracks();
        }
        ConfigMemHelper_write(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
        // writeEEPROM();
      } else { // reference point found, log it
        refCountDec--; // decrement track count
        lastStep = stepper.currentPosition();
        setReferences(refCountDec, lastStep, true);
        #ifdef TT_DEBUG
        Serial.print(F("References count: "));
              Serial.print(refCountDec);
        Serial.print(F(" at position: "));
              Serial.print(ConfigMemHelper_config_data.References[refCountDec].stepsForward);
        Serial.print(F(" and: "));
              Serial.println(ConfigMemHelper_config_data.References[refCountDec].stepsReverse);
        #endif
      }
    }
    break;
  default:
    // statements    
    #if defined(DISABLE_OUTPUTS_IDLE)
      stepper.disableOutputs();
    #endif
    Serial.println(F("CALIBRATION: FAILED, could not home, could not determine step count"));
    calibrating = false;
    calibrationPhase = 0;
    homed = 0;
    // lastTarget = sanitySteps;
    return;
    break;
    }          
}


void startChecking()
{
  outside = fullTurnSteps;
  stepAdjustment = 0;
  variance = 0;
  targetSave = 0;
  closest = 0;
  rehome = false;
}


void positionCheck()

// check this over carefully

{
  currentStep = absPosition(stepper.currentPosition());
  bool CCWdirection = (stepper.currentPosition()<stepper.targetPosition());

  // re-homing check
  if ((CCWdirection) && (getHomeState() == HOME_SENSOR_ACTIVE_STATE) && (stepper.currentPosition() > homeSensitivity) && (rehome)) {    
    targetSave = stepper.targetPosition();
        #ifdef TT_DEBUG
        Serial.println(F(" ** Reset home "));
        #endif
    stepper.setCurrentPosition(0); 
    //ConfigMemHelper_config_data.CurrentTrack = homeTrack;
    lastStep = (currentStep);  
    lastTarget = (lastTarget);
    moveToPosition(targetSave,ConfigMemHelper_config_data.BridgeOrientation);
    stepper.run(); 
    rehome = false;
  }

// find if you are at a reference point
  if ((getBridgeState() == BRIDGE_SENSOR_ACTIVE_STATE) && (stepper.isRunning()) && (!calibrating) && (homed == 1)) { 
    for (int i = 1; i <= (ConfigMemHelper_config_data.attributes.ReferenceCount); i++)   // find the closest reference to position
    { 
        // if ((i != LastTrack) && (i !=ConfigMemHelper_config_data.CurrentTrack) && (abs(currentStep - ConfigMemHelper_config_data.References[i].stepsReverse) < BRIDGE_PRECISION)) { // within range of this track
        if (abs(currentStep - ConfigMemHelper_config_data.References[i-1].stepsReverse) < BRIDGE_PRECISION) { // within range of this reference
          variance =  ConfigMemHelper_config_data.References[i-1].stepsReverse - currentStep;
          if (variance < outside)
            { outside = variance;
              closest = i-1;
              stepAdjustment =  variance;
            }
        }
    } 
  }   
  
  if (!stepper.isRunning()) {   
    if ((abs(outside) > REHOME_THRESHOLD) && (abs(outside) < BRIDGE_PRECISION) ) {
      // reHome, either adjust step position here based on variance or set homed to one for a total rehome which might be operationally inconvenient
      targetSave = absPosition(stepper.targetPosition());  
      #ifdef TT_DEBUG
        Serial.print(F(" ** Regitrack variance: "));
              Serial.print(outside);
        Serial.print(F(" at reference: "));
              Serial.print(closest);
        Serial.print(F(" with adjustment: "));
              Serial.print(stepAdjustment);
        Serial.print(F(" at position: "));
              Serial.print(currentStep);
        Serial.print(F(" for saved target: "));
              Serial.println(targetSave);
        #endif   
      stepper.enableOutputs();
      stepper.setCurrentPosition(currentStep + stepAdjustment);    
      lastStep = (currentStep + stepAdjustment);  
      lastTarget = (lastTarget + stepAdjustment);
      // stepper.move(-stepAdjustment);
      // stepper.runToNewPosition(targetSave-stepAdjustment);
      moveToPosition(targetSave,ConfigMemHelper_config_data.BridgeOrientation);
      stepper.run(); 
      startChecking();
      rehome = true;
    }
  }
}



// #if PHASE_SWITCHING == AUTO   // If phase switching is set to auto, calculate the trigger point steps based on the angle.
// void processAutoPhaseSwitch() {
//   if (PHASE_SWITCH_ANGLE + 180 >= 360) {
//     Serial.print(F("ERROR: The defined phase switch angle of "));
//     Serial.print(PHASE_SWITCH_ANGLE);
//     Serial.println(F(" degrees is invalid, setting to default 45 degrees"));
//   }
// #if PHASE_SWITCH_ANGLE + 180 >= 360
// #undef PHASE_SWITCH_ANGLE
// #define PHASE_SWITCH_ANGLE 45
// #endif
//   phaseSwitchStartSteps = fullTurnSteps / 360 * PHASE_SWITCH_ANGLE;
//   phaseSwitchStopSteps = fullTurnSteps / 360 * (PHASE_SWITCH_ANGLE + 180);
// }
// #endif


bool getHomeState() {   // Function to debounce and get the state of the homing sensor
  bool newHomeSensorState = digitalRead(homeSensorPin);
  if (newHomeSensorState != lastHomeSensorState && (millis() - lastHomeDebounce) > DEBOUNCE_DELAY) {
    lastHomeDebounce = millis();
    lastHomeSensorState = newHomeSensorState;
  }
  return lastHomeSensorState;
}


bool getBridgeState() {   // Function to debounce and get the state of the bridge sensor
  bool newBridgeSensorState = digitalRead(bridgeSensorPin);
  if (newBridgeSensorState != lastBridgeSensorState && (millis() - lastBridgeDebounce) > DEBOUNCE_DELAY) {
    lastBridgeDebounce = millis();
    lastBridgeSensorState = newBridgeSensorState;
  }
  return lastBridgeSensorState;
}


void initiateHoming() {   // Function to reset home state, triggering homing to happen
  homed = 0;
  calibrating = false;
  calibrationPhase = 1;
  moveHome();
  Serial.println(F(" initiating homing..."));
  stepper.enableOutputs();
  stepper.moveTo(sanitySteps);
  lastTarget = sanitySteps;
}

void initiateStepCount() {    // Function to trigger calibration to begin
  calibrating = true;  
  Serial.println(F("CALIBRATION: Phase 2, counting full turn steps..."));
  calibrationPhase = 2;
  stepper.enableOutputs();
  stepper.moveTo(sanitySteps);
  lastStep = sanitySteps;
  if (activeScreen == 1)   drawTurnTable();
}

void initiateReferences() {    // Function to trigger calibration to begin
  calibrating = true;
  ConfigMemHelper_config_data.attributes.ReferenceCount = 0;
  stepper.enableOutputs();
  stepper.moveTo(fullTurnSteps);
  lastTarget = fullTurnSteps;
  calibrationPhase = 3;
  if (activeScreen == 1)   drawTurnTable();
}


// void setLEDActivity(uint8_t activity) {   // Function to set LED activity
//   ledState = activity;
// }


// void setAccessory(bool state) {   // Function to set the state of the accessory pin
//   digitalWrite(accPin, state);
// }

void IncrementTrack(){ 
              // Go to higher position
    Serial.println(F("Increment Track"));
    getTrack();
    if(ConfigMemHelper_config_data.CurrentTrack >= ConfigMemHelper_config_data.attributes.TrackCount)
      {
        MoveToTrack(1,ConfigMemHelper_config_data.BridgeOrientation);
      }
    else
      {
        MoveToTrack(ConfigMemHelper_config_data.CurrentTrack + 1,ConfigMemHelper_config_data.BridgeOrientation);
      }
}
void DecrementTrack(){ 
              //      Go to lower position
    Serial.println(F("Decrement Track"));  
    getTrack();
    if(ConfigMemHelper_config_data.CurrentTrack < 2)
      {
        MoveToTrack(ConfigMemHelper_config_data.attributes.TrackCount,ConfigMemHelper_config_data.BridgeOrientation);
      }
    else
      {
        MoveToTrack(ConfigMemHelper_config_data.CurrentTrack - 1,ConfigMemHelper_config_data.BridgeOrientation);
      }
}
void BumpBar(){
              // bump on X touch value           
    Serial.println(F("Bump X_Coord"));     
      #ifdef STEPPER_ENABLE_PIN
            stepper.enableOutputs();
      #endif
    if (abs(X_Coord - (HRES/2)) > 80) {
      if (X_Coord > (HRES/2) )
        moveToPosition(stepper.currentPosition()+(STEPPER_GEARING_FACTOR * ((X_Coord - (HRES/2)-80)/6)),ConfigMemHelper_config_data.BridgeOrientation);
      else
        moveToPosition(stepper.currentPosition()+(STEPPER_GEARING_FACTOR * ((X_Coord - (HRES/2)+80)/6)),ConfigMemHelper_config_data.BridgeOrientation);
    }
}
void BumpForeward(int x){
              // bump clockwise           
    Serial.println(F("Bump foreward"));     
      #ifdef STEPPER_ENABLE_PIN
            stepper.enableOutputs();
      #endif
    // stepper.move(Bump * x);
    moveToPosition(stepper.currentPosition()+(Bump * x),ConfigMemHelper_config_data.BridgeOrientation);
}
void BumpBack(int x){
                // bump counter-clockwise          
    Serial.println(F("Bump back"));     
      #ifdef STEPPER_ENABLE_PIN
            stepper.enableOutputs();
      #endif
    // stepper.move(-Bump * x);
    moveToPosition(stepper.currentPosition()-(Bump * x),ConfigMemHelper_config_data.BridgeOrientation);
}
void SetHome(){
              // set home to current position         
    Serial.println(F("Set Home Here"));    
    stepper.setCurrentPosition(0);  
   ConfigMemHelper_config_data.CurrentTrack = homeTrack;               
}
void Turn180(){
                //          flip 180
    Serial.println(F("Turn 180 degrees"));      
    if (ConfigMemHelper_config_data.BridgeOrientation == 0) {ConfigMemHelper_config_data.BridgeOrientation = 32;} else {ConfigMemHelper_config_data.BridgeOrientation = 0;}
      #ifdef STEPPER_ENABLE_PIN
            stepper.enableOutputs();
      #endif
    // stepper.move(halfTurnSteps);    
    moveToPosition(absPosition(stepper.currentPosition()+halfTurnSteps),ConfigMemHelper_config_data.BridgeOrientation);                  
}
void Case8(){
              //          change direction
    Serial.println(F("Change Direction"));    
                
}

void touchCommand(int boxCode)
{ 
/*
0 = null spot
 PageBoxes 20        
1 = bridge
2 = bridge shack
3 - 20 = page buttons

variable selection buttons = 2 * variables, add to pageboxes to get track count variable box #define TrackSelBoxes 24    
Track setting buttons = 7 * tracks, add to TrackSelBoxes to get track box starting point #define TrackBoxes 144      
Servo selection buttons = 2 * variables, add to TrackBoxes to get servo variable update box #define ServoSelBoxes 146   
Servo setting buttons = 6 * servos, add to TrackSelBoxes to get track box starting point #define ServoBox 206       
Turntable track operation buttons = 3 * tracks, add to ServoBox to get track box starting point #define TrackBox 266      

 PossibleBoxes  270    // space for array of click box boundaries (static boxes plus track boxes)

*/
// check for debounce
box_started_ms = millis();      
if (box_started_ms - box_last_change < box_db_time)     return;     // Debounce time not ellapsed.

// box_changed = false;  // Debounce time has not ellapsed.
// // box_changed = (box_current_state != box_last_state); // Report state change if current state vary from last state. 

// box_last_state = box_current_state;				// Save last state.
// box_current_state = boxCode;					// Assign new state as current state . 
// if (!(box_current_state == box_last_state)) {box_changed = true;};
// if (box_changed) {           // State changed.
//   Serial.println(' ');
//   Serial.print(" box changed   ");  
//   box_last_change = read_started_ms;        // Save current millis as last change time.
// }

#ifdef TT_DEBUG
  // tft.setCursor(300, 270,  2);
  // tft.print("HotSpot is ");tft.print(HotSpotBox(X_Coord,Y_Coord));tft.print("     ");
  // tft.setCursor(300, 285,  2);
  // tft.print("X = ");tft.print(X_Coord);tft.print("   ");
  // tft.setCursor(300, 300,  2);
  // tft.print("Y = ");tft.print(Y_Coord);tft.print("   ");
#endif  
  if (boxCode <= PageBoxes) { // main screen buttons
    switch (boxCode) {
    case 1:      // bridge 
      Turn180();
      break;
    case 2:      // bridge shack
      TogglePixels();
      drawShack((absPosition(stepper.currentPosition())*360)/fullTurnSteps);
      break;
    case 3:      // find reference positions
      initiateReferences();
      break;
    case 4:    // Re-home
      Serial.println(F("Reset Home Position...."));
      initiateHoming();
      break;
    case 5:      // decrement
      DecrementTrack();
      break;
    case 6:      // Bump Bar
      BumpBar();
      break;      
    case 7:      // toggle RH interior lights
      produceLightIn();
      // ToggleLight(0);
      break;
    case 8:      // toggle RH exterior lights
      produceLightEx();
      // ToggleLight(1);
      break;    
    case 9:      // button 5
      IncrementTrack();
      break;      
    case 10:      // open all doors 
      produceOpenAll();    
      for (int track = 0; track <= ConfigMemHelper_config_data.attributes.TrackCount; track++) {
      if (ConfigMemHelper_config_data.Tracks[track].doorPresent) 
      {
        // MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 32);            
        // drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
        switch (activeScreen)
        {
        case  1:
          /* code */
          drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
          break;
        case 2:
          /* code */
          drawDoorButton(track);
          break;
        default:
          break;
        }
      }
      }
      break;
    case 11:      // close all doors 
      produceCloseAll();
      for (int track = 0; track <= ConfigMemHelper_config_data.attributes.TrackCount; track++) {
      if (ConfigMemHelper_config_data.Tracks[track].doorPresent) 
      {
        // MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 0);            
        // drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
        switch (activeScreen)
        {
        case  1:
          /* code */
          drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
          break;
        case 2:
          /* code */
          drawDoorButton(track);
          break;
        default:
          break;
        }
      }
      }
      break;    
    case 12:      // settings page
      // drawSettingsPage();
      break;
    case 13:      // diagnostics page
      drawDiagnosticPage();
      break;    
    case 14:      // turn table page
      drawHomePage();
      drawTracks();
      break;
    case 15:      // clear EEPROM
      // clearEEPROM();
      ConfigMemHelper_clear_config_mem();
      drawDiagnosticPage();
      break;    
    case 16:      // read EEPROM
      {
      // long savedSteps = readEEPROM();
      long savedSteps = getSteps();
      drawDiagnosticPage();
      break;
      }
    case 17:      // write EEPROM   
    #ifdef TT_DEBUG
      Serial.println(F("DEBUG: write EEPROM "));
      Serial.println(boxCode);
    #endif   
      // writeEEPROM();
      ConfigMemHelper_write(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
      drawDiagnosticPage();
      break;    
    case 18: // settings page
      drawButtonPage();
      // drawConfigPage();
      break;    
    case 19:
      // determine step count - make button?
      initiateStepCount();
      break;
    case 20:
      // decrement fullTurnSteps
      --fullTurnSteps;
      // drawSteps();
      break;    
    case 21:
      // increment fullTurnSteps
      ++fullTurnSteps;
      // drawSteps();
      break;    
    case 22:
      // read step count from storage
      // fullTurnSteps = getSteps();
      // drawSteps();
      break;    
    case 23:
      // write step count to storage
      // writeSteps(fullTurnSteps);
      // drawSteps();
      break;    
    case 24:
      // read references from storage
      getReferences();
      drawDiagnosticPage();
      break;    
    case 25:
      // write references to storage
      writeReferences();
      // drawConfigPage();
      break;    
    case 26:
      //  load default tracks - updated to CDI data      
      // writeNVMdefaults();
      // readNVM();
      // setTrackDefaults(); // this updates just to RAM, not CDI
      drawDiagnosticPage();
      break;    
    case 27:
      // read tracks from storage
      getTracks();
      drawDiagnosticPage();
      break;    
    case 28:
      // write tracks to storage
      writeTracks();
      // drawConfigPage();
      break;    
    case 29:
      //  load default servos
      // setServoDefaults();
      drawDiagnosticPage();
      break;    
    case 30:
      // read servos from storage
      // getServos();
      drawDiagnosticPage();
      break;    
    case 31:
      // write servos to storage
      // writeServos();
      // drawConfigPage();
      break;    
    default:
      /*
      
      */ 
    #ifdef TT_DEBUG
      Serial.println(F("DEBUG: default switch case "));
      Serial.println(boxCode);
    #endif   
      break;
    }    
  } 
  else  // process a track variable box  
  {
    if (boxCode <= TrackSelBoxes) {
      int action = (boxCode-PageBoxes-1);
      switch (action){
      case 0:
        // decrement track count
        if (ConfigMemHelper_config_data.attributes.TrackCount > 0){
           --ConfigMemHelper_config_data.attributes.TrackCount;
           writeCount();
        }
        break;
      case 1:
        // increment track count
        if (ConfigMemHelper_config_data.attributes.TrackCount < NUM_TRACKS) {
           ++ConfigMemHelper_config_data.attributes.TrackCount;
           writeCount();
        }
        break;
      case 2:
        // decrement track edit selection
        if (ConfigMemHelper_config_data.attributes.ReferenceCount > 0) --ConfigMemHelper_config_data.attributes.ReferenceCount;
        break;
      case 3:
        // increment track edit selection
        if (ConfigMemHelper_config_data.attributes.ReferenceCount < NumberOfReferences) ++ConfigMemHelper_config_data.attributes.ReferenceCount;
        break;
      case 4:
        // decrement track edit selection
        if (editTrack > 0) --editTrack;
        break;
      case 5:
        // increment track edit selection
        if (editTrack < ConfigMemHelper_config_data.attributes.TrackCount) ++editTrack;
        break;
      default:
        // statements
        break;
      }   
      // drawSetting(editTrack);
    }
    else  // process a track box
    {
      if (boxCode < TrackBoxes) {
        int action = (boxCode-TrackSelBoxes-1) % 7;
        switch (action){
        case 0:
          // decrement address
          if (ConfigMemHelper_config_data.Tracks[editTrack].address > 0) --ConfigMemHelper_config_data.Tracks[editTrack].address ;
          break;
        case 1:
          // increment address
          // if (ConfigMemHelper_config_data.Tracks[editTrack].address < MaxDCCaddress) ++ConfigMemHelper_config_data.Tracks[editTrack].address;
          break;
        case 2:
          // decrement step position
          if (ConfigMemHelper_config_data.Tracks[editTrack].trackFront > 0) {
            --ConfigMemHelper_config_data.Tracks[editTrack].trackFront;
            --ConfigMemHelper_config_data.Tracks[editTrack].trackBack;
          }
          break;
        case 3:
          // increment step position
          if (ConfigMemHelper_config_data.Tracks[editTrack].trackFront < fullTurnSteps) {
            ++ConfigMemHelper_config_data.Tracks[editTrack].trackFront;
            ++ConfigMemHelper_config_data.Tracks[editTrack].trackBack;
          }
          break;
        case 4:
          // toggle door presence to track w/redraw
          if (ConfigMemHelper_config_data.Tracks[editTrack].doorPresent)
          {ConfigMemHelper_config_data.Tracks[editTrack].doorPresent = false;}
          else
          {ConfigMemHelper_config_data.Tracks[editTrack].doorPresent = true;}
          break;
        case 5:
          // decrement servo number
          if (ConfigMemHelper_config_data.Tracks[editTrack].servoNumber > 0) --ConfigMemHelper_config_data.Tracks[editTrack].servoNumber ;
          break;
        case 6:
          // increment servo number
          // if (ConfigMemHelper_config_data.Tracks[editTrack].servoNumber < i_max_servo) ++ConfigMemHelper_config_data.Tracks[editTrack].servoNumber;
          break;
        default:
          // statements
          break;
        }
        // drawSetting(editTrack);
      }
      else  // process a servo variable box  
      {
        if (boxCode <= ServoSelBoxes) {
          int action = (boxCode-TrackBoxes-1);
          switch (action){
          case 0:
            // decrement track count
            if (editServo > 0) --editServo;
            break;
          case 1:
            // increment track count
            // if (editServo < i_max_servo) ++editServo;
            break;
          default:
            // statements
            break;
          }      
          // drawServo(editServo);
        }
        else {  // buttons on servo settings page ServoSelBoxes
          if (boxCode < TrackBox) {
            int action = (boxCode-ServoSelBoxes-1) % 6;
            switch (action){
          case 0:
            // decrement address
            // if (Servos[editServo].address > 0) --Servos[editServo].address ;
            break;
          case 1:
            // increment address
            // if (Servos[editServo].address < MaxDCCaddress) ++Servos[editServo].address;
            break;
          case 2:
            // decrement servo minimum range
            // if (Servos[editServo].ServoMin > MinServoRange) --Servos[editServo].ServoMin ;
            break;
          case 3:
            // increment servo minimum range
            // if (Servos[editServo].ServoMin < MaxServoRange) ++Servos[editServo].ServoMin;
            break;
          case 4:
            // decrement servo minimum range
            // if (Servos[editServo].ServoMax > MinServoRange) --Servos[editServo].ServoMax ;
            break;
          case 5:
            // increment servo minimum range
            // if (Servos[editServo].ServoMax < MaxServoRange) ++Servos[editServo].ServoMax;
            break;
            default:
              // statements
              break;
            }
            // drawServo(editServo);
          }
          else { // track buttons from the turntable diagram
            int track = (boxCode - TrackBox)/3;
            int action = (boxCode-TrackBox) % 3;
            switch (action){
            case 0:
              // move front side to track
              // move to track foreward
              MoveToTrack(track,32);
              break;
            case 1:
              // move back side to track
              // move to track backward
              MoveToTrack(track,0);
              break;
            case 2:
              // toggle door to track w/redraw
              // Serial.print(F("Door: "));
              // Serial.print(track);
              if (ConfigMemHelper_config_data.Tracks[track].doorPresent) 
              {
                produceDoor(ConfigMemHelper_config_data.Tracks[track].servoNumber);
              // Serial.print(F("  Servo: "));
              // Serial.println(ConfigMemHelper_config_data.Tracks[track].servoNumber);
                // if (Servos[ConfigMemHelper_config_data.Tracks[track].servoNumber].Status)
                // {              MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 0);            }
                // else
                // {              MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 32);            }
                switch (activeScreen)
                {
                case  1:
                  /* code */
                  drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
                  break;
                case 2:
                  /* code */
                  drawDoorButton(track);
                  break;
                default:
                  break;
                }
              }
              break;
            default:
              // statements
              break;
            }
          }
        }
      }
    }
  }
  
#ifdef TT_DEBUG  
    // Serial.print(F("Buffer: "));
    // Serial.println(buff);
#endif          
}

long absPosition(long position)
{ if (fullTurnSteps <= 0) return position;
  while (position < 0) { position = position + fullTurnSteps;  }
  while (position > fullTurnSteps) { position = position - fullTurnSteps;  }
  return position;
}

void setTrack(int track, long position, bool reverse)
{ if ((track >= 0) && (track < NUM_TRACKS)) {
    position = absPosition(position);
    if (reverse) {      
      ConfigMemHelper_config_data.Tracks[track].trackBack = position;
    } 
    else {
      ConfigMemHelper_config_data.Tracks[track].trackFront = position;
      if (position > halfTurnSteps) {  
        ConfigMemHelper_config_data.Tracks[track].trackBack = position - halfTurnSteps;
      }
      else {
        ConfigMemHelper_config_data.Tracks[track].trackBack = position + halfTurnSteps;
      }
    }
  }  
}
void setReferences(int spot, long position, bool reverse)
{ if ((spot >= 0) && (spot < NumberOfReferences)) {
    position = absPosition(position);
    if (reverse) ConfigMemHelper_config_data.References[spot].stepsReverse = position;
    else ConfigMemHelper_config_data.References[spot].stepsForward = position;
  }  
}
void setTrackDefaults()
{
  for (int i = 0; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)); i++) {
      // ConfigMemHelper_config_data.Tracks[i].address = TrackStartAddress - 1 + i;
      ConfigMemHelper_config_data.Tracks[i].trackFront = 0;
      ConfigMemHelper_config_data.Tracks[i].trackBack = (FULL_TURN_STEPS / 2);
      ConfigMemHelper_config_data.Tracks[i].doorPresent = false;
      ConfigMemHelper_config_data.Tracks[i].servoNumber = 0;
    }
  for (int i = 4; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)-1); i++) {
      ConfigMemHelper_config_data.Tracks[i].doorPresent = true;
      // ConfigMemHelper_config_data.Tracks[i].servoNumber = (i-4) % i_max_servo;
    }
  ConfigMemHelper_config_data.attributes.TrackCount = NUM_TRACKS;
  
  homeTrack = 3;
	// track zero is the position of the homing sensor
	// ConfigMemHelper_config_data.Tracks[1].address = 500; // TrackStartAddress
	ConfigMemHelper_config_data.Tracks[1].trackFront = absPosition(entryTrack1);
	ConfigMemHelper_config_data.Tracks[1].trackBack = absPosition(entryTrack1 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[2].address = 501;
	ConfigMemHelper_config_data.Tracks[2].trackFront = absPosition(entryTrack2);
	ConfigMemHelper_config_data.Tracks[2].trackBack = absPosition(entryTrack2 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[3].address = 502;
	ConfigMemHelper_config_data.Tracks[3].trackFront = absPosition(entryTrack3);
	ConfigMemHelper_config_data.Tracks[3].trackBack = absPosition(entryTrack3 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[4].address = 503;
	ConfigMemHelper_config_data.Tracks[4].trackFront = absPosition(houseTrack1);
	ConfigMemHelper_config_data.Tracks[4].trackBack = absPosition(houseTrack1 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[5].address = 504;
	ConfigMemHelper_config_data.Tracks[5].trackFront = absPosition(houseTrack2);
	ConfigMemHelper_config_data.Tracks[5].trackBack = absPosition(houseTrack2 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[6].address = 505;
	ConfigMemHelper_config_data.Tracks[6].trackFront = absPosition(houseTrack3);
	ConfigMemHelper_config_data.Tracks[6].trackBack = absPosition(houseTrack3 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[7].address = 506;
	ConfigMemHelper_config_data.Tracks[7].trackFront = absPosition(houseTrack4);
	ConfigMemHelper_config_data.Tracks[7].trackBack = absPosition(houseTrack4 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[8].address = 507;
	ConfigMemHelper_config_data.Tracks[8].trackFront = absPosition(houseTrack5);
	ConfigMemHelper_config_data.Tracks[8].trackBack = absPosition(houseTrack5 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[9].address = 508;
	ConfigMemHelper_config_data.Tracks[9].trackFront = absPosition(houseTrack6);
	ConfigMemHelper_config_data.Tracks[9].trackBack = absPosition(houseTrack6 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[10].address = 509;
	ConfigMemHelper_config_data.Tracks[10].trackFront = absPosition(houseTrack7);
	ConfigMemHelper_config_data.Tracks[10].trackBack = absPosition(houseTrack7 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[11].address = 510;
	ConfigMemHelper_config_data.Tracks[11].trackFront = absPosition(houseTrack8);
	ConfigMemHelper_config_data.Tracks[11].trackBack = absPosition(houseTrack8 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[12].address = 511;
	ConfigMemHelper_config_data.Tracks[12].trackFront = absPosition(houseTrack9);
	ConfigMemHelper_config_data.Tracks[12].trackBack = absPosition(houseTrack9 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[13].address = 512;
	ConfigMemHelper_config_data.Tracks[13].trackFront = absPosition(houseTrack10);
	ConfigMemHelper_config_data.Tracks[13].trackBack = absPosition(houseTrack10 + (FULL_TURN_STEPS / 2));

	// ConfigMemHelper_config_data.Tracks[14].address = 513;
	ConfigMemHelper_config_data.Tracks[14].trackFront = absPosition(houseTrack11);
	ConfigMemHelper_config_data.Tracks[14].trackBack = absPosition(houseTrack11 + (FULL_TURN_STEPS / 2));
}

// void setServoDefaults()
// // memory for the Status of servo:  
// //			0 = deviate position
// //      1 = correct position
// //      2 = Status on start sketch

// {	
//   for (int i = 0; i < (sizeof(Servos) / sizeof(ServoAddress)); i++) {
//       Servos[i].address = ServoStartingAddress + i;	// DCC address for this servo
//       Servos[i].active = false; // servo in use flag
//       Servos[i].Status = 2; // flag for opening or closing of servo
//       Servos[i].Position = 45; // current position
//     }
  
//   // these are Status arrays for each servo
// 	Servos[0].ServoMin = -45;	// position of servo at close
// 	Servos[0].ServoMax = 70;	// position of servo at open

// 	Servos[1].ServoMin = -45;
// 	Servos[1].ServoMax = 70;

// 	Servos[2].ServoMin = -45;
// 	Servos[2].ServoMax = 50;

// 	Servos[3].ServoMin = -45;
// 	Servos[3].ServoMax = 50;

// 	Servos[4].ServoMin = -45;
// 	Servos[4].ServoMax = 60;
  
// 	Servos[5].ServoMin = -45;
// 	Servos[5].ServoMax = 45;
  
// 	Servos[6].ServoMin = -45;
// 	Servos[6].ServoMax = 56;
  
// 	Servos[7].ServoMin = -45;
// 	Servos[7].ServoMax = 40;
  
// 	Servos[8].ServoMin = -45;
// 	Servos[8].ServoMax = 45;
  
// 	Servos[9].ServoMin = -45;
// 	Servos[9].ServoMax = 45;

/*	repeat the above construct for any additional servos implemented 	*/
// }