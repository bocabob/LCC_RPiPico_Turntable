#include <sys/_stdint.h>
/**
 * Configuration Memory Helper Implementation
 * Auto-generated from OpenLCB configuration
 *
 * Uses chunked memory read/write approach:
 * - OpenLCB datagrams limited to 64 bytes max payload
 * - Large structs read/written in 64-byte chunks
 * - All segments in address space 0xFD (Configuration)
 */

#include "Arduino.h"
#include <LibPrintf.h>
#include "config_mem_helper.h"
#include "openlcb_user_config.h"
#include "src/openlcb/openlcb_application.h"
#include "src/pico/rpi_pico_drivers.h"
#include "src/utilities/mustangpeak_endian_helper.h"
#include <string.h>
#include <stdio.h>

#include "TTvariables.h"


static bool _direct_access = false;

config_mem_t ConfigMemHelper_config_data;
bool ConfigMemHelper_log_access = false;
extern long absPosition(long position);

extern bool stepsSet;

bool ConfigMemHelper_toggle_log_access(void) {

  ConfigMemHelper_log_access = !ConfigMemHelper_log_access;

  return ConfigMemHelper_log_access;

}

static void _load_defaults_node(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {

  const char *name_def = "Southern Piedmont";
  strncpy(config->nodeid.node_name, name_def, sizeof(config->nodeid.node_name)); // Will pad with nulls
  const char *descript_def = "Turntable Controller Node";
  strncpy(config->nodeid.node_description, descript_def, sizeof(config->nodeid.node_description)); // Will pad with nulls

}

static void _load_defaults_reset_control(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {

  config->reset_control.flag = 238;
  
}

static void _load_defaults_attributes(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {

  config->attributes.TrackCount = NUM_TRACKS;
  config->attributes.HomeTrack = 3; // Default home track is track 4 (0 indexed)
  config->attributes.ReferenceCount = false; // Disable reference correction by default
  config->attributes.FullTurnSteps = swap_endian32(FULL_TURN_STEPS); // number of steps in a full turn of the turntable, needs to be in little endian for the OpenLcbLib functions to write it correctly to memory and have it be correct when read back on a big endian system
  config->attributes.Rehome = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++;// EventID for rehome
  config->attributes.IncrementTrack = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for increment track
  config->attributes.DecrementTrack = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for decrement track
  config->attributes.RotateTrack180 = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for rotate track 180
  config->attributes.ToggleBridgeLights = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for toggle bridge lights
  for (int t = 0; t < MAX_TRACKS; t++) {
    strncpy(config->attributes.tracks[t].trackName, TrackName[t], sizeof(config->attributes.tracks[t].trackName));
    strncpy(config->attributes.tracks[t].trackShort, TrackTag[t], sizeof(config->attributes.tracks[t].trackShort));
    // config->attributes.tracks[t].trackShort = TrackTag[t];
    config->attributes.tracks[t].Front = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track front
    config->attributes.tracks[t].Back = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track back
    config->attributes.tracks[t].Occupancy = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track occupancy
    config->attributes.tracks[t].RailCom = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track railcom
    config->attributes.tracks[t].steps = 0; // Default position in steps for each track
  }
    config->attributes.tracks[1].steps = swap_endian32(absPosition(entryTrack1)); // Default position in steps for each track
    config->attributes.tracks[2].steps = swap_endian32(absPosition(entryTrack2)); // Default position in steps for each track
    config->attributes.tracks[3].steps = swap_endian32(absPosition(entryTrack3)); // Default position in steps for each track
    config->attributes.tracks[4].steps = swap_endian32(absPosition(houseTrack1)); // Default position in steps for each track
    config->attributes.tracks[5].steps = swap_endian32(absPosition(houseTrack2)); // Default position in steps for each track
    config->attributes.tracks[6].steps = swap_endian32(absPosition(houseTrack3)); // Default position in steps for each track
    config->attributes.tracks[7].steps = swap_endian32(absPosition(houseTrack4)); // Default position in steps for each track
    config->attributes.tracks[8].steps = swap_endian32(absPosition(houseTrack5)); // Default position in steps for each track
    config->attributes.tracks[9].steps = swap_endian32(absPosition(houseTrack6)); // Default position in steps for each track
    config->attributes.tracks[10].steps = swap_endian32(absPosition(houseTrack7)); // Default position in steps for each track
    config->attributes.tracks[11].steps = swap_endian32(absPosition(houseTrack8)); // Default position in steps for each track
    config->attributes.tracks[12].steps = swap_endian32(absPosition(houseTrack9)); // Default position in steps for each track
    config->attributes.tracks[13].steps = swap_endian32(absPosition(houseTrack10)); // Default position in steps for each track
    config->attributes.tracks[14].steps = swap_endian32(absPosition(houseTrack11)); //

  config->attributes.DoorCount = NUM_DOORS;
  config->attributes.OpenAll = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for open all doors
  config->attributes.CloseAll = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for close all doors
  const char *door_name = "Door";
  const char *door_tag = "D";
  for (int d = 0; d < MAX_DOORS; d++) {
    strncpy(config->attributes.doors[d].doorName, door_name, sizeof(config->attributes.doors[d].doorName));
    // config->attributes.doors[d].doorName = "Door";
    // config->attributes.doors[d].doorShort[0] = "D";
    strncpy(config->attributes.doors[d].doorShort, door_tag, sizeof(config->attributes.doors[d].doorShort));
    config->attributes.doors[d].eidToggle = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for toggle door
    config->attributes.doors[d].TrackLocation = 5 + d; // Default track location for each door
  }
  config->attributes.eidBridge = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for toggle bridge lights
  config->attributes.eidInterior = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for toggle interior lights
  config->attributes.eidExterior = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for toggle exterior lights
  config->attributes.HighLuminosity = MAX_LUMINANCE; // Max brightness when dimmer is off
  config->attributes.eidHighLuminosity_On = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for high luminosity on
  config->attributes.LowLuminosity = DIM_LUMINANCE; // Default brightness when dimmer is on
  config->attributes.eidLowLuminosity_On = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++;  // EventID for low luminosity on
 
}

static void _load_defaults_status(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {
/*

  uint8_t event_state[2+12+28]; // Array to hold the state of each event (on/off/unknown) for the 42 events defined in the configuration

*/
  for (int i = 0; i < (5+4*MAX_TRACKS+3); i++) {
    config->consumer_status[i] = EVENT_STATUS_UNKNOWN; // Default event state is unknown (0)
  }

  for (int i = 0; i < (2+MAX_DOORS+2); i++) {
    config->producer_status[i] = EVENT_STATUS_UNKNOWN; // Default event state is unknown (0)
  }

}
static void _load_defaults_application(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {
/*
  TrackAddress Tracks[MAX_TRACKS];
  ReferenceStep References[NumberOfReferences];
  LightAddress Lights[NumOfLights];
  uint8_t CurrentTrack; // current track location
  uint8_t BridgeOrientation; // current bridge orientation
  uint8_t MemVersion;

  
	int address;
	long trackFront;
	long trackBack;
  bool doorPresent;
  int servoNumber;
*/
    // setTrackDefaults();
    
  for (int i = 0; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)); i++) {
      ConfigMemHelper_config_data.Tracks[i].address = 500 + i;
      ConfigMemHelper_config_data.Tracks[i].trackFront = 0;
      ConfigMemHelper_config_data.Tracks[i].trackBack = (FULL_TURN_STEPS / 2);
      ConfigMemHelper_config_data.Tracks[i].doorPresent = false;
      ConfigMemHelper_config_data.Tracks[i].servoNumber = 0;
    }
  for (int i = 0; i < config->attributes.TrackCount; i++) {
    config->Tracks[i].address = 500+i; // Default address
    config->Tracks[i].trackFront = swap_endian32(config->attributes.tracks[i].steps); // Default address
    config->Tracks[i].trackBack = config->Tracks[i].trackFront + (swap_endian32(config->attributes.FullTurnSteps) / 2); // Default address
    config->Tracks[i].doorPresent = false; // Default address
    config->Tracks[i].servoNumber = i; // Default address
  }

  for (int i = 4; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)-1); i++) {
      ConfigMemHelper_config_data.Tracks[i].doorPresent = true;
      ConfigMemHelper_config_data.Tracks[i].servoNumber = (i-4) % MAX_DOORS;
    }
  ConfigMemHelper_config_data.attributes.TrackCount = NUM_TRACKS;
  
  homeTrack = 3;

return;

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

uint16_t ConfigMemHelper_config_mem_write(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer) {

  // Hook into the Configuration Memory Write to update the data structures in parallel

  // Are we in the internal process of syncing the NMV with the struct?  If so just write to the NVM as we are syncing them.
  if (_direct_access) {  

    delay(10);

    return RPiPicoDrivers_config_mem_write(openlcb_node, address, count, buffer);

  }

  // This is a call from a Configuration Memory Protocol message from an external node or configuration tool so keep the NVM and data structure in sync
  
  if (ConfigMemHelper_log_access) {
    Serial.print("ConfigMemHelper_config_mem_write - Writing Address: ");
    Serial.print(address);
    Serial.print(", count: ");
    Serial.println(count);
  }
  
  // First write the value to the RAM structure
  uint8_t *byte_array = (uint8_t*) &ConfigMemHelper_config_data;
  byte_array += address;
  memcpy(byte_array, buffer, count);

  // Now write to the NVM
  return RPiPicoDrivers_config_mem_write(openlcb_node, address, count, buffer);

}

uint16_t ConfigMemHelper_config_mem_read(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer) {

  // Hook into the Configuration Memory Read to update the datastructures in parallel


  // Are we in the internal process of syncing the NMV with the struct?  If so just write to the NVM as we are syncing them.
  if (_direct_access) {

    delay(10);

    return RPiPicoDrivers_config_mem_read(openlcb_node, address, count, buffer);

  }

  // This is a call from a Configuration Memory Protocol message from an external node or configuration tool so the data structures should be in sync with NVM so just read what is in the buffers.

  if (ConfigMemHelper_log_access) {
    Serial.print("ConfigMemHelper_config_mem_read - Reading Address: ");
    Serial.print(address);
    Serial.print(", count: ");
    Serial.println(count);
  }

  uint8_t *byte_array = (uint8_t*) &ConfigMemHelper_config_data;
  byte_array += address;
  memcpy(buffer, byte_array, count);

  // Now read from the NVM
  // return RPiPicoDrivers_config_mem_read(openlcb_node, address, count, buffer);

  return count;

}

void Load_application_defaults(openlcb_node_t *openlcb_node){
  uint16_t consumer_index = 0;
  uint16_t producer_index = 0;
  
  _load_defaults_application(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);

}

bool ConfigMemHelper_reset_and_write_default(openlcb_node_t *openlcb_node) {

  uint16_t consumer_index = 0;
  uint16_t producer_index = 0;

  // Just write this to the NVM don't try to keep the RAM buffer in sync, it is as we want it and just want that image in NVM
  _direct_access = true;

  _load_defaults_node(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);
  _load_defaults_reset_control(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);
  _load_defaults_attributes(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);
  _load_defaults_status(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);
  _load_defaults_application(openlcb_node, &ConfigMemHelper_config_data, &consumer_index, &producer_index);

  if (!ConfigMemHelper_write(openlcb_node, &ConfigMemHelper_config_data)) {

    Serial.println("Failed to write to ConfigMemHelper_write_node");
    _direct_access = false;
    return false;

  }

  _direct_access = false;
  return true;
}

/**
 * Read Config Mem from configuration memory
 *
 * Address: CONFIG_START_ADDR
 * Space: 0xFD (Configuration)
 * Type: config_mem_t
 */
bool ConfigMemHelper_read(openlcb_node_t *openlcb_node, config_mem_t *config) {
  if (!openlcb_node || !config) {
    return false;
  }

  uint32_t address = CONFIG_START_ADDR;  // Starting address from #define
  uint32_t total_size = sizeof(config_mem_t);
  uint32_t bytes_remaining = total_size;
  uint8_t *dest = (uint8_t *)config;

  configuration_memory_buffer_t temp_buffer;

  _direct_access = true;

  // Read in chunks (max 64 bytes per datagram)
  while (bytes_remaining > 0) {
    uint16_t chunk_size = (bytes_remaining > LEN_DATAGRAM_MAX_PAYLOAD)
                            ? LEN_DATAGRAM_MAX_PAYLOAD
                            : bytes_remaining;

    uint16_t bytes_read = ConfigMemHelper_config_mem_read(
      openlcb_node,
      address,
      chunk_size,
      &temp_buffer);

    if (bytes_read != chunk_size) {
      _direct_access = false;
      return false;  // Error or partial read
    }

    // Copy chunk to destination
    memcpy(dest, temp_buffer, chunk_size);

    // Advance pointers for next chunk
    address += chunk_size;
    dest += chunk_size;
    bytes_remaining -= chunk_size;
  }

  _direct_access = false;
  return true;
}

/**
 * Write NODE segment to configuration memory
 *
 * Address: 0x00
 * Space: 0xFD (Configuration)
 * Type: config_mem_t
 *
 * Writes in chunks (max 64 bytes per call) to respect datagram size limits.
 *
 * @param openlcb_node Pointer to OpenLCB node
 * @param config Pointer to nodeid_t struct to write
 * @return true on success, false on error
 */
bool ConfigMemHelper_write(openlcb_node_t *openlcb_node, config_mem_t *config) {
  if (!openlcb_node || !config) {
    return false;
  }

  uint32_t address = NODE_ADDR;  // Starting address from #define
  uint32_t total_size = sizeof(config_mem_t);
  uint32_t bytes_remaining = total_size;
  uint8_t *src = (uint8_t *)config;

  configuration_memory_buffer_t temp_buffer;

  _direct_access = true;

  // Write in chunks (max 64 bytes per datagram)
  while (bytes_remaining > 0) {
    uint16_t chunk_size = (bytes_remaining > LEN_DATAGRAM_MAX_PAYLOAD)
                            ? LEN_DATAGRAM_MAX_PAYLOAD
                            : bytes_remaining;

    // Copy chunk to buffer
    memcpy(temp_buffer, src, chunk_size);

    uint16_t bytes_written = ConfigMemHelper_config_mem_write(
      openlcb_node,
      address,
      chunk_size,
      &temp_buffer);

    if (bytes_written != chunk_size) {

      _direct_access = false;
      return false;  // Error or partial write

    }

    // Advance pointers for next chunk
    address += chunk_size;
    src += chunk_size;
    bytes_remaining -= chunk_size;
  }

  _direct_access = false;
  return true;
}

void ConfigMemHelper_reset_config_mem(void) {

  configuration_memory_buffer_t buffer;

  _direct_access = true;

  memset(&buffer, 0xFF, sizeof(buffer));
  uint16_t address = 0;
  for (unsigned int i = 0; i < (CONFIG_MEM_SIZE / sizeof(buffer)); i++) {

    ConfigMemHelper_config_mem_write(NULL, address, sizeof(buffer), &buffer);
    address = address + sizeof(buffer);
  }

  _direct_access = false;

}

void ConfigMemHelper_clear_config_mem(void) {

  configuration_memory_buffer_t buffer;

  _direct_access = true;

  memset(&buffer, 0x00, sizeof(buffer));
  uint16_t address = 0;
  for (unsigned int i = 0; i < (CONFIG_MEM_SIZE / sizeof(buffer)); i++) {

    ConfigMemHelper_config_mem_write(NULL, address, sizeof(buffer), &buffer);
    address = address + sizeof(buffer);
  }

  _direct_access = false;
}

bool ConfigMemHelper_is_config_mem_reset(void) {

  configuration_memory_buffer_t buffer;
  
  _direct_access = true;

  ConfigMemHelper_config_mem_read(NULL, 0, 1, &buffer);

  _direct_access = false;

  return (buffer[0] == 0xFF);
}

long getSteps() {
  char data[4];
  long eepromSteps;
  stepsSet = true;
  
  if (stepsSet) {
    eepromSteps = (swap_endian64(ConfigMemHelper_config_data.attributes.FullTurnSteps));
    if (eepromSteps <= sanitySteps) {
#ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN steps read from EEPROM: "));
      Serial.println(eepromSteps);
#endif
      return eepromSteps;
    } else {
#ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN steps in EEPROM are invalid: "));
      Serial.println(eepromSteps);
#endif
      calibrating = true;
      return 0;
    }
  } else {
#ifdef TT_DEBUG
    Serial.println(F("DEBUG: TTLN steps not defined in EEPROM"));
#endif
    calibrating = true;
    return 0;
  }
}

// Function to write step count with "TTLN" identifier to EEPROM.
void writeSteps(long steps) {
  ConfigMemHelper_config_data.attributes.FullTurnSteps = swap_endian64(steps);
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN wrote steps in EEPROM: "));
      Serial.println(steps);
  #endif
}
// write the current track number and orientation
void writeTrack(uint8_t i,uint8_t Direction){
      if (i > NUM_TRACKS) return;
      // ee.writeBlock(9,(uint8_t *) &i, sizeof(i));  // mesh with other EEPROM uses - need to fix
      ConfigMemHelper_config_data.CurrentTrack = i;
      ConfigMemHelper_config_data.BridgeOrientation = Direction;
  }

// write the current track number and orientation
void getTrack(){      
      CurrentTrack =  ConfigMemHelper_config_data.CurrentTrack; // ee.readByte(9);  
      BridgeOrientation = ConfigMemHelper_config_data.BridgeOrientation; // ee.readByte(11);
      if (ConfigMemHelper_config_data.CurrentTrack > NUM_TRACKS)ConfigMemHelper_config_data.CurrentTrack = 0;
}

// write the number of tracks 
void writeCount(){
      ConfigMemHelper_config_data.attributes.TrackCount = trackCount;
      ConfigMemHelper_config_data.attributes.ReferenceCount = refCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN wrote track count in EEPROM: "));
      Serial.println(trackCount);
      Serial.print(F(" and reference count in EEPROM: "));
      Serial.println(ConfigMemHelper_config_data.attributes.ReferenceCount);
  #endif
}

// get the number of tracks 
void getCount(){
      trackCount = ConfigMemHelper_config_data.attributes.TrackCount;
      refCount = ConfigMemHelper_config_data.attributes.ReferenceCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN read track count in EEPROM: "));
      Serial.println(trackCount);
      Serial.print(F(" and reference count in EEPROM: "));
      Serial.println(ConfigMemHelper_config_data.attributes.ReferenceCount);
  #endif
}
void writeTracks()
{
  /* 
  int address;
	long trackFront;
	long trackBack;
  bool doorPresent;
  int servoNumber;
  */
  rp2040.idleOtherCore();

  ConfigMemHelper_config_data.attributes.TrackCount = trackCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN wrote track count in EEPROM: "));
      Serial.println(trackCount);
  #endif

  // for (int i = 0; i < (sizeof(Tracks) / sizeof(TrackAddress)); i++) {
  // ConfigMemHelper_config_data.Tracks[i] = Tracks[i];
  // }
  
  rp2040.resumeOtherCore();
}

void getTracks()
{
  /* 
  int address;
	long trackFront;
	long trackBack;
  bool doorPresent;
  int servoNumber;
  */
  rp2040.idleOtherCore();

  trackCount = ConfigMemHelper_config_data.attributes.TrackCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN read track count in EEPROM: "));
      Serial.println(trackCount);
  #endif

  // for (int i = 0; i < (sizeof(Tracks) / sizeof(TrackAddress)); i++) {
  // Tracks[i] = ConfigMemHelper_config_data.Tracks[i];
  // }
  rp2040.resumeOtherCore();
}

void writeReferences()
{
    // add writeReferences
  rp2040.idleOtherCore();

  ConfigMemHelper_config_data.attributes.ReferenceCount = refCount;

  for (int i = 0; i < (sizeof(References) / sizeof(ReferenceStep)); i++) {
  ConfigMemHelper_config_data.References[i] = References[i];
  }

  rp2040.resumeOtherCore();
}

void getReferences()
{
  // add read References
  
  rp2040.idleOtherCore();
  
  refCount = ConfigMemHelper_config_data.attributes.ReferenceCount;

  for (int i = 0; i < (sizeof(References) / sizeof(ReferenceStep)); i++) {
  References[i] = ConfigMemHelper_config_data.References[i];
  }

  rp2040.resumeOtherCore();
}
