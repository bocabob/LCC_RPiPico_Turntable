/*
 * ProjectConfig.h — THE single file to edit when switching hardware targets.
 *
 * Step 1: Uncomment exactly ONE board line.
 * Step 1b: For LCC_BOARD_NODE_V30, also uncomment exactly ONE breakout
 *          combination in NodeConfig.h.
 * Step 2: For v2.7/v2.9/v2.95/v3.0, uncomment exactly ONE display driver line.
 *         (v2.95 has no active display — display driver lines are ignored.
 *          v3.0's TURNTABLE_BREAKOUT_PARALLEL_TMC2209 combo also ignores
 *          this — it forces DISPLAY_DRIVER_SSD1963_PARALLEL_V30 itself in
 *          NodeConfig.h, since the breakout determines the driver.)
 * Step 3: Switch User_Setup_LCC_Active.h in the TFT_eSPI_RA8876 library
 *         to match (not needed for DISPLAY_DRIVER_RA8876_NATIVE, but keep
 *         it set to the RA8876 SPI setup so the library compiles correctly).
 * Step 4: For LCC_BOARD_STEPPER_V24 or LCC_BOARD_NODE_V30 with
 *         TURNTABLE_BREAKOUT_PARALLEL_TMC2209 (both use vanilla TFT_eSPI,
 *         SSD1963 parallel driver), switch the active #include line in
 *         TFT_eSPI/User_Setup_Select.h to match: Setup104a_RP2040_SSD1963_parallel.h
 *         for v2.4, Setup104b_RP2040_SSD1963_parallel.h for v3.0's parallel
 *         breakout. TFT_eSPI only reads its active setup from that file in its
 *         own library folder — a sketch-side tft_setup.h is NOT reliably
 *         picked up for every translation unit, so this manual switch is the
 *         authoritative one. (Not needed for TURNTABLE_BREAKOUT_SPI_TMC2209 —
 *         that combo uses TFT_eSPI_RA8876/Step 3 above instead.)
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
//#define LCC_BOARD_STEPPER_V295   // v2.95 Node + TMC2209 breakout on I/O-2, display on I/O-1
#define LCC_BOARD_NODE_V30       // v3.0 generic NODE board — pick breakout combo in NodeConfig.h

// ---- Step 2: v2.7/v2.9/v2.95/v3.0 display driver (ignored for v2.4 and for
//      v3.0's parallel breakout combo, which forces its own driver) --------
//#define DISPLAY_DRIVER_RA8876_NATIVE    // native RA8876_RP2040 library (layers, BTE)
//#define DISPLAY_DRIVER_RA8876_TFTESPI // TFT_eSPI_RA8876 library
