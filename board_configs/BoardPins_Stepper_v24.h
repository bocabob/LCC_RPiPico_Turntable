/*
 *  Pin definitions for LCC RPi Pico Stepper board v2.4
 *
 *  Do not include this file directly — it is selected automatically
 *  by BoardSettings.h based on the LCC_BOARD_* define in the .ino file.
 */

#ifndef BOARDPINS_STEPPER_V24_H
#define BOARDPINS_STEPPER_V24_H

// --------------------------------------------
//  Sensors
// --------------------------------------------
#define BRIDGE_SENSOR_PIN   0   // Bridge position sensor
#define HOME_SENSOR_PIN     1   // Home position sensor

// --------------------------------------------
//  Stepper driver (A4988 / DRV8825 style)
// --------------------------------------------
#define STEPPER_INTERFACE       1
#define STEPPER_DIR_PIN         2
#define STEPPER_ENABLE_PIN      14
#define STEPPER_STEP_PIN        15

// --------------------------------------------
//  NeoPixel
// --------------------------------------------
#define NeoPixel_PinA   21

// --------------------------------------------
//  Touch controller (I2C0 = Wire, gp4/gp5)
// --------------------------------------------
#define TOUCH_SDA   4
#define TOUCH_SCL   5
#define TOUCH_INT   3       // T_IRQ — connected but not actively used
#define TOUCH_RST   -1
#define TOUCH_WIRE  Wire

// --------------------------------------------
//  I2C storage — EEPROM (I2C1 = Wire1, gp26/gp27)
// --------------------------------------------
#define I2C_SDA     26
#define I2C_SCL     27
#define STOR_WIRE   Wire1

// --------------------------------------------
//  CAN transceiver — MCP2517/18 via SPI0
// --------------------------------------------
#define MCP2517_SPI SPI
#define MCP2517_SDO 16  // MISO from MCP2517
#define MCP2517_CS  17
#define MCP2517_SCK 18
#define MCP2517_SDI 19  // MOSI to MCP2517
#define MCP2517_INT 20

// --------------------------------------------
//  Relay / LED / accessory outputs
// --------------------------------------------
const uint8_t relay1Pin = 25;
const uint8_t relay2Pin = 25;
const uint8_t ledPin    = 25;
const uint8_t accPin    = 25;

// --------------------------------------------
//  Buttons (shared with I2C storage pins)
// --------------------------------------------
#define BLUE_BUTTON_PIN     26  // shared with I2C_SDA
#define GOLD_BUTTON_PIN     27  // shared with I2C_SCL

// --------------------------------------------
//  Display geometry & orientation
// --------------------------------------------
#define HRES            800   // landscape pixel width
#define VRES            480   // landscape pixel height
#define ROTATION          3   // TFT_eSPI rotation (3 = landscape)
#define TOUCH_ROTATION    0   // bb_captouch orientation

// --------------------------------------------
//  Display driver — fixed for v2.4 (SSD1963 8-bit parallel, TFT_eSPI_RA8876)
//  USER_SETUP_LOADED and all TFT_eSPI pin macros are set in the config below.
//    TFT_DC  = gp28  (Data/Command)
//    TFT_WR  = gp22  (Write strobe, PIO-driven)
//    TFT_D0  = gp6  .. TFT_D7 = gp13
//
//  NOTE: the matching DisplayConfig header is included by BoardSettings.h (sketch
//  root) so that "display_configs/" resolves from the correct base directory.
// --------------------------------------------
#define DISPLAY_DRIVER_SSD1963_PARALLEL

#endif  // BOARDPINS_STEPPER_V24_H
