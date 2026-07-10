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

void ConfigMemHelper_mirror_write(uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer) {

  if (address >= sizeof(config_mem_t))
    return;

  if (address + count > sizeof(config_mem_t))
    count = (uint16_t)(sizeof(config_mem_t) - address);

  uint8_t *byte_array = (uint8_t*) &ConfigMemHelper_config_data;
  memcpy(byte_array + address, buffer, count);

}

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
  // CurrentTrack is a top-level config_mem_t field (not under .attributes), so
  // it's runtime state rather than a CDI-defined value — none of the
  // _load_defaults_* functions touched it, meaning an 'r' (wipe to 0xFF) + 'i'
  // (write defaults) reset left it at 255. drawBridge() indexes
  // TrackName[CurrentTrack] (TrackName[MAX_TRACKS=20]) with no bounds check,
  // so CurrentTrack=255 read 6375 bytes past the array into whatever flash
  // data the linker placed next — which turned out to render as genuine,
  // readable text from this node's own embedded CDI XML. Explicitly default
  // it here so a reset actually initializes it.
  config->CurrentTrack = 3; // matches HomeTrack default above
  config->attributes.ReferenceCount = false; // Disable reference correction by default
  config->attributes.FullTurnSteps = swap_endian32(FULL_TURN_STEPS); // number of steps in a full turn of the turntable, needs to be in little endian for the OpenLcbLib functions to write it correctly to memory and have it be correct when read back on a big endian system
  config->attributes.Rehome = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++;// EventID for rehome
  config->attributes.IncrementTrack = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for increment track
  config->attributes.DecrementTrack = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for decrement track
  config->attributes.RotateTrack180 = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for rotate track 180
  config->attributes.ToggleBridgeLights = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for toggle bridge lights
  for (int t = 0; t < MAX_TRACKS; t++) {
    // strncpy only null-terminates dest if src is SHORTER than dest's size --
    // if src is ever >= sizeof(dest), the copy fills the whole buffer with no
    // terminator at all. putString() (RA8876_common.cpp) renders strings with
    // an unbounded "while (*str != '\0')" loop, so an unterminated trackName/
    // trackShort would make it walk off the end of the array and keep
    // rendering whatever bytes happen to follow in memory until it stumbles
    // onto a zero byte. Force-terminate explicitly regardless of src length.
    strncpy(config->attributes.tracks[t].trackName, TrackName[t], sizeof(config->attributes.tracks[t].trackName));
    config->attributes.tracks[t].trackName[sizeof(config->attributes.tracks[t].trackName) - 1] = '\0';
    strncpy(config->attributes.tracks[t].trackShort, TrackTag[t], sizeof(config->attributes.tracks[t].trackShort));
    config->attributes.tracks[t].trackShort[sizeof(config->attributes.tracks[t].trackShort) - 1] = '\0';
    // config->attributes.tracks[t].trackShort = TrackTag[t];
    config->attributes.tracks[t].Front = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track front
    config->attributes.tracks[t].Back = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track back
    config->attributes.tracks[t].Occupancy = swap_endian64((openlcb_node->id << 16) + *consumer_index); (*consumer_index)++; // EventID for track occupancy
    config->attributes.tracks[t].RailCom = 0; // EventID for track railcom
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
    strncpy(config->attributes.doors[d].doorShort, door_tag, sizeof(config->attributes.doors[d].doorShort));
    config->attributes.doors[d].eidToggle = swap_endian64((openlcb_node->id << 16) + *producer_index); (*producer_index)++; // EventID for toggle door
    config->attributes.doors[d].TrackLocation = 4 + d; // Default track location for each door
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
  for (int i = 0; i < (5+3*MAX_TRACKS+3); i++) {
    config->consumer_status[i] = EVENT_STATUS_UNKNOWN; // Default event state is unknown (0)
  }

  for (int i = 0; i < (2+MAX_DOORS+2); i++) {
    config->producer_status[i] = EVENT_STATUS_UNKNOWN; // Default event state is unknown (0)
  }

}
static void _load_defaults_application(openlcb_node_t *openlcb_node, config_mem_t *config, uint16_t *consumer_index, uint16_t *producer_index) {
  // Initialise all track slots to safe defaults; actual positions are derived from
  // CDI attributes by Set_Application_Values_From_Config() called below.
  for (int i = 0; i < (int)(sizeof(config->Tracks) / sizeof(TrackAddress)); i++) {
    config->Tracks[i].address =  i;
    config->Tracks[i].trackFront = 0;
    config->Tracks[i].trackBack = (FULL_TURN_STEPS / 2);
    config->Tracks[i].doorPresent = false;
    config->Tracks[i].servoNumber = 0;
  }
  Set_Application_Values_From_Config(openlcb_node, config);
}

void Set_Application_Values_From_Config(openlcb_node_t *openlcb_node, config_mem_t *config) {
  // Track 0 is the homing-sensor position (step 0) — its position is established at
  // run-time by the homing routine, not from the CDI.  Tracks 1 through TrackCount
  // are the user-configured usable tracks; TrackCount is the number of usable tracks
  // as entered in the CDI (track 0 is not counted).
  //
  // TrackCount/DoorCount come straight from NVM, which may hold stale or
  // incompatible data (e.g. carried over from a different board/config
  // layout). Clamp to the fixed array sizes so a corrupt count can never walk
  // these loops past Tracks[]/tracks[]/doors[] — an out-of-bounds write here
  // previously corrupted adjacent memory and hung the node during setup1().
  uint8_t trackCount = config->attributes.TrackCount;
  if (trackCount > MAX_TRACKS - 1) trackCount = MAX_TRACKS - 1;

  for (int i = 1; i <= trackCount; i++) {
    config->Tracks[i].trackFront = swap_endian32(config->attributes.tracks[i].steps);
    config->Tracks[i].trackBack = config->Tracks[i].trackFront + (swap_endian32(config->attributes.FullTurnSteps) / 2);
    config->Tracks[i].doorPresent = false;
    config->Tracks[i].servoNumber = 0;
  }

  uint8_t doorCount = config->attributes.DoorCount;
  if (doorCount > MAX_DOORS) doorCount = MAX_DOORS;

  for (int d = 0; d < doorCount; d++) {
    uint8_t trackLocation = config->attributes.doors[d].TrackLocation;
    if (trackLocation > 0 && trackLocation < MAX_TRACKS) {
      config->Tracks[trackLocation].doorPresent = true;
      config->Tracks[trackLocation].servoNumber = d;
    }
  }
  homeTrack = config->attributes.HomeTrack;
}
uint16_t ConfigMemHelper_config_mem_write(openlcb_node_t *openlcb_node, uint32_t address, uint16_t count, configuration_memory_buffer_t *buffer) {

  // Hook into the Configuration Memory Write to update the data structures in parallel
  uint16_t bytes_written = 0;
  // Are we in the internal process of syncing the NMV with the struct?  If so just write to the NVM as we are syncing them.
  if (_direct_access) {  

    delay(10);
    bytes_written = RPiPicoDrivers_config_mem_write(openlcb_node, address, count, buffer);
    // RPiPicoDrivers_config_mem_read(openlcb_node, address, count, buffer);
    delay(10);
    Set_Application_Values_From_Config(openlcb_node, &ConfigMemHelper_config_data);
    return bytes_written;
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

  // Sync application state (servo ranges, event states, etc.) from updated config
  Set_Application_Values_From_Config(openlcb_node, &ConfigMemHelper_config_data);

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
  uint16_t producer_index = 128; // Start producer index at 128 to leave room for the 128 consumer events defined in the attributes for auto assignment of EventIDs to producers after the consumer EventIDs in the nodeid space

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

  // Clamp NVM-sourced counts to their actual array bounds here, once, at the
  // single point every normal boot loads config from NVM. TrackCount/
  // DoorCount are read directly (unclamped) all over this codebase
  // (Turntable.cpp, UserInterface.cpp, callbacks.cpp) — fixing it at the
  // source means every one of those loops is safe automatically, rather
  // than needing the same clamp re-applied at every call site. Stale/
  // incompatible NVM data with a bad count here previously caused an out-
  // of-bounds read in drawTracks() that rendered unrelated memory content
  // (once, literally the tail of the node's own CDI XML) as on-screen text.
  if (config->attributes.TrackCount > MAX_TRACKS - 1) {
    config->attributes.TrackCount = MAX_TRACKS - 1;
  }
  if (config->attributes.DoorCount > MAX_DOORS) {
    config->attributes.DoorCount = MAX_DOORS;
  }
  // CurrentTrack indexes TrackName[MAX_TRACKS] (drawBridge(), UserInterface.cpp)
  // with no bounds check at the use site. Same class of bug as the two
  // clamps above, just for a top-level field instead of one under
  // .attributes — see the comment on the default-loading side in
  // _load_defaults_attributes() for the full story (this is what actually
  // caused stray CDI text to render on screen during homing).
  if (config->CurrentTrack > MAX_TRACKS - 1) {
    config->CurrentTrack = 0;
  }

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

bool ConfigMemHelper_nvm_is_accessible(void) {

  configuration_memory_buffer_t buffer;

  _direct_access = true;
  uint16_t bytes_read = ConfigMemHelper_config_mem_read(NULL, 0, 1, &buffer);
  _direct_access = false;

  return (bytes_read == 1);
}

bool ConfigMemHelper_is_config_mem_reset(void) {

  configuration_memory_buffer_t buffer;

  _direct_access = true;

  uint16_t bytes_read = ConfigMemHelper_config_mem_read(NULL, 0, 1, &buffer);

  _direct_access = false;

  if (bytes_read != 1) {
    Serial.println("ConfigMemHelper_is_config_mem_reset: read failed, cannot determine NVM state");
    return false;
  }

  return (buffer[0] == 0xFF);
}

long getSteps() {
  long eepromSteps = (swap_endian64(ConfigMemHelper_config_data.attributes.FullTurnSteps));
  stepsSet = true;

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
  rp2040.idleOtherCore();

  ConfigMemHelper_config_data.attributes.TrackCount = trackCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN wrote track count in EEPROM: "));
      Serial.println(trackCount);
  #endif

  rp2040.resumeOtherCore();
}

void getTracks()
{
  rp2040.idleOtherCore();

  trackCount = ConfigMemHelper_config_data.attributes.TrackCount;
  #ifdef TT_DEBUG
      Serial.print(F("DEBUG: TTLN read track count in EEPROM: "));
      Serial.println(trackCount);
  #endif

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
