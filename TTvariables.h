// Variables and Declarations

#ifndef VARIABLES_H
#define VARIABLES_H

#include "BoardSettings.h"

#include <SPI.h>
#include <TFT_eSPI.h>
// #include <TFT_Touch.h>
#include "src/application_drivers/my_bb_captouch.h"
// #include "TAMC_GT911.h"

// extern TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, HRES, VRES);

// extern AccelStepper stepper;

extern bool calibrating;
extern uint8_t calibrationPhase;                       // Flag for calibration phase.
extern uint8_t homed;

extern const long sanitySteps;
extern long fullTurnSteps;
extern long halfTurnSteps;
extern long phaseSwitchStartSteps;
extern long phaseSwitchStopSteps;
extern long lastTarget;
extern bool stepsSet;

extern uint16_t lastAddr;
extern uint8_t lastDirection;
extern uint8_t LastTrack;
extern uint8_t homeTrack;
extern uint8_t BridgeOrientation;
extern uint8_t CurrentTrack;
extern uint8_t BridgeRotation;

extern uint8_t trackCount;
extern uint8_t refCount;
extern uint8_t refCountDec;
extern long outside;
extern long stepAdjustment;
extern long variance;
extern long targetSave;
extern uint8_t closest;
extern uint8_t inTrack;
extern bool inBounds;
extern bool rehome;

extern bool pixelsOn;

// extern int LightPin[NumOfLights] {Light_A, Light_B};
// extern int LightAddr[NumOfLights] {700, 701};


typedef struct
{
	int address;
	long trackFront;
	long trackBack;
  bool doorPresent;
  int servoNumber;
}
TrackAddress;
// extern TrackAddress Tracks[MAX_TRACKS];

const char TrackName[MAX_TRACKS][25]=
{
	"                     ",
  "Entry Track 1  ",
  "Entry Track 2  ",
  "Entry Track 3  ",
  "Roundhouse Bay 1 ",
  "Roundhouse Bay 2 ",
  "Roundhouse Bay 3 ",
  "Roundhouse Bay 4 ",
  "Roundhouse Bay 5 ",
  "Roundhouse Bay 6 ",
  "Roundhouse Bay 7 ",
  "Roundhouse Bay 8 ",
  "Roundhouse Bay 9 ",
  "Roundhouse Bay 10",
  "Machine Shop     ",
	"                     ",
	"                     ",
	"                     ",
	"                     ",
	"                     ",
};

const char TrackTag[MAX_TRACKS][5]=
{
	" ",
  "1",
  "2",
  "3",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "10",
  "Shop",
	" ",
	" ",
	" ",
	" ",
	" ",
};

typedef struct
{
	long stepsForward;
	long stepsReverse;
}
ReferenceStep;
extern ReferenceStep References[NumberOfReferences];

// typedef struct
// {
// 	int address;
// 	bool active;
// 	int Status;
// 	int ServoMin;
// 	int ServoMax;
// 	int Position;
// }
// ServoAddress;
// extern ServoAddress Servos[i_max_servo];

typedef struct
{
	int address;
	int pin;
	bool active;
}
LightAddress;
extern LightAddress Lights[NumOfLights];

typedef struct {
  char trackName[25];        // description of this Track
  char trackShort[5];        // short description of this Track
  long steps;       // position
} tracks;
extern tracks _Tracks[MAX_TRACKS];
// Door parameters
extern uint8_t _DoorCount;    // int8 Number of Doors off turntable tracks

typedef struct {
  char doorName[16];        // description of this Door
  char doorShort[5];        // short description of this Door
  uint8_t TrackLocation;    // int8 number of the track where the door is located
} doors;
extern doors _Doors[MAX_DOORS];

#include <NeoPixelBus.h>

typedef struct {
  RgbColor color;
  // int effect;
  // bool on;
  uint8_t Rintensity;
  uint8_t Gintensity;
  uint8_t Bintensity;
  uint8_t RcyclesOn;     // cycles to be on
  uint8_t GcyclesOn;     // cycles to be on
  uint8_t BcyclesOn;     // cycles to be on
  uint8_t RcyclesOff;     // cycles to be off
  uint8_t GcyclesOff;     // cycles to be off
  uint8_t BcyclesOff;     // cycles to be off
  uint8_t Rstart;     // starting cycle = 0 or 1
  uint8_t Gstart;     // starting cycle = 0 or 1
  uint8_t Bstart;     // starting cycle = 0 or 1
  uint8_t Reffect;    // effect (0=off; 1=constant; 2= blink; 3=flicker)
  uint8_t Geffect;    // effect (0=off; 1=constant; 2= blink; 3=flicker)
  uint8_t Beffect;    // effect (0=off; 1=constant; 2= blink; 3=flicker)
  uint8_t Rgroup;
  uint8_t Ggroup;
  uint8_t Bgroup;
  bool Rstate;
  bool Gstate;
  bool Bstate;
  uint8_t Rcount;
  uint8_t Gcount;
  uint8_t Bcount;
  // uint32_t color;
  // public: static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
  // uint32_t magenta = stripA.Color(255, 0, 255);
} npHead;  //

typedef struct {           // strings
  uint8_t heads;       // number of neopixel heads
  bool ON;
  bool EffectsON;
  bool DimON;
  npHead _Head[MAX_LIGHTS];
} npStrings;
extern npStrings _Strings[MAX_STRINGS];

extern int activeScreen;
extern int editTrack;
extern int editServo;

// // Invoke custom TFT driver library
// extern TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

// /* Create an instance of the touch screen library */
// TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

typedef struct
{
	int screen;
  int X1;
	int X2;
	int Y1;
	int Y2;
}
HotBox;
// extern HotBox HotBoxes[PossibleBoxes];
extern int X_Coord;
extern int Y_Coord;

extern uint32_t box_db_time;	// Debounce time (ms) for box command processing.

/*
typedef struct _fttouchinfo
{
  int count;
  uint16_t x[5], y[5];
  uint8_t pressure[5], area[5];
} TOUCHINFO;
*/

// extern Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // 0.96 INCH OLED DISPLAY INIT
// extern Adafruit_SH1106 display(OLED_RESET); // 1.3 INCH OLED DISPLAY INIT

#endif