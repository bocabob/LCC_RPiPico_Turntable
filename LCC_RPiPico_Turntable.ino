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
 * This sketch creates an OpenLcb Turntable controller node running on a
 * Raspberry Pi Pico 2 with an MCP2518 hardware CAN controller.
 *
 * @author Bob Gamble, David Harris, Alex Shepherd, and Jim Kueneman
 * @date 23 Feb 2025
 */
/*
 * LCC Stepper Motor Turntable Controller
 *
 * Original stepper work: Alex Shepherd 2017-12-04
 * Modified for LocoNet: Bob Gamble 2022-01-21
 * EX-Turntable code: Peter Cole Oct 2023
 * Rewritten for LCC (OpenLCB): Bob Gamble 2025-01-10
 * Modified: DPH 2024, RSG 2025
 * Copyright 2019 Alex Shepherd, David Harris, and Bob Gamble
 *
 * This program uses a Raspberry Pi Pico 2 with:
 *   - MCP2518 hardware CAN controller (LCC Shield)
 *   - A4988 stepper motor driver module (Step/Dir/Enable)
 *   - Hall sensor(s) for home and bridge indexing
 *   - 800x480 parallel Touch TFT for control and status display
 *   - NeoPixel for bridge/accessory lighting
 *   - I2C EEPROM (24LC512) for persistent configuration
 *
 * The local AccelStepper copy in src/application_drivers/ contains
 * modifications — do not replace with the stock Arduino library version.
 *
 * Pin assignments are in BoardSettings.h / board_configs/BoardPins_*.h.
 */

// Board and display driver selection is in ProjectConfig.h — edit that file
// to switch boards or display drivers.  Defines set here are only visible to
// the .ino translation unit; ProjectConfig.h propagates them to all .cpp files.

#include "Arduino.h"
#include <Wire.h>
#include <LibPrintf.h>

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
// Display driver selected by board config — included via DisplayDriver.h / UserInterface.h
// Include local files
#include "Turntable.h"
#include "src/application_drivers/AccelStepper.h"
#include "UserInterface.h"

#define NODE_ID 0x050101019419      // 05 01 01 01 94 ** range assigned to Bob Gamble / Southern Piedmont


////// DECLARATIONS

extern bool lastRunningState;   // Stores last running state to allow turning the stepper off after moves.

extern AccelStepper stepper;
extern bool isStepperRunning();
extern long getCurrentPosition();
extern void runStepper();
extern void disableStepper();

bool setupComplete = false;
bool StorageReady = false;

bool stepsSet;

npStrings _Strings[MAX_STRINGS];

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
    .firmware_write                  = &Callbacks_write_firmware,
    .on_100ms_timer                  = &Callbacks_on_100ms_timer_callback,
    .on_login_complete               = &Callbacks_on_login_complete,
    .on_consumed_event_identified    = &Callbacks_on_consumed_event_identified,
    .on_consumed_event_pcer          = &Callbacks_on_consumed_event_pcer,
    .on_broadcast_time_changed       = &Callbacks_on_broadcast_time_time_changed,
};

void _check_for_nvm_initialization(void) {

  Serial.println("Checking for initialized NVM");

  if (!ConfigMemHelper_nvm_is_accessible()) {
    Serial.println("FATAL: NVM not accessible - check I2C wiring, address (0x50), and pullups on SDA/SCL");
    // while (true) { delay(1000); }  // halt
  }

  // If the first byte of the configuration memory is 0xFF then the space has never been accessed (fresh firmware load) and need to be initialized to 0x00
  if (ConfigMemHelper_is_config_mem_reset()) {
   
    Serial.println("Initializing Configuration Memory to 0x00");
    ConfigMemHelper_clear_config_mem();
    Serial.println("Writing default values to the Configuration Memory");
    ConfigMemHelper_reset_and_write_default(OpenLcbUserConfig_node_id);
    Serial.println("Defaults set...");

  } else {

    Serial.println("Configuration Memory has been previously initialized");

  }
}

void _register_producers(void) {
  int index = 0;
  OpenLcbApplication_clear_producer_eventids(OpenLcbUserConfig_node_id);

  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.OpenAll), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  index++;
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.CloseAll), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  index++;
  for (int i = 0; i < ConfigMemHelper_config_data.attributes.DoorCount; i++) {
     OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.doors[i].eidToggle), ConfigMemHelper_config_data.producer_status[index+i]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  }
  index += ConfigMemHelper_config_data.attributes.DoorCount;
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidInterior), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  OpenLcbApplication_register_producer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidExterior), ConfigMemHelper_config_data.producer_status[index+1]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID 
}

bool produceLightIn()
{
  if (OpenLcbApplication_send_event_pc_report(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidInterior))) {
  // Toggle interior lights
  ToggleProducerEventStatus(ConfigMemHelper_config_data.attributes.DoorCount+2);
  return true;
  }
  return false;
}
bool produceLightEx()
{
  if (OpenLcbApplication_send_event_pc_report(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidExterior))) {
  // Toggle exterior lights
  ToggleProducerEventStatus(ConfigMemHelper_config_data.attributes.DoorCount+3);
  return true;
  }
  return false;
}
bool produceOpenAll()
{
  if (OpenLcbApplication_send_event_pc_report(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.OpenAll))) {
  // open all doors — SET means open (matches Roundhouse SetServoStatus() convention)
  for (int i = 0; i < ConfigMemHelper_config_data.attributes.DoorCount; i++) {
  ConfigMemHelper_config_data.producer_status[2+i] = EVENT_STATUS_SET;
  }
  return true;
  }
  return false;
}
bool produceCloseAll()
{
  if (OpenLcbApplication_send_event_pc_report(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.CloseAll))) {
  // close all doors — CLEAR means closed (matches Roundhouse SetServoStatus() convention)
  for (int i = 0; i < ConfigMemHelper_config_data.attributes.DoorCount; i++) {
  ConfigMemHelper_config_data.producer_status[2+i] = EVENT_STATUS_CLEAR;
  }
  return true;
  }
  return false;
}

bool produceDoor(int servo)
{
  if (OpenLcbApplication_send_event_pc_report(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.doors[servo].eidToggle))) {
  // Optimistically toggle the local producer_status before the Roundhouse confirms
  // the move.  If the CAN message is lost or the servo fails, the Turntable display
  // will diverge from the actual door state.  The Roundhouse's "Producer Identified"
  // broadcasts at the next LCC login are the recovery mechanism that re-syncs the
  // display to ground truth.
  ToggleProducerEventStatus(2 + servo);

  return true;
  }
  return false;
}

void ToggleProducerEventStatus(int eventIndex)
{
  switch (ConfigMemHelper_config_data.producer_status[eventIndex]) {   // need to read the state from the NVM to know if it is on/off/unknown when producing the producer event ID

    case EVENT_STATUS_UNKNOWN:
      ConfigMemHelper_config_data.producer_status[eventIndex] = EVENT_STATUS_SET;  // default to setting the event when it is unknown and the button is pushed, but could be set to clear if desired
      break;

    case EVENT_STATUS_SET:
      ConfigMemHelper_config_data.producer_status[eventIndex] = EVENT_STATUS_CLEAR;  // clear if it is currently set so that the next time the event is produced it will be clear
      break;

    case EVENT_STATUS_CLEAR:
      ConfigMemHelper_config_data.producer_status[eventIndex] = EVENT_STATUS_SET;  // set if it is currently clear so that the next time the event is produced it will be set
      break;
  }
  // Mark the RAM config as dirty so the 100ms timer will flush to EEPROM within
  // ~3 seconds.  Writing directly here blocks Core 0 for 200-500ms per call,
  // which stalls CAN bus processing and can corrupt the I2C bus under load.
  _config_dirty = true;
}

void _register_consumers(void) {
  int index = 0;
  OpenLcbApplication_clear_consumer_eventids(OpenLcbUserConfig_node_id);

  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.Rehome), ConfigMemHelper_config_data.consumer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.IncrementTrack), ConfigMemHelper_config_data.consumer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.DecrementTrack), ConfigMemHelper_config_data.consumer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.RotateTrack180), ConfigMemHelper_config_data.consumer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.ToggleBridgeLights), ConfigMemHelper_config_data.consumer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
  index++;


  // Register the bridge track (track 0) RailCom detector event range if configured.
  // Track 0 has no LCC command events (homing is handled by the Rehome consumer above),
  // but it may have a DCC detector on the bridge itself.
#ifdef OPENLCB_COMPILE_DCC_DETECTOR
  if (ConfigMemHelper_config_data.attributes.tracks[0].RailCom != 0) {
    OpenLcbApplication_register_consumer_range(OpenLcbUserConfig_node_id,
        swap_endian64(ConfigMemHelper_config_data.attributes.tracks[0].RailCom),
        EVENT_RANGE_COUNT_32768);
  }
#endif

  // Register consumer events for usable tracks 1 through TrackCount.
  // Track 0 is the homing-sensor position; it has no LCC command events — homing
  // is handled by the dedicated Rehome consumer registered above.
  // The loop offset (i-1) maps track i into the zero-based consumer_status slot so
  // the contiguous consumer_status[] block is fully packed with no gap at slot 0.
  for (int i = 1; i <= ConfigMemHelper_config_data.attributes.TrackCount; i++) {
    int slot = i - 1;  // zero-based slot within the track block of consumer_status[]
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Front),    ConfigMemHelper_config_data.consumer_status[index + slot*3]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the consumer event ID
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Back),     ConfigMemHelper_config_data.consumer_status[index + slot*3 + 1]);
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].Occupancy), ConfigMemHelper_config_data.consumer_status[index + slot*3 + 2]);
    // need to check if the RailCom event ID is non-zero before registering it, since it is optional and may not be set for all tracks.  If it is zero then skip registration since zero is not a valid event ID and the OpenLCB stack will reject attempts to register it, which causes the rest of the registration calls to be ignored and the expected consumer events will not be registered.
    if (ConfigMemHelper_config_data.attributes.tracks[i].RailCom != 0) {
      OpenLcbApplication_register_consumer_range(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.tracks[i].RailCom), EVENT_RANGE_COUNT_32768);  // register the RailCom event ID as a consumer range to allow for future expansion with additional RailCom-related events if needed without requiring a firmware update to add new consumer event ID registration calls
      //extern bool OpenLcbApplication_register_consumer_range(openlcb_node_t *openlcb_node, event_id_t event_id_base, event_range_count_enum range_size);
    }
  }
  index += ConfigMemHelper_config_data.attributes.TrackCount*3;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidBridge), ConfigMemHelper_config_data.consumer_status[index]);
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidHighLuminosity_On), ConfigMemHelper_config_data.consumer_status[index]);
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidLowLuminosity_On), ConfigMemHelper_config_data.consumer_status[index]);
  index++;

  // Register the Roundhouse door ToggleDoor event IDs as consumers on the Turntable.
  // This allows the Turntable to receive "Consumer Identified" messages from the Roundhouse
  // during LCB login, carrying the NVM-restored open/closed state for each door, so the
  // Turntable display is synchronised at startup without any user interaction.
  for (int i = 0; i < ConfigMemHelper_config_data.attributes.DoorCount; i++) {
    OpenLcbApplication_register_consumer_eventid(
        OpenLcbUserConfig_node_id,
        swap_endian64(ConfigMemHelper_config_data.attributes.doors[i].eidToggle),
        ConfigMemHelper_config_data.consumer_status[index++]);
  }

  // uncomment the following if you want to register consumer event IDs for the producer events as well, but it is not needed unless you want to be able to consume your own producer events (e.g. for testing or if you have a use case for it in your application)
  /*
  index = 0;
    OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.OpenAll), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.CloseAll), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  index++;
  for (int i = 0; i < ConfigMemHelper_config_data.attributes.DoorCount; i++) {
     OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.doors[i].eidToggle), ConfigMemHelper_config_data.producer_status[index+i]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  }
  index += ConfigMemHelper_config_data.attributes.DoorCount;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidInterior), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID
  index++;
  OpenLcbApplication_register_consumer_eventid(OpenLcbUserConfig_node_id, swap_endian64(ConfigMemHelper_config_data.attributes.eidExterior), ConfigMemHelper_config_data.producer_status[index]);  // need to read the state from the NVM to know if it is on/off/unknown when registering the producer event ID 
  */
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
  OpenLcbConfig_initialize(&openlcb_config);

  Callbacks_initialize();

  Serial.println("Creating Node.....");

  OpenLcbUserConfig_node_id = OpenLcbConfig_create_node(NODE_ID, &OpenLcbUserConfig_node_parameters);
  // do this after initialization or the I2C will not be initialized

  _check_for_nvm_initialization();

  // Read the NVM into the local data structures
  Serial.println("Loading NVM values into Config Mem data variable");
  ConfigMemHelper_read(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
  Serial.println("Data variable loaded and ready for use");
  // TODO: need to load track variables and door variables into the local data structures as well for use in the program, or read from the NVM when needed.  The latter is simpler but less efficient.  The former is more efficient but more complex to implement and maintain.  For now, we will read from the NVM when needed.
  
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

  // set defaults
  fullTurnSteps = FULL_TURN_STEPS;
  halfTurnSteps = fullTurnSteps / 2;
  
  Serial.print(fullTurnSteps);
  Serial.println(F(" default steps per revolution"));  

  // check EEPROM for saved values and initialize
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
  }
  halfTurnSteps = fullTurnSteps / 2;  

  Set_Application_Values_From_Config(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
  displayConfig();  // Display Turntable configuration
  notice(" ");
  notice("Display configured");
  
  setupStepperDriver();   // Set up the stepper driver
  notice("Stepper set up");
  lastRunningState = isStepperRunning();
  
	notice("Turntable Program Started");
	
  setupComplete = true;
  Serial.println(F("Setup zero complete"));
  // Note: setup1Complete was removed — it was declared false and never set true,
  // making while(!setup1Complete) an infinite loop that prevented Core 1 from
  // ever reaching startChecking() or loop1().
  startChecking();
  if (!stepsSet) {
    //  drawSettingsPage();
    // drawConfigPage();
    drawDiagnosticPage();
  }
  else {
    drawHomePage();
    // drawTracks() is now called inside drawHomePage() for TFT_eSPI modes (under
    // the _display_page_drawing guard) to prevent a two-core SPI1 race between
    // Core 1 (here in setup1) and Core 0's updateBridgeAnimation().  No separate
    // drawTracks() call is needed here.
    homed = 0;
    initiateHoming();
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
      case 'x':      
       Load_application_defaults(OpenLcbUserConfig_node_id);
      break;
      case 'z':    
       Set_Application_Values_From_Config(OpenLcbUserConfig_node_id, &ConfigMemHelper_config_data);
      break;
    };  
  }

  RPiPicoCanDriver_process_receive();

  OpenLcbConfig_run();

  touchIO();    // process touch input

  // Redraw bridge whenever the stepper has moved ≥ 1° since the last frame.
  // For RA8876_NATIVE this only touches Layer 1; tracks on Layer 0 are untouched.
  // For TFT_eSPI modes the function also calls drawTracks() to restore track lines
  // erased by the fillCircle erase in drawBridge().
  updateBridgeAnimation();

  // Drain all display dirty flags set by event callbacks (door state, track
  // occupancy, RailCom addresses, bridge power state, etc.).  Runs after
  // updateBridgeAnimation() so a simultaneous bridge + track update on
  // TFT_eSPI modes is handled by the bridge path covering both in one pass.
  updateDisplay();

}

// ==== Loop Two for node function processes ==========================
void loop1() {
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

    #if defined(DISABLE_OUTPUTS_IDLE)   // If disabling on idle is enabled, disable the stepper.
      if (isStepperRunning() != lastRunningState) {
        lastRunningState = isStepperRunning();
        if (!lastRunningState) {
          disableStepper();
          // NOTE: do NOT call drawBridge() here — this is Core 1 and the display
          // SPI bus is owned by Core 0.  Calling drawBridge() from loop1() causes
          // a two-core SPI collision (confirmed via serial bisection).
          // updateBridgeAnimation() on Core 0 redraws the bridge at the stopped
          // position within one loop() iteration, so no separate call is needed.
        }
      }
    #endif

}