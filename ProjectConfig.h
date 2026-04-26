/*
 * ProjectConfig.h — THE single file to edit when switching hardware targets.
 *
 * Step 1: Uncomment exactly ONE board line.
 * Step 2: For v2.7, uncomment exactly ONE display driver line.
 * Step 3: Switch User_Setup_LCC_Active.h in the TFT_eSPI_RA8876 library
 *         to match (not needed for DISPLAY_DRIVER_RA8876_NATIVE, but keep
 *         it set to the RA8876 SPI setup so the library compiles correctly).
 *
 *  LCC_BOARD_STEPPER_V24  →  v2.4 board, SSD1963 8-bit parallel 800×480
 *  LCC_BOARD_STEPPER_V27  →  v2.7 board, RA8876 SPI 1024×600
 */

// ---- Step 1: Board selection -----------------------------------------------
//#define LCC_BOARD_STEPPER_V24    // v2.4 board (SSD1963 parallel)
#define LCC_BOARD_STEPPER_V27    // v2.7 board (RA8876 SPI)

// ---- Step 2: v2.7 display driver (ignored for v2.4) ------------------------
#define DISPLAY_DRIVER_RA8876_NATIVE    // native RA8876_RP2040 library (layers, BTE)
//#define DISPLAY_DRIVER_RA8876_TFTESPI // TFT_eSPI_RA8876 library
