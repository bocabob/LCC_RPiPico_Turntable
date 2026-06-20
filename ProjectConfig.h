/*
 * ProjectConfig.h — THE single file to edit when switching hardware targets.
 *
 * Step 1: Uncomment exactly ONE board line.
 * Step 1b: For LCC_BOARD_NODE_V30, also uncomment exactly ONE breakout
 *          combination in NodeConfig.h.
 * Step 2: For v2.7/v2.9/v2.95/v3.0, uncomment exactly ONE display driver line.
 *         (v2.95 has no active display — display driver lines are ignored.)
 * Step 3: Switch User_Setup_LCC_Active.h in the TFT_eSPI_RA8876 library
 *         to match (not needed for DISPLAY_DRIVER_RA8876_NATIVE, but keep
 *         it set to the RA8876 SPI setup so the library compiles correctly).
 *
 *  LCC_BOARD_STEPPER_V24   →  v2.4 board, SSD1963 8-bit parallel 800×480
 *  LCC_BOARD_STEPPER_V27   →  v2.7 board, RA8876 SPI 1024×600
 *  LCC_BOARD_STEPPER_V29   →  v2.9 board, RA8876 SPI 1024×600, CAN on gp0-4
 *  LCC_BOARD_STEPPER_V295  →  v2.95 Node board + TMC2209 breakout on I/O-2,
 *                              RA8876 display on I/O-1 (SPI1); no Blue/Gold buttons
 *  LCC_BOARD_NODE_V30      →  v3.0 generic NODE board + breakouts (STEPPER
 *                              family is legacy as of v3.0 — see
 *                              LCC_RPiPico_Common/LCC_NODE_STANDARD.md §4).
 *                              Select a breakout combo in NodeConfig.h.
 */

// ---- Step 1: Board selection -----------------------------------------------
//#define LCC_BOARD_STEPPER_V24    // v2.4 board (SSD1963 parallel)
//#define LCC_BOARD_STEPPER_V27    // v2.7 board (RA8876 SPI)
//#define LCC_BOARD_STEPPER_V29    // v2.9 board (RA8876 SPI, CAN on gp0-4)
#define LCC_BOARD_STEPPER_V295   // v2.95 Node + TMC2209 breakout on I/O-2, display on I/O-1
//#define LCC_BOARD_NODE_V30       // v3.0 generic NODE board — pick breakout combo in NodeConfig.h

// ---- Step 2: v2.7/v2.9/v2.95/v3.0 display driver (ignored for v2.4) -------
#define DISPLAY_DRIVER_RA8876_NATIVE    // native RA8876_RP2040 library (layers, BTE)
//#define DISPLAY_DRIVER_RA8876_TFTESPI // TFT_eSPI_RA8876 library
