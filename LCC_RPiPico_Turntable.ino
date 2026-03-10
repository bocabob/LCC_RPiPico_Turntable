/** \copyright
 * Copyright (c) 2025, Jim Kueneman, Bob Gamble, David Harris, and Alex Shepherd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file LCC_RPiPico_Turntable.ino
 *
 * This sketh will create a very basic OpenLcb Node.  It needs the Config Memory handlers and a reset impementation finished (esp32_drivers.c)
 * Connect the CAN transciever Tx pin to GPIO 21 adn the Rx pin to GPIO 22 on the ESP32 Dev Board.
 *
 * @author Bob Gamble, David Harris, Alex Shepherd, and Jim Kueneman
 * @date 23 Feb 2025
 */
/* LCC Stepper Motor Controller ( A4988 ) Example
 Author: Alex Shepherd 2017-12-04, modified by Bob Gamble for LocoNet 2022-1-21, with code from Peter Cole's EX-Turntable project Oct 2023 
 Rewritten by Bob Gamble for LCC 2025-1-10
 The original example required two Arduino Libraries:
 1) The AccelStepper library from: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html
 2) The OpenLCB single thread library is used for LCC
the stepper library is included as modifications have been made

 This current program uses a Raspberry Pi Pico:
1) the OpenLCB single thread, Wire, AccelStepper, TFT, Touch, and I2C_eeprom libraries;
2) 
3) A4988 stepper motor driver module or similar
4) Hall sensor(s) for indexing
5) parallel Touch TFT for control and status
6) NeoPixel for lights
7) 
8) I2C EEPROM for saving states

Pin usage specified in the configuration file:
  The LCC Shield uses pins for the HW CAN controller MCP2518
  The A4988 module interface uses three pins
  The Hall sensors uses a pin each for sensing
  The I2C uses pins
  The NeoPixel uses a pin 

Other pins for relays and LEDs may be used.
//==============================================================
// Pico LCC Node with 2518 HW controller
// Modified DPH 2024 RSG 2025
// Copyright 2019 Alex Shepherd and David Harris and Bob Gamble, 2025
//==============================================================
*/
#include "Arduino.h"
#include <Wire.h>
#include <LibPrintf.h>

// #define USER_DEFINED_CONSUMER_COUNT 60

#include "BoardSettings.h"

#include "callbacks.h"
#include "openlcb_user_config.h"
#include "config_mem_helper.h"

#include "src/pico/rpi_pico_drivers.h"
#include "src/pico/rpi_pico_can_drivers.h"

#include "src/drivers/canbus/can_config.h"
#include "src/openlcb/openlcb_config.h"
#include "src/openlcb/openlcb_application.h"
#include "src/utilities/mustangpeak_endian_helper.h"
#include "src/openlcb/openlcb_application_broadcast_time.h"

#include "TTvariables.h"
#include <TFT_eSPI.h>      // Hardware-specific library
// Include local files
#include "Turntable.h"
#include "src/application_drivers/AccelStepper.h"
#include "UserInterface.h"

// #define NODE_ID 0x050101010777
#define NODE_ID 0x050101019411      // 05 01 01 01 94 ** range assigned to Bob Gamble / Southern Piedmont
// #define NODE_ADDRESS  5,1,1,1,94,0x08   // 05 01 01 01 94 ** range assigned to Bob Gamble / Southern Piedmont


////// DECLARATIONS

// extern AccelStepper stepper;
extern bool lastRunningState;   // Stores last running state to allow turning the stepper off after moves.

extern AccelStepper stepper;
extern bool isStepperRunning();
extern long getCurrentPosition();
extern void runStepper();
extern void disableStepper();

bool setupComplete = false;
bool setup1Complete = false;
bool StorageReady = false;

bool stepsSet;

npStrings _Strings[MAX_STRINGS];
// MemStruct CDI_RAM;
// extern bool GroupState[MAX_GROUPS];   // defined in NPlights.cpp

static const can_config_t can_config = {
    .transmit_raw_can_frame  = &RPiPicoCanDriver_transmit_raw_can_frame,
    .is_tx_buffer_clear      = &RPiPicoCanDriver_is_can_tx_buffer_clear,
    .lock_shared_resources   = &RPiPicoDrivers_lock_shared_resources,
    .unlock_shared_resources = &RPiPicoDrivers_unlock_shared_resources,
    .on_rx                   = &Callbacks_on_can_rx_callback,
    .on_tx                   = &Callbacks_on_can_tx_callback,
    .on_alias_change         = &Callbacks_alias_change_callback,
};

static const openlcb_config_t openlcb_config = {
    .lock_shared_resources           = &RPiPicoDrivers_lock_shared_resources,
    .unlock_shared_resources         = &RPiPicoDrivers_unlock_shared_resources,
    .config_mem_read                 = &ConfigMemHelper_config_mem_read,
    .config_mem_write                = &ConfigMemHelper_config_mem_write,
    .reboot                          = &RPiPicoDrivers_reboot,
    .factory_reset                   = &Callbacks_operations_request_factory_reset,
    .freeze                          = &Callbacks_freeze,
    .unfreeze                        = &Callbacks_unfreeze,
    .firmware_write                  = &Callbacks_write_firemware,
    .on_100ms_timer                  = &Callbacks_on_100ms_timer_callback,
    .on_login_complete               = &Callbacks_on_login_complete,
    .on_consumed_event_identified    = &Callbacks_on_consumed_event_identified,
    .on_consumed_event_pcer          = &Callbacks_on_consumed_event_pcer,
    .on_broadcast_time_changed       = &Callbacks_on_broadcast_time_time_changed,
};

void _check_for_nvm_initialization(void) {

  Serial.println("Checking for initialized NVM");
  // If the first byte of the configuration memory is 0xFF then the space has never been accessed (fresh firmware load) and need to be initialized to 0x00
  if (ConfigMemHelper_is_config_mem_reset()) {
   
    Serial.println("Initializing Configuration Memory to 0x00");
    ConfigMemHelper_clear_config_mem();
    Serial.println("Writing default values to the Configuration Memory");
    ConfigMemHelper_reset_and_write_default(OpenLcbUserConfig_node_id);
    Serial.println("Defaults set...");

  } else {

    Serial.println("Configuration Memory has been previously initalized");

  }
}

void _register_producers(void) {

  OpenLcbApplication_clear_producer_eventids(OpenLcbUserConfig_node_id);
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.OpenAll), ConfigMemHelper_config_data.producer_status[0]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.CloseAll), ConfigMemHelper_config_data.producer_status[1]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  for (int i = 0; i < MAX_DOORS; i++) {
     OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.doors[i].eidToggle), ConfigMemHelper_config_data.producer_status[2+i]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  }
  
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidInterior), ConfigMemHelper_config_data.producer_status[2+MAX_DOORS]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidExterior), ConfigMemHelper_config_data.producer_status[3+MAX_DOORS]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID 
/*

void produceLightIn()
{
OpenLcb.produce(IndexLightIn);
}
void produceLightEx()
{
OpenLcb.produce(IndexLightEx);
}
void produceOpenAll()
{
OpenLcb.produce(IndexOpenAll);
}
void produceCloseAll()
{
OpenLcb.produce(IndexCloseAll);
}
void produceDoor(int servo)
{
OpenLcb.produce(IndexDoor1 + servo);
}

*/
}

void _register_consumers(void) {

  OpenLcbApplication_clear_consumer_eventids(OpenLcbUserConfig_node_id);

  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.Rehome), ConfigMemHelper_config_data.consumer_status[0]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.IncrementTrack), ConfigMemHelper_config_data.consumer_status[1]);
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.DecrementTrack), ConfigMemHelper_config_data.consumer_status[2]);
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.RotateTrack180), ConfigMemHelper_config_data.consumer_status[3]);
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.ToggleBridgeLights), ConfigMemHelper_config_data.consumer_status[4]);


  for (int i = 0; i < MAX_TRACKS; i++) {
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Front), ConfigMemHelper_config_data.consumer_status[4+i*4]); // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Back), ConfigMemHelper_config_data.consumer_status[5+i*4]);
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Occupancy), ConfigMemHelper_config_data.consumer_status[6+i*4]);
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].RailCom), ConfigMemHelper_config_data.consumer_status[7+i*4]);
  }

    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidBridge), ConfigMemHelper_config_data.consumer_status[8+MAX_TRACKS*4]);
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidHighLuminosity_On), ConfigMemHelper_config_data.consumer_status[9+MAX_TRACKS*4]);
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidLowLuminosity_On), ConfigMemHelper_config_data.consumer_status[10+MAX_TRACKS*4]);

  
}

bool node_initiated = false;

void setup() {
  // put your setup code here, to run once:

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  uint32_t stimer = millis();
  while (!Serial && (millis() - stimer < 3000)) {   // wait for 3 secs for USB/serial connection to be established
    delay(100);
  }

  Serial.println("Can Statemachine init.....");
  
  RPiPicoCanDriver_setup();
  RPiPicoDriver_setup();

  CanConfig_initialize(&can_config);
  OpenLcb_initialize(&openlcb_config);

  Callbacks_initialize();

  Serial.println("Creating Node.....");

  OpenLcbUserConfig_node_id = OpenLcb_create_node(NODE_ID, &OpenLcbUserConfig_node_parameters);
  // do this after initialization or the I2C will not be initialized

  _check_for_nvm_initialization();

  // Read the NVM into the local data structures
  Serial.println("Loading NVM values into Config Mem data variable");
  ConfigMemHelper_read(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
  Serial.println("Data variable loaded and ready for use");
  // TODO: need to load track variables and door variables into the local data structures as well for use in the program, or read from the NVM when needed.  The latter is simpler but less efficient.  The former is more efficient but more complex to implement and maintain.  For now, we will read from the NVM when needed.
  
  // Load_application_defaults(OpenLcbUserConfig_node_id);

  // initStringFlags();
  // SetupPixels();
  // InitialzePixels();  // TODO: JDK THIS CAUSED A HANG ON MY BOARD.... 

  // Now use the data found in the data structures to register the current event IDs
  // need to read states from NVM to know the state when registering the consumer event IDs
  _register_consumers();
  _register_producers();

  OpenLcbApplicationBroadcastTime_setup_consumer(OpenLcbUserConfig_node_id, BROADCAST_TIME_ID_DEFAULT_FAST_CLOCK); // initialize the fast clock

  node_initiated = true;
}


void setup1() {
  // put your setup code here, to run once:
  // while (!Serial) {}
  
  while (!node_initiated)
  {
    delay(1000);  /* wait */
    Serial.print(" . ");
  }
  
  // Run startup configuration
  setupDisplay(); 
  initializeHardware(); 

// // old code *****************************************

  // set defaults
  fullTurnSteps = FULL_TURN_STEPS;
  halfTurnSteps = fullTurnSteps / 2;
  
  Serial.print(fullTurnSteps);
  Serial.println(F(" default steps per revolution"));  

  // check EEPROM for saved values and initialize
  // long savedSteps = readEEPROM();
  long savedSteps = getSteps();
  if (savedSteps > 0)
    {
      fullTurnSteps = savedSteps;
      halfTurnSteps = fullTurnSteps / 2;
    }

  Serial.print(fullTurnSteps);
  Serial.println(F(" saved steps per revolution"));

  // If step count explicitly defined, use that
  #ifdef FULL_STEP_COUNT
    fullTurnSteps = FULL_STEP_COUNT;  
  #endif

  if (fullTurnSteps == 0)
  {
    fullTurnSteps = FULL_TURN_STEPS;
    // setTrackDefaults();
    // setServoDefaults();
  }
  halfTurnSteps = fullTurnSteps / 2;  

  // if (!calibrating ) {
  //   calibrating = true;
  //   calibrationPhase = 3;
  //   stepper.enableOutputs();
  //   stepper.moveTo(fullTurnSteps);
  //   lastTarget = fullTurnSteps;
  //   Serial.print(calibrating);
  //   Serial.print(F(" calibrating   "));
  //   Serial.print(fullTurnSteps);
  //   Serial.println(F(" steps per revolution"));
  // }

// #ifndef SENSOR_TESTING  // If we're not sensor testing, start Wire()
  // setupWire();
// #endif
  
  displayConfig();  // Display Turntable configuration
  notice(" ");
  notice("Display configured");
  
  setupStepperDriver();   // Set up the stepper driver
  notice("Stepper set up");
  lastRunningState = isStepperRunning();
  
  // setupServos();

	notice("Turntable Program Started");
	
  setupComplete = true;
  Serial.println(F("Setup zero complete"));
  while(!setup1Complete);
  delay(10);

  startChecking();
  if (!stepsSet) {
    //  drawSettingsPage();
    // drawConfigPage();
    drawDiagnosticPage();
  }
  else {
    drawHomePage();
    drawTracks();
    homed = 0;
    initiateHoming();
    // below was commented out
    calibrating = true;
    calibrationPhase = 1;
    moveHome();

    // Serial.println(F("Setup initial homing..."));
    // stepper.enableOutputs();
    // stepper.moveTo(sanitySteps);
    // lastTarget = sanitySteps;
  }  
  Serial.println("Loop 1 started \n");
}

// ==== Loop One for LCC processes ==========================
void loop() {

  if (Serial.available()) {

    switch (Serial.read()) {
      case 'h':
        Serial.println("'h': Help");
        Serial.println("'c': Setting NVM to 0x00");
        Serial.println("'i': Resetting NVM to CDI default values");
        Serial.println("'p': Toggle Message Logging");
        Serial.println("'r': Resetting NVM to 0xFF for a fresh boot");
        Serial.println("'m': Toggle Config Mem read/write Logging");
        Serial.println("'t': Display current time");
        Serial.println("'q': Query current time");
      break;
      case 'c':
        Serial.println("Setting NVM to 0x00...");
        ConfigMemHelper_clear_config_mem();  // reset all EEPROM to initalized nulls 0x00
        Serial.println("Setting NVM to 0x00 COMPLETE");
      break;
      case 'i':
        Serial.println("Resetting NVM to default values...");
        ConfigMemHelper_reset_and_write_default(OpenLcbUserConfig_node_id);  // reset all EEPROM to CDI defined defaults
        Serial.println("Resetting NVM to default values COMPLETE");
      break;
      case 'r':
        Serial.println("Setting NVM to 0xFF (factory fresh configuration...)");
        ConfigMemHelper_reset_config_mem();  // reset all EEPROM to factory new 0xFF
        Serial.println("Setting NVM to 0xFF (factory fresh configuration) COMPLETE");
      break;
      case 'p':
        Serial.print("Toggling Message Logging...)");
        Serial.println();
        Callbacks_toggle_log_messages(); 
      break;
      case 'm':
        Serial.print("Toggling Config Mem Logging...");
        Serial.println();
        if (ConfigMemHelper_toggle_log_access()) {
          Serial.println("Logging Access");
        } else {
          Serial.println("Not Logging Access");
        }
      break;
      // case 'q':
      //   OpenLcbApplicationBroadcastTime_send_query(OpenLcbUserConfig_node_id, BROADCAST_TIME_ID_DEFAULT_FAST_CLOCK);
      // break;
      // case 't':    
      //   broadcast_clock_state_t* clock = OpenLcbApplicationBroadcastTime_get_clock(BROADCAST_TIME_ID_DEFAULT_FAST_CLOCK);
      //   // clock->is_running = true;
      //   printf("Current time: %02d:%02d\n", clock->time.hour, clock->time.minute);  
      // break;
      case 'x':      
       Load_application_defaults(OpenLcbUserConfig_node_id);
      break;
    };  
  }

  // // put your main code here, to run repeatedly
  RPiPicoCanDriver_process_receive();

  OpenLcb_run();
  // ProcessPixels();
  
  touchIO();    // process touch input
  
}

// ==== Loop Two for node function processes ==========================
void loop1() {
  static long nextdot = 0;
  if(millis()>nextdot) {
    nextdot = millis()+2000;
    //dP("\n.");
  }
    
  // ResetStrings(); // ResetDirty()
  // ProcessPixels();
  
    
    if (calibrating) calibration();    // If flag is set for calibrating, do it.
    else
    { 
      if (homed == 0)  moveHome();      // If we haven't successfully homed yet, do it.
      else
      {
        if (!isStepperRunning() && (getCurrentPosition() == 0)) MoveToTrack(homeTrack,ConfigMemHelper_config_data.BridgeOrientation);
        positionCheck();
      }
    }  


    runStepper();    // Process the stepper object continuously.


    // processLED();   // Process our LED.

    #if defined(DISABLE_OUTPUTS_IDLE)   // If disabling on idle is enabled, disable the stepper.
      if (isStepperRunning() != lastRunningState) {
        lastRunningState = isStepperRunning();
        if (!lastRunningState) { 
          disableStepper();
          if (activeScreen == 1) drawBridge(absPosition(getCurrentPosition())*360/fullTurnSteps); 
          }
      }
    #endif

  // touchIO();    // process touch input
  
    // processSerialInput();   // Receive and process and serial input for test commands.

  // #endif 

}