/*
 *  This is the configuration file for the Raspberry Pi Pico node hardware.
    defines and constants
 */

// Configureation settings

#ifndef DEFINES_H
#define DEFINES_H

// #include <cstdint>

#define ARDUINO_COMPATIBLE

// --------------------------------------------
// Select ONE of these for Non-volitile Memory Storage
// --------------------------------------------
// #define USE_INTERNAL_FLASH_STORAGE
#define USE_I2C_STORAGE

// if using external NVM storage, select ONE of these supported types, otherwise use none
#define EXTERNAL_EEPROM     // if uncommented uses external eeprom, using Adafruit library
// #define EXTERNAL_FRAM         // if uncommented uses externalFRAM, using Adafruit library

// if using external NVM, use either Tillaart library or default to Adafruit library
#define USE_TILLAART       // if uncommented then uses Rob Tillaart EEPROM and/or FRAM library

// --------------------------------------------

// -------------------------------------------
// Select ONE of these for Configuration Memory Size
// --------------------------------------------
// #define CONFIG_MEM_SIZE      65536
#define CONFIG_MEM_SIZE      32768
//#define CONFIG_MEM_SIZE      16384
//#define CONFIG_MEM_SIZE      8192
//#define CONFIG_MEM_SIZE      4096
//#define CONFIG_MEM_SIZE      2048
//#define CONFIG_MEM_SIZE      1024
//#define CONFIG_MEM_SIZE      512
//#define CONFIG_MEM_SIZE      256
//#define CONFIG_MEM_SIZE      128
// --------------------------------------------

// Define the size of the EEPROM chip or use 4096 if using emulated internal flash storage
#define I2C_DEVICESIZE      65536  // 24LC512
// #define I2C_DEVICESIZE      32768  // 24LC256
// #define I2C_DEVICESIZE      16384  // 24LC128
// #define I2C_DEVICESIZE       8192  // 24LC64
// #define I2C_DEVICESIZE       4096  // 24LC32    // this is the size to use for internal FLASH EEPROM emulation
// #define I2C_DEVICESIZE       2048  // 24LC16
// #define I2C_DEVICESIZE       1024  // 24LC08
// #define I2C_DEVICESIZE        512  // 24LC04
// #define I2C_DEVICESIZE        256  // 24LC02
// #define I2C_DEVICESIZE        128  // 24LC01

/////////////////////////////////////////////////////////////////////////////////////
//  Define a valid (and free) I2C address, 0x60 is the default.
// 
// #define I2C_ADDRESS 0x60 
// #define KEYPAD_ADDRESS 0x20
// #define DISPLAY_ADDRESS 0x3C
// #define SERVO_ADDRESS 0x40
// #define EEPROM_ADDRESS 0x50
#define STORAGE_ADDR 0x50  // 0x50 is the default address!
#define STOR_WIRE Wire1     // make Wire1 or Wire

#define I2C_SDA  26           // pin to use
#define I2C_SCL  27           // pin to use
// #define I2C2_SDA  4           // pin to use
// #define I2C2_SCL  5           // pin to use

#define TOUCH_SDA  4
#define TOUCH_SCL  5
//                  The touch screen interrupt request pin (T_IRQ) is not used
#define TOUCH_INT  3
#define TOUCH_RST  -1

#define TOUCH_WIRE Wire

// #include <NeoPixelBusLg.h>

// #define KeyPad_Int_Pin 2    // Pin for interupt from I2C module
#define BRIDGE_SENSOR_PIN 0    // Bridge Position Sensor Input (any - 11 / 23)
#define HOME_SENSOR_PIN 1      // Home Position Sensor Input

#define STEPPER_INTERFACE 1
#define STEPPER_ENABLE_PIN 14   // (use any - 13 / 31) Uncomment to enable Powering-Off the Stepper if its not running 
#define STEPPER_STEP_PIN 15      // (use any - 4 / 27)
#define STEPPER_DIR_PIN 2       // (use any - 5 / 29)

#define NeoPixel_PinA 21        // (use any (mega 22-43) - 12 / 25) for the bridge / board interface
// #define NeoPixel_PinA 2           // pin to use for the board interface
// #define NeoPixel_PinB 6           // pin to use for the board interface
// #define NeoPixel_PinC 7           // pin to use for the board interface
// #define NeoPixel_PinD 3           // pin to use for the board interface

//——————————————————————————————————————————————————————————————————————————————
//  MCP2517 connections: adapt theses settings to your design
//  As hardware SPI is used, you should select pins that support SPI functions.
//  This code is designed to use SPI
//  If standard SPI, SPI2 pins are not used then define them
//    SCK input of MCP2517 is connected to pin #32
//    SDI input of MCP2517 is connected to pin #0
//    SDO output of MCP2517 is connected to pin #1
//  CS input of MCP2517 should be connected to a digital output port
//  INT output of MCP2517 should be connected to a digital input port, with interrupt capability

// static const byte MCP2517_SCK = 18 ; // SCK input of MCP2517/8
// static const byte MCP2517_SDI =  19 ; // SI input of MCP2517/8
// static const byte MCP2517_SDO =  16 ; // SO output of MCP2517/8
// static const byte MCP2517_CS  = 17 ; // CS input of MCP2517/8
// static const byte MCP2517_INT = 20 ; // INT output of MCP2517/8
//——————————————————————————————————————————————————————————————————————————————

#define MCP2517_SPI  SPI   // SPI port to use for MCP2517/8

#define MCP2517_CS  17   // CS input of MCP2517/8
#define MCP2517_INT 20  // INT output of MCP2517/8
#define MCP2517_SCK 18  // SCK input of MCP2517/8
#define MCP2517_SDI 19  // SI input of MCP2517/8
#define MCP2517_SDO 16  // SO output of MCP2517/8

/*
 *  This is the configuration file for the NeoPixel operation.
 */

// Configureation settings

// #include <cstdint>

/////////////////////////////////////////////////////////////////////////////////////
//  Define the LED blink rates for fast and slow blinking in milliseconds.
// 
//  The LED will alternative on/off for these durations.
#define FREQUENCY 100

// Define current version of EEPROM configuration
#define EEPROM_VERSION 8

//  Enable debug outputs if required during troubleshooting.
#define NODE_DEBUG true  // uncomment for debug
#define TT_DEBUG true  // uncomment for debug
// #define TT2_DEBUG true  // uncomment for debug

#define NOTICE_PRINT Serial //serial or tft

// NeoPixel defines

#define NeoPixel_PIO pio0
const uint16_t PixelCount = 2; // number of NeoPixels in the string
const uint8_t PixelPin = NeoPixel_PinA;  // pin for the data line, ignored for Esp8266

#define brightnesss 90
#define MAX_LUMINANCE 100
#define DIM_LUMINANCE 35
#define MIN_LUMINANCE 5
#define i_max_pixel MAX_STRINGS * MAX_LIGHTS     // number of pixel drivers in the daisy-chain

#define MAX_STRINGS 1
#define MAX_LIGHTS 20
#define NumOfLights 2
#define Light_A 1
#define Light_B 0

#define MAX_TRACKS 20
#define NUM_TRACKS 15 // Define number of turntable tracks for default / max, includes non-track zero position for home sensor
#define MAX_DOORS 16
#define NUM_DOORS 10

// need to define number of all events in event table
// #define NUM_EVENT 5

#define NUM_SERVOS 10
#define NUM_POS 2

#define NUM_TABLE_EVENTS  5
#define NUM_TRACK_EVENTS MAX_TRACKS * 4
#define NUM_DOOR_EVENTS 2 + MAX_DOORS // 2 All events and a toggle for each servo
// Light events
#define NUM_LUM_EVENTS  5
// #define NUM_LIGHT_EVENTS MAX_STRINGS * MAX_LIGHTS * 9
#define NUM_EVENT NUM_DOOR_EVENTS + NUM_LUM_EVENTS + NUM_TRACK_EVENTS + NUM_TABLE_EVENTS 

#define IndexOpenAll NUM_TABLE_EVENTS + NUM_TRACK_EVENTS // PEID(OpenAll) OpenLcb.produce(IndexOpenAll);
#define IndexCloseAll IndexOpenAll + 1 // PEID(CloseAll) OpenLcb.produce(IndexCloseAll);
#define IndexDoor1 IndexCloseAll + 1 // PEID(doors[s].eidToggle) OpenLcb.produce(IndexDoor1 + i);
#define IndexLightIn  NUM_TABLE_EVENTS + NUM_TRACK_EVENTS + NUM_DOOR_EVENTS + 1   // Light events  PEID(eidInterior) OpenLcb.produce(IndexLightIn);
#define IndexLightEx IndexLightIn + 1 // PEID(eidExterior) OpenLcb.produce(IndexLightEx);

const uint8_t relay1Pin = 25;                        // Control pin for relay 1.
const uint8_t relay2Pin = 25;                        // Control pin for relay 2.
const uint8_t ledPin = 25;                           // Pin for LED output.
const uint8_t accPin = 25;                           // Pin for accessory output.

/*
 *  This is the configuration file for LCC Turntable.
 */

// TFT Screen pixel resolution in landscape orientation, change these to suit your display
// Defined in landscape orientation !
#define HRES 800
#define VRES 480
#define ROTATION 3  // rotation for screen
#define TOUCH_ROTATION 0  // rotation for touch
#define DEBOUNCE_TOUCH 40
#define DEBOUNCE_BOX 80

/* The TFT interface is defined in the User_Setup.h file in the TFT_eSPI Arduino library folder. These are my settings:
////////////////////////////////////////////////////////////////////////////////////////////
// RP2040 pins used
////////////////////////////////////////////////////////////////////////////////////////////

//#define TFT_CS   -1  // Do not define, chip select control pin permanently connected to 0V

// These pins can be moved and are controlled directly by the library software
#define TFT_DC   28    // Data Command control pin
#define TFT_RST   2    // Reset pin

//#define TFT_RD   -1  // Do not define, read pin permanently connected to 3V3

// Note: All the following pins are PIO hardware configured and driven
  #define TFT_WR   22

  // PIO requires these to be sequentially increasing - do not change
  #define TFT_D0    6
  #define TFT_D1    7
  #define TFT_D2    8
  #define TFT_D3    9
  #define TFT_D4   10
  #define TFT_D5   11
  #define TFT_D6   12
  #define TFT_D7   13

//#define TOUCH_CS -1     // Chip select pin (T_CS) of touch screen

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT

/***************************************************************************************
**                         Section 5: Font datum enumeration
**************************************************************************************
//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

/***************************************************************************************
**                         Section 6: Colour enumeration
**************************************************************************************
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 
#define TFT_NAVY        0x000F      /*   0,   0, 128 
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 
#define TFT_MAROON      0x7800      /* 128,   0,   0 
#define TFT_PURPLE      0x780F      /* 128,   0, 128 
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 
#define TFT_BLUE        0x001F      /*   0,   0, 255 
#define TFT_GREEN       0x07E0      /*   0, 255,   0 
#define TFT_CYAN        0x07FF      /*   0, 255, 255 
#define TFT_RED         0xF800      /* 255,   0,   0 
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 
#define TFT_PINK        0xFE19      /* 255, 192, 203  //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 
#define TFT_SILVER      0xC618      /* 192, 192, 192 
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 
#define TFT_VIOLET      0x915C      /* 180,  46, 226 
*/
#define RailColor       0x9A60      /* 150,  75,   0  brown */

#define RedLevel 50   // bridge center
#define BlueLevel 10  // bridge shack
#define GreenLevel 0

#define USE_SENSORS true  // uncomment for LocoNet sensor reporting

#define UNUSED_PIN 127

/* User Interface Parameters */

#define PageBoxes 32         // number of buttons on all pages for boxes in array. 
// setting buttons = 2 * variables, add to pageboxes to get track count variable box 
#define TrackSelBoxes 38         // ending number of buttons for variable update boxes in array. 
// setting buttons = 7 * tracks, add to TrackSelBoxes to get track box starting point
#define TrackBoxes 156         // ending number of buttons for track update boxes in array. 
// setting buttons = 2 * variables, add to TrackBoxes to get servo variable update box 
#define ServoSelBoxes 158         // ending number of buttons for variable update boxes in array. 
// setting buttons = 6 * servos, add to TrackSelBoxes to get track box starting point
#define ServoBox 218         // ending point for servo boxes in array
// setting buttons = 3 * tracks, add to ServoBox to get track box starting point
#define TrackBox 278         // ending point for turntable diagram track boxes in array. 3 boxes per track, add to get possible boxes
#define PossibleBoxes 500    // space for array of click box boundaries (static boxes plus track boxes)

#define TT_DIA (VRES/2.5)+10    // turn table diameter plus 10
#define TT_RAD (VRES/5)     // turn table radius
#define TT_ROTATION 90      // rotation of TT diagram
#define TT_STATION_SIZE 18    // size of track button
#define TT_CX HRES/2        // center X position
#define TT_CY VRES/2        // center Y position
#define TT_B_WIDTH 12     // button width
#define TT_S_WIDTH 8      // track width
#define TT_S_SPACE 65     // space between tracks

#define T_FRINGE  5   // added pixels around a touch box

// Stepper tracks
#define NumberOfReferences 10   // Define max number of stepper reference points

/*           Stepper Section

 Define the stepper controller in use according to those available below, refer to the
 documentation for further details on which to select for your application.

 ULN2003_HALF_CW     : ULN2003 in half step mode, clockwise homing/calibration
 ULN2003_HALF_CCW    : ULN2003 in half step mode, counter clockwise homing/calibration
 ULN2003_FULL_CW     : ULN2003 in full step mode, clockwise homing/calibration
 ULN2003_FULL_CCW    : ULN2003 in full step mode, counter clockwise homing/calibration
 A4988               : Two wire drivers (eg. A4988, DRV8825)
 A4988_INV           : Two wire drivers (eg. A4988, DRV8825), with enable pin inverted

 NOTE: If you are using a different controller than those already defined, refer to
 the documentation to define the appropriate configuration variables. Note there are
 some controllers that are pin-compatible with an existing defined controller, and
 in those instances, no custom configuration would be required.
*/
// #define STEPPER_DRIVER ULN2003_HALF_CW
// #define STEPPER_DRIVER ULN2003_HALF_CCW
// #define STEPPER_DRIVER ULN2003_FULL_CW
// #define STEPPER_DRIVER ULN2003_FULL_CCW
// #define STEPPER_DRIVER A4988
#define STEPPER_DRIVER A4988_INV

/////////////////////////////////////////////////////////////////////////////////////
//  Define the various stepper configuration items below if the defaults don't suit
//
//  Disable the stepper controller when idling, comment out to leave on. Note that this
//  is handy to prevent controllers overheating, so this is a recommended setting.
#define DISABLE_OUTPUTS_IDLE
// 
//  Define the acceleration and speed settings.
#define STEPPER_MAX_SPEED 1800     // Maximum possible speed the stepper will reach
#define STEPPER_ACCELERATION 180   // Acceleration and deceleration rate
// 
#define FULLSTEPS 400
// If using a gearing or microstep setup with larger than 32767 steps, you need to set the
// gearing factor appropriately.
#define STEPPER_GEARING_FACTOR 1    //15 for layout
#define DRIVER_BOARD_STEPPING 16    // 1, 2, 4, 8, or 16 are valid
#define FTS FULLSTEPS * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING
// This section is legacy hardcode track positioning
// for a 1.8 deg stepper, there are 200 full steps
const long FULL_STEPS_PER_REVOLUTION = FULLSTEPS*STEPPER_GEARING_FACTOR;
const long FULL_TURN_STEPS = FTS; // calculated steps

// define the position of each track for program defaults

const long Bump = 10*STEPPER_GEARING_FACTOR;

const long entryTrack1 = (FULL_TURN_STEPS-(FULL_TURN_STEPS / 36 * 2) + (1*STEPPER_GEARING_FACTOR));
const long entryTrack2 = (FULL_TURN_STEPS-(FULL_TURN_STEPS / 36 * 1) + (38*STEPPER_GEARING_FACTOR));
const long entryTrack3 = 33*STEPPER_GEARING_FACTOR;       // home location
const long houseTrack1 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 11)  + (54*STEPPER_GEARING_FACTOR));
const long houseTrack2 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 10)  + (44*STEPPER_GEARING_FACTOR));
const long houseTrack3 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 9)  + (48*STEPPER_GEARING_FACTOR));
const long houseTrack4 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 8)  + (50*STEPPER_GEARING_FACTOR));
const long houseTrack5 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 7)  + (40*STEPPER_GEARING_FACTOR));
const long houseTrack6 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 6)  + (40*STEPPER_GEARING_FACTOR));
const long houseTrack7 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 5)  + (40*STEPPER_GEARING_FACTOR));
const long houseTrack8 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 4) + (34*STEPPER_GEARING_FACTOR));
const long houseTrack9 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 3)  + (26*STEPPER_GEARING_FACTOR));
const long houseTrack10 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 2)  + (33*STEPPER_GEARING_FACTOR));
const long houseTrack11 = (FULL_TURN_STEPS/2-(FULL_TURN_STEPS / 36 * 0) + (33*STEPPER_GEARING_FACTOR));

//  Define the maximum number of steps homing and calibration will perform before marking
//  these activities as failed. This step count must exceed a single full rotation in order to be useful.
#define SANITY_STEPS FULLSTEPS * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING * 3
// 
//  Define the minimum number of steps the turntable needs to move before the homing sensor
//  deactivates, which is required during the calibration sequence. For high step count
//  setups, this may need to be increased.
#define HOME_SENSITIVITY 20 * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING
#define BRIDGE_SENSITIVITY 10 * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING
#define REHOME_THRESHOLD 1 * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING
#define BRIDGE_PRECISION 10 * STEPPER_GEARING_FACTOR * DRIVER_BOARD_STEPPING
// 
//  Override the step count determined by automatic calibration by uncommenting the line
//  below, and manually defining a specific step count.
// #define FULL_STEP_COUNT 6400
// 
// Ensure AUTO and MANUAL phase switching has a value to test.
#define AUTO 1
#define MANUAL 0

/////////////////////////////////////////////////////////////////////////////////////
//  Define the active state for the homing sensor.
//  LOW = When activated, the input is pulled down (ground or 0V).
//  HIGH = When activated, the input is pulled up (typically 5V). 
#define HOME_SENSOR_ACTIVE_STATE LOW

/////////////////////////////////////////////////////////////////////////////////////
//  REQUIRED FOR BRIDGE MODE ONLY - Define the active state for the bridge position sensor.
//  LOW = When activated, the input is pulled down (ground or 0V).
//  HIGH = When activated, the input is pulled up (typically 5V).
#define BRIDGE_SENSOR_ACTIVE_STATE LOW

/////////////////////////////////////////////////////////////////////////////////////
//  Define the active state for the phase switching relays.
//  LOW = When activated, the input is pulled down (ground or 0V).
//  HIGH = When activated, the input is pulled up (typically 5V).
// #define RELAY_ACTIVE_STATE HIGH

/////////////////////////////////////////////////////////////////////////////////////
//  Define phase switching behaviour.
//  PHASE_SWITCHING options:
//  AUTO    : When defined, phase will invert at PHASE_SWITCH_START_ANGLE, and revert
//            at PHASE_SWITCH_STOP_ANGLE (see below).
//  MANUAL  : When defined, phase will only invert using the Turn_PInvert command.
//  Refer to the documentation for the full explanation on phase switching, and when
//  it is recommended to change these options.
#define PHASE_SWITCHING AUTO

/////////////////////////////////////////////////////////////////////////////////////
//  Define automatic phase switching angle.
//  If PHASE_SWITCHING is set to AUTO (see above), then when the turntable rotates
//  PHASE_SWITCH_ANGLE degrees from home, the phase will automatically invert.
//  Once the turntable reaches a further 180 degrees, the phase will automatically revert.
//  Refer to the documentation for the full explanation on phase switching, and how to
//  define the angle that's relevant for your layout.
// #define PHASE_SWITCH_ANGLE 45

/////////////////////////////////////////////////////////////////////////////////////
//  Define the LED blink rates for fast and slow blinking in milliseconds.
//  The LED will alternative on/off for these durations.
#define LED_FAST 100
#define LED_SLOW 500

/////////////////////////////////////////////////////////////////////////////////////
//  ADVANCED OPTIONS
//  In normal circumstances, the settings below should not need to be adjusted unless
//  requested by support ticket, or if Tinkerers or Engineers are working with alternative
//  stepper drivers and motors.
// 
//  Override the default debounce delay (in ms) if using mechanical home/limit switches that have
//  "noisy" switch bounce issues.
//  In TRAVERSER mode, default is 10ms as these would typically use mechanical switches.
//  In TURNTABLE mode, default is 0ms as these would typically use hall effect sensors.
#define DEBOUNCE_DELAY 0

#endif

