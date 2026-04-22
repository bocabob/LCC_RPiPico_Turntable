/*
 * ProjectConfig.h — Board and display driver selection
 *
 * Edit this file to match your hardware.  It is included by BoardSettings.h
 * so every translation unit (.cpp file) sees the same defines regardless of
 * what is set in the .ino file.
 *
 * Board selection — uncomment exactly one:
 *   LCC_BOARD_STEPPER_V24  : v2.4 board, SSD1963 parallel 8-bit TFT
 *   LCC_BOARD_STEPPER_V27  : v2.7 board, RA8876 SPI TFT
 *
 * Display driver (v2.7 only) — uncomment at most one:
 *   (none)                       : TFT_eSPI_RA8876 wrapper (default)
 *   DISPLAY_DRIVER_RA8876_NATIVE : native RA8876_RP2040 library
 *                                  (enables layer compositing + BTE)
 */

#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

// --------------------------------------------
//  Board selection — uncomment exactly one
// --------------------------------------------
// #define LCC_BOARD_STEPPER_V24
#define LCC_BOARD_STEPPER_V27

// --------------------------------------------
//  Display driver (v2.7 only) — uncomment to
//  use native RA8876_RP2040 library instead of
//  TFT_eSPI_RA8876 (the default).
// --------------------------------------------
#define DISPLAY_DRIVER_RA8876_NATIVE

#endif  // PROJECT_CONFIG_H
