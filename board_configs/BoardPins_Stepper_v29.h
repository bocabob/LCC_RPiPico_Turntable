/*
 *  Pin definitions for LCC RPi Pico Stepper board v2.9
 *
 *  Do not include this file directly — it is selected automatically
 *  by BoardSettings.h based on the LCC_BOARD_* define in ProjectConfig.h.
 *
 *  Key differences from v2.7:
 *    - CAN (MCP2517) moved to SPI0 gp0-4 (freed gp16-20)
 *    - I2C storage (EEPROM) on I2C1 gp6/gp7 (was gp26/gp27)
 *    - NeoPixel moved to gp20 (was gp3)
 *    - Stepper Enable/Step/Dir moved to gp26/gp27/gp28 (freed gp2/gp6/gp7)
 *    - Sensors moved to gp18 (bridge) / gp19 (home) (were gp0/gp1)
 *    - Touch INT not connected (-1)
 *    - Display reset on gp14 (I/O-2 Pin 9)
 *    - I2C0 header (gp16/gp17) available but not used by firmware
 *    - gp5 (TT-Pin1) reserved / not yet assigned
 */

#ifndef BOARDPINS_STEPPER_V29_H
#define BOARDPINS_STEPPER_V29_H

// --------------------------------------------
//  Sensors
// --------------------------------------------
#define BRIDGE_SENSOR_PIN   18  // Bridge position sensor (TT-Pin3)
#define HOME_SENSOR_PIN     19  // Home position sensor  (TT-Pin2)

// --------------------------------------------
//  Stepper driver (A4988 / DRV8825 style)
// --------------------------------------------
#define STEPPER_INTERFACE       1
#define STEPPER_DIR_PIN         28
#define STEPPER_ENABLE_PIN      26
#define STEPPER_STEP_PIN        27

// --------------------------------------------
//  NeoPixel (TT-Pin4)
// --------------------------------------------
#define NeoPixel_PinA   20

// --------------------------------------------
//  Touch controller (I2C0 = Wire, gp12/gp13)
//  Connected via I/O-2 IDC connector pins 7/8
// --------------------------------------------
#define TOUCH_SDA   12
#define TOUCH_SCL   13
#define TOUCH_INT   -1      // not connected on v2.9
#define TOUCH_RST   -1
#define TOUCH_WIRE  Wire

// --------------------------------------------
//  I2C storage — EEPROM (I2C1 = Wire1, gp6/gp7)
// --------------------------------------------
#define I2C_SDA     6
#define I2C_SCL     7
#define STOR_WIRE   Wire1

// --------------------------------------------
//  CAN transceiver — MCP2517/18 via SPI0 (gp0-4)
//  RP2040 SPI0 alternate pin set: RX=gp0, CSn=gp1, SCK=gp2, TX=gp3
// --------------------------------------------
#define MCP2517_SPI SPI
#define MCP2517_SDO 0   // MISO from MCP2517 (ACAN_RX_PIN)
#define MCP2517_CS  1   // ACAN_CS_PIN
#define MCP2517_SCK 2   // ACAN_SCK_PIN
#define MCP2517_SDI 3   // MOSI to MCP2517  (ACAN_TX_PIN)
#define MCP2517_INT 4   // ACAN_INT_PIN

// --------------------------------------------
//  Relay / LED / accessory outputs
// --------------------------------------------
const uint8_t relay1Pin = 25;
const uint8_t relay2Pin = 25;
const uint8_t ledPin    = 25;
const uint8_t accPin    = 25;

// --------------------------------------------
//  Buttons
// --------------------------------------------
#define BLUE_BUTTON_PIN     21
#define GOLD_BUTTON_PIN     22

// --------------------------------------------
//  Reserved / not yet assigned
// --------------------------------------------
#define TT_PIN1     5   // gp5 — TT-Pin1, purpose TBD

// --------------------------------------------
//  Display geometry & orientation — TFTM101 10.1" (RA8876, 1024×600)
//  See v2.7 header for RA8876 rotation notes.
// --------------------------------------------
#define HRES           1024
#define VRES            600
#define ROTATION          0
#define TOUCH_ROTATION  180

// --------------------------------------------
//  Display — SPI1 via I/O-2 IDC connector (same as v2.7)
//    SPI1 SDI (display input)  = gp8  (I/O-2 Pin 1) — RP2040 SPI1 RX / MISO
//    SPI1 CS                   = gp9  (I/O-2 Pin 2)
//    SPI1 CLK                  = gp10 (I/O-2 Pin 3)
//    SPI1 SDO (display output) = gp11 (I/O-2 Pin 4) — RP2040 SPI1 TX / MOSI
//    Display Reset             = not connected (-1)
// --------------------------------------------
#define DISPLAY_SPI     SPI1
#define DISPLAY_SDI     8
#define DISPLAY_CS      9
#define DISPLAY_SCK     10
#define DISPLAY_SDO     11
#define DISPLAY_RST     14  // I/O-2 Pin 9

// --------------------------------------------
//  Display driver selection (same logic as v2.7)
// --------------------------------------------
#if defined(DISPLAY_DRIVER_RA8876_NATIVE)
  // Native RA8876_RP2040 library — no TFT_eSPI USER_SETUP_LOADED needed.
#else
  #if !defined(DISPLAY_DRIVER_RA8876_TFTESPI)
    #define DISPLAY_DRIVER_RA8876_TFTESPI
  #endif
#endif

#endif  // BOARDPINS_STEPPER_V29_H
