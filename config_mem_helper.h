#ifndef CONFIG_MEM_HELPER_H
#define CONFIG_MEM_HELPER_H

#include <stdint.h>
#include <stdbool.h>
#include "src/openlcb/openlcb_types.h"

/**
 * Configuration Memory Helper Functions
 * Provides utilities for reading/writing configuration memory
 */

// ===== Configuration Memory Structure =====
// Auto-generated from CDI XML via C Struct Generation Panel

// Auto-generated from CDI XML
// OpenLCB Configuration Memory Structure

#include <stdint.h>

#include "BoardSettings.h"
#include "TTvariables.h"

#pragma pack(push, 1)  // Byte-aligned structures

// Memory space 253, origin 0x00
#define CONFIG_START_ADDR 0x00
// Memory space 253, origin 0x00
#define NODE_ADDR 0x00
// Memory space 253, origin 0x7D (125)
#define RESET_CONTROL_ADDR 0x7D
// Memory space 253, origin 0x7E (126)
#define ATTRIBUTES_ADDR 0x7E

typedef struct{
  // Memory space 253, origin NODE_ADDR
  struct {           
    char node_name[62];
    char node_description[63];
  } nodeid;
   // Memory space 253, origin RESET_CONTROL_ADDR
  struct {                             
    uint8_t flag;
  } reset_control;
  // Memory space 253, origin ATTRIBUTES_ADDR
  struct {    
// Track parameters
    uint8_t TrackCount;    // int8 Number of Tracks off turntable
    uint8_t HomeTrack;    // int8 initial track location after homing
    uint8_t ReferenceCount;   // number of reference locations for correction, if false then no reference locations and correction is disabled, if true then the number of reference locations is equal to the number of tracks and they are located at the same position as the tracks
    int32_t FullTurnSteps;   // number of steps in a full turn of the turntable
  
    event_id_t Rehome;     // consumer eventID
    event_id_t IncrementTrack;    // consumer eventID
    event_id_t DecrementTrack;  // consumer eventID
    event_id_t RotateTrack180 ; // consumer eventID
    event_id_t ToggleBridgeLights;  // consumer eventID
    struct {
      char trackName[25];        // description of this Track
      char trackShort[5];        // short description of this Track
      event_id_t Front;       // consumer eventID
      event_id_t Back;       // consumer eventID
      int32_t steps;       // position
      event_id_t Occupancy;       // consumer eventID
      event_id_t RailCom;       // consumer eventID
    } tracks[MAX_TRACKS];
// Door parameters
    uint8_t DoorCount;    // int8 Number of Doors off turntable tracks
    event_id_t OpenAll;      // producer eventID
    event_id_t CloseAll;      // producer eventID
    struct {
      char doorName[16];        // description of this Door
      char doorShort[5];        // short description of this Door
      event_id_t eidToggle;       // producer Toggle door position eventID
      uint8_t TrackLocation;    // int8 number of the track where the door is located
    } doors[MAX_DOORS];
// Lights parameters
    event_id_t eidBridge;       // consumer Toggle Bridge Lights eventID
    event_id_t eidInterior;       // producer Toggle Interior Lights eventID
    event_id_t eidExterior;       // producer Toggle Exterior Lights eventID
    uint8_t HighLuminosity;    // int8 factor on brightness when dimmer is off
    event_id_t eidHighLuminosity_On;       // consumer turn Group 0 ON eventID
    uint8_t LowLuminosity;    // int8 factor on brightness when dimmer is on
    event_id_t eidLowLuminosity_On;       // consumer turn Group 0 ON eventID
  } attributes;
  
// modify as desired
// data not used by CDI 
  event_status_enum consumer_status[5+4*MAX_TRACKS+3]; // Array to hold the state of each event (on/off/unknown) for the 42 events defined in the configuration
  event_status_enum producer_status[2+MAX_DOORS+2]; // Array to hold the state of each event (on/off/unknown) for the events defined in the configuration
  TrackAddress Tracks[MAX_TRACKS];
  ReferenceStep References[NumberOfReferences];
  LightAddress Lights[NumOfLights];
  uint8_t CurrentTrack; // current track location
  uint8_t BridgeOrientation; // current bridge orientation
  uint8_t MemVersion;
  // uint8_t curpos[NUM_SERVOS];
} config_mem_t;

#pragma pack(pop)


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// Global configuration structure instance
extern config_mem_t ConfigMemHelper_config_data;

// ===== Configuration Memory Access Functions =====

// Reads the current NVM into the passed structure, the two are in synce when it returns.
extern bool ConfigMemHelper_read(openlcb_node_t *openlcb_node, config_mem_t *config);
// Writes the current passed structure in the NVM, the two are in synce when it returns.
extern bool ConfigMemHelper_write(openlcb_node_t *openlcb_node, config_mem_t *config);
// Loads the default values for the NMV into the structure and writes them to the NVM, the two are in synce when it returns.
extern bool ConfigMemHelper_reset_and_write_default(openlcb_node_t *openlcb_node);

// Sets all Configuration Memory to the default 0xFF that a newly programmed device would have
extern void ConfigMemHelper_reset_config_mem(void);
// Sets all Configuration Memory to 0x00 which is a valid "cleared memory"
extern void ConfigMemHelper_clear_config_mem(void);
// Tests if the first byte of the Configuration memory is 0xFF, if so then the memory not initialized (cleared 0x00 is initialized as byte 0 is the User Name string which initialized is a null string)
extern bool ConfigMemHelper_is_config_mem_reset(void);

// Hooks for the OpenLcbLib that allows snooping on Config Mem writes before passing them on to the Pico Drivers to write to NVM
extern uint16_t ConfigMemHelper_config_mem_write(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer);
// Hooks for the OpenLcbLib that allows snooping on Config Mem reads before passing them on to the Pico Drivers to read/ite to NVM
extern uint16_t ConfigMemHelper_config_mem_read(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer);

extern bool ConfigMemHelper_toggle_log_access(void);

void Load_application_defaults(openlcb_node_t *openlcb_node);

long getSteps();
void writeSteps(long steps);
void getTrack();
void writeTrack(uint8_t i,uint8_t Direction);
void writeCount();
void getCount();
void writeTracks();
void getTracks();
void writeReferences();
void getReferences();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // CONFIG_MEM_HELPER_H
