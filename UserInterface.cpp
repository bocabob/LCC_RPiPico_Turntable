/*
 * Parts of this © 2022 Peter Cole, 2023-5 Bob Gamble
 *
 *  This file is a part of the LCC Turntable project
 *
 *  This is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  It is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  For a copy of the GNU General Public License see <https://www.gnu.org/licenses/>.
*/

#include "UserInterface.h"
#include "BoardSettings.h"
#include "TTvariables.h"
#include <NeoPixelConnect.h>

#include <SPI.h>    // Call up the TFT driver library
#include <TFT_eSPI.h>      // Hardware-specific library
// #include <TFT_Touch.h>    // Call up touch screen library
// #include <bb_captouch.h>
#include "src/application_drivers/my_bb_captouch.h"
#include <cstring>
#include "mdebugging.h"
#include "src/drivers/canbus/can_types.h"
#include "config_mem_helper.h"
#include "src/openlcb/openlcb_application_dcc_detector.h"
#include "callbacks.h"

extern openlcb_node_t *OpenLcbUserConfig_node_id;

extern config_mem_t ConfigMemHelper_config_data;

extern railcom_info_t _RailCom[MAX_TRACKS];
extern volatile bool _railcom_dirty;

volatile uint8_t _display_dirty_flags = 0;

void markBridgeDirty()    { _display_dirty_flags |= DISP_DIRTY_BRIDGE; }
void markAllTracksDirty() { _display_dirty_flags |= DISP_DIRTY_TRACKS; }

void updateDirtyDisplay() {
  if (!_display_dirty_flags) return;
  uint8_t flags = _display_dirty_flags;
  _display_dirty_flags = 0;
  if (activeScreen != 1) return;
  if (flags & DISP_DIRTY_BRIDGE) {
    drawTurnTable();
    drawTracks();
  } else if (flags & DISP_DIRTY_TRACKS) {
    drawTracks();
  }
}

// extern AccelStepper stepper;

/*
typedef struct _fttouchinfo
{
  int count;
  uint16_t x[5], y[5];
  uint8_t pressure[5], area[5];
} TOUCHINFO;
*/
TOUCHINFO ti;
TOUCHINFO  _current_state = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
TOUCHINFO  _last_state = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const char *szNames[] = {"Unknown", "FT6x36", "GT911", "CST820", "CST226", "MXT144", "AXS15231"};

BBCapTouch tp;

// #include "TAMC_GT911.h"
/*
typedef struct
{
  uint8_t id;
  uint16_t x;
  uint16_t y;
  uint8_t size;
} TP_Point;
*/

// TP_Point  _current_state = TP_Point(0,0,0,0);
// TP_Point  _last_state = TP_Point(0,0,0,0);
bool _changed = false;
uint32_t read_started_ms = millis();
uint32_t _last_change = millis();
uint32_t _db_time = DEBOUNCE_TOUCH;	// Debounce time (ms).

// Create an instance of NeoPixelConnect and initialize it
// to use GPIO pin PixelPin as the control pin, for a string
// of PixelCount neopixels. Name the instance strip

NeoPixelConnect strip(PixelPin, PixelCount, NeoPixel_PIO, 0);

bool pixelsOn = false;
int activeScreen = 0;
int editTrack = 0;
int editServo = 0;

unsigned long gearingFactor = STEPPER_GEARING_FACTOR;
// const byte numChars = 20;
// char serialInputChars[numChars];
// bool newSerialData = false;
// bool testCommandSent = false;
// uint8_t testActivity = 0;
// uint8_t testStepsMSB = 0;
// uint8_t testStepsLSB = 0;

HotBox HotBoxes[PossibleBoxes];

// Invoke custom TFT driver library
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

extern bool isStepperRunning();
extern long getCurrentPosition();
extern void runStepper();

/* Create an instance of the touch screen library */
// TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, HRES, VRES);
// TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);

// ===== Process Consumer-eventIDs =====
void TurntableCallback(uint16_t callin) {
  dP("\nEventid callback: index="); dP((uint16_t)callin); 
  uint16_t index;
  uint8_t current;
  uint8_t target;
  index = callin;
// Invoked when an event is consumed; 
// drive actions as needed from index of all events.
//
//dPS((const char*)"\npceCallback: Event Index: ", index);
// dP("\neventid callback: index="); dP((uint16_t)index);
  if (index < NUM_TABLE_EVENTS){    
        switch (index) { //
          case 0:  // CEID(Rehome)
          touchCommand(4);
          break;
          case 1:  //  CEID(IncrementTrack) 
          touchCommand(9);
          break;
          case 2:  //  CEID(DecrementTrack) 
          touchCommand(5);
          break;
          case 3:  //  CEID(RotateTrack180) 
          touchCommand(1);
          break;
          case 4:  //  CEID(ToggleBridgeLights) 
          touchCommand(2);
          break;
          default:
            // do nothing
          break;
        }
  }
 else if (index < NUM_TABLE_EVENTS + ConfigMemHelper_config_data.attributes.TrackCount * 3) {
    index = index - NUM_TABLE_EVENTS;
    // Consumer events are registered for usable tracks 1 through TrackCount (track 0
    // is the homing position and has no command events).  The registration loop uses
    // a zero-based slot offset (slot = track - 1), so slot 0 → track 1, slot N-1 →
    // track N.  Add 1 here to recover the correct 1-based track number.
    uint8_t track = index / 3 + 1;
    uint8_t outputState = index % 3;  // 0=front, 1=back, 2=occupancy, 3=railcom
    switch (outputState) {
      case 0:
      // move back side to track
        MoveToTrack(track,0);
        break;
      case 1:
      // move to track backward
        MoveToTrack(track,32);
        break;
      case 2:
        // track occupancy toggle, just redraw track with new occupancy state
        // TODO: set track occupancy state from new consumer_status[] slot that the CDI updates on occupancy events, rather than just toggling blindly on every occupancy event received.  This will ensure the display reflects actual track state even if an occupancy event is lost or received out of order.
        // if (fullTurnSteps != 0) {
        //   drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
        // }
        break;
      // case 3:
        // track railcom toggle, just redraw track with new railcom state
        // if (fullTurnSteps != 0) {
        //   drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
        // }
        // break;
    }      
      // toggle door to track w/redraw
      if (ConfigMemHelper_config_data.Tracks[track].doorPresent) 
      {
        // if (Servos[ConfigMemHelper_config_data.Tracks[track].servoNumber].Status)
        // {              MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 0);            }
        // else
        // {              MoveServo(ConfigMemHelper_config_data.Tracks[track].servoNumber, 32);            }
        if (fullTurnSteps != 0) {
          drawTrack(track,((ConfigMemHelper_config_data.Tracks[track].trackFront*360)/fullTurnSteps));
        }
      }
  }
  else {
    // Adjust index relative to the first non-track consumer event
    index = index - (NUM_TABLE_EVENTS + ConfigMemHelper_config_data.attributes.TrackCount * 3);

    // The next 3 consumer events are luminosity / bridge controls (consumed locally)
    if (index < 3) {
        switch (index) {
          case 0:  // CEID(eidBridge) – toggle bridge lights
            touchCommand(2);
            break;
          case 1:  // CEID(eidHighLuminosity_On)
            DimmerHigh();
            break;
          case 2:  // CEID(eidLowLuminosity_On)
            DimmerLow();
            break;
          default:
            break;
        }
    } else {
      // Door state sync: the Roundhouse broadcasts a "Producer Identified" for each
      // door during LCC login, carrying the NVM-restored open/closed state.
      // Mirror the received state into producer_status[] so drawDoorButton() and
      // drawTrack() reflect the correct colour without any user interaction.
      //
      // Note: _config_dirty is NOT set here.  The Turntable does not independently
      // persist door display state to its own NVM.  The Roundhouse is the single
      // authoritative source; the Turntable always re-syncs from the Roundhouse's
      // "Producer Identified" broadcasts at each LCC login.  If the Turntable
      // power-cycles while the Roundhouse is still offline, door indicators will
      // show yellow (UNKNOWN) until the Roundhouse comes back and completes login.
      int doorIdx = (int)index - 3;  // 0-based door number
      if (doorIdx >= 0 && doorIdx < ConfigMemHelper_config_data.attributes.DoorCount) {
        // Mirror Roundhouse producer state into the Turntable's producer_status[] slot
        ConfigMemHelper_config_data.producer_status[2 + doorIdx] = ConfigMemHelper_config_data.consumer_status[callin];
        // Redraw every track whose door indicator belongs to this servo
        for (int t = 1; t <= ConfigMemHelper_config_data.attributes.TrackCount; t++) {
          if (ConfigMemHelper_config_data.Tracks[t].doorPresent && ConfigMemHelper_config_data.Tracks[t].servoNumber == doorIdx) {
            if (activeScreen == 1 && fullTurnSteps != 0) {
              // Home page: radial track line
              drawTrack(t, ((ConfigMemHelper_config_data.Tracks[t].trackFront * 360) / fullTurnSteps));
            } else if (activeScreen == 2) {
              // Button page: coloured door button
              drawDoorButton(t);
            }
          }
        }
      }
    }
  }
}


void setupDisplay()
{  
  tft.init();
  tft.setRotation(ROTATION);   // Set TFT the screen to landscape orientation
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TC_DATUM);  // Set text plotting reference datum to Top Centre (TC)
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Set text to white on black

  // calibration values from prior run of the display calibration library example
// touch.setCal(3668, 365, 3422, 258, 480, 320, 1);
// touch.setCal(3671, 356, 3459, 297, 480, 320, 1);

  // tp.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT);
  // int iType = tp.sensorType();
  // Serial.printf("Sensor type = %s\n", szNames[iType]);

  tp.init(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, TOUCH_INT, 400000, &TOUCH_WIRE);
  int iType = tp.sensorType();
  Serial.printf("Sensor type = %s\n", szNames[iType]);

  Serial.println("bb_captouch Touch: Ready");
  // tp.begin();
  
    // int setOrientation(int iOrientation, int iWidth, int iHeight);
  int result = tp.setOrientation(TOUCH_ROTATION, HRES, VRES);

  // pinMode(TFT_BL, OUTPUT);    // lcd light
  // digitalWrite(TFT_BL, LOW);
  // digitalWrite(TFT_BL, HIGH);

  Serial.println(F("Setup Display"));  
  tft.setCursor(0, 0, 4);
  tft.println(F("LCC Turntable Control"));  
  tft.setCursor(50, 50, 2);
  tft.println("");
  tft.print(F("Version "));
  tft.println(VERSION);
  tft.print(F("by Bob Gamble"));

  String text;
  text+= "Screen rotation = ";
  text+= tft.getRotation();
  char buffer[30];
  text.toCharArray(buffer,30);
  tft.drawString(buffer, 350, 250, 2);

  activeScreen = 0;

  for (int i = 0; i < (sizeof(HotBoxes) / sizeof(HotBox)); i++) {
    HotBoxes[i].screen = 0;
    HotBoxes[i].X1 = 0;
    HotBoxes[i].X2 = 0;
    HotBoxes[i].Y1 = 0;
    HotBoxes[i].Y2 = 0;
  }

  Serial.println(F("<<< Display READY >>>"));
// write_to_display();
}

void displayConfig() {  // Function to display the defined stepper motor config.
  // Basic setup, display what this is.
  // Serial.begin(115200);
  // while(!Serial);
  Serial.print(F("LCC Turntable version "));
  Serial.println(VERSION);
  if (fullTurnSteps == 0) {
    Serial.println(F("LCC-Turntable has not been calibrated yet"));
  } else {
#ifdef FULL_STEP_COUNT    
    Serial.print(F("Manual override has been set for "));
#else
    Serial.print(F("LCC-Turntable has been calibrated for "));
#endif
    Serial.print(fullTurnSteps);
    Serial.println(F(" steps per revolution"));
  }
  Serial.print(F("Gearing factor set to "));
  Serial.println(gearingFactor);
  
  if (calibrating) {
    Serial.println(F("Calibrating..."));
  } else {
    if (homed == 0 ){
      Serial.println(F("Homing..."));
    }
  }

}

void notice(const char *string)
{
  NOTICE_PRINT.println(string);
}

void drawButton(int box, int length, int32_t x, int32_t y, uint32_t fg_color,const char *string)
{
// Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
// The outer corner radius is r, inner corner radius is ir
// The inside and outside of the border are anti-aliased
// drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF, uint8_t quadrants = 0xF);
tft.setTextDatum(TC_DATUM);  // Set text plotting reference datum to Top Centre (TC)
tft.setTextPadding(tft.textWidth(string, 2)); // get the width of the text in pixels
tft.drawSmoothRoundRect( x,  y,  10,  9,  length,  24,  fg_color, TFT_BLACK, 0xF);
setHotSpot(box, x, y, x + length, y + 23);
tft.drawString(string, x+(length/2), y+4, 2);
tft.setTextPadding(0); // set to zero
}

void drawFastClock(int hour, int minute)
{
  tft.setTextDatum(ML_DATUM);  // Set text plotting reference datum to Top Centre (TC)
  // tft.setTextPadding(100); // get the width of the text in pixels
  // tft.drawString("Clock", HRES/2, VRES/2, 4);
  // tft.setTextPadding(0); // set to zero
  
      //   broadcast_clock_state_t* clock = OpenLcbApplicationBroadcastTime_get_clock(BROADCAST_TIME_ID_DEFAULT_FAST_CLOCK);
      //   // clock->is_running = true;
      //   printf("Current time: %02d:%02d\n", clock->time.hour, clock->time.minute);  
  switch (activeScreen)
  {
  case  1:
  {
    String TimeText;
    TimeText+= "Fast Clock: ";
    TimeText+= hour;
    TimeText+= ":";
    if (minute < 10) {
      TimeText+= "0"; // Add leading zero for minutes less than 10
    }
    TimeText+= minute;
    char buffer[30];
    TimeText.toCharArray(buffer,30);
    tft.drawString(buffer, 5, (VRES/2)-30, 4);

    // tft.setCursor(HRES/8, (VRES/2)-30);
    // tft.print(hour);
    // tft.print(":");
    // if (minute < 10) {
    //   tft.print("0"); // Add leading zero for minutes less than 10
    // }
    // tft.print(minute);
    break;
  }
  case 2:
    tft.setCursor(HRES/2, (VRES-30));
    tft.print(hour);
    tft.print(":");
    if (minute < 10) {
      tft.print("0"); // Add leading zero for minutes less than 10
    }
    tft.print(minute);
    break;
  default:
    break;
  }
}

void drawHomePage()
{
  activeScreen = 1;
  /*
  main page from which to operate
  shows turntable with tracks and doors
  operational buttons for main TT functions and lights
  links to configuration settings and diagnostic reports
  */
  int butLen = 100;
  int Lcol = 5;
  int Rcol = HRES - butLen - 5;
  int vSpace = 40;
  int row = 5;
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  drawButton(13,butLen,Lcol,row,TFT_GREEN,"Diagnostics"); // go to diagnostics page
  // drawButton(12,butLen,Lcol,row,TFT_GREEN,"Settings");  // go to settings page
  row = row + vSpace;
  drawButton(18,butLen,Lcol,row,TFT_GREEN,"Button Page"); // go to button page
  // drawButton(18,butLen,Lcol,row,TFT_GREEN,"Configuration"); // go to config page
  row = row + vSpace;
  // drawButton(3,90,Lcol,row,TFT_GREEN,"Configure");  // run configuration routine
  row = row + vSpace;
  drawButton(4,butLen,Lcol,row,TFT_GREEN,"Re-Home");  // run find home routine
  row = row + vSpace*5;
  drawButton(5,butLen,Lcol,row,TFT_GREEN,"Decrement");  // move one track back
  drawButton(9,butLen,Rcol,row,TFT_GREEN,"Increment");  // move to next track
  row = 5;
  drawButton(7,butLen,Rcol,row,TFT_GREEN,"RH Interior");  // toggle RH interior light
  row = row + vSpace;
  drawButton(8,butLen,Rcol,row,TFT_GREEN,"RH Exterior");  // toggle RH exterior light
  row = row + (vSpace * 2);
  drawButton(10,butLen,Rcol,row,TFT_GREEN,"Open All");  // open all doors
  row = row + vSpace;
  drawButton(11,butLen,Rcol,row,TFT_GREEN,"Close All"); // close all doors
  row = VRES - vSpace;
  drawButton(6,250,((HRES/2)-375),row,TFT_GREEN,"Clockwise Bump Bar");  // execute bump bar routine
  drawButton(6,250,((HRES/2)+125),row,TFT_GREEN,"Counterclockwise Bump Bar");  // execute bump bar routine
  setHotSpot(6, 1, row, HRES - 2, row + 23);

  drawTurnTable();
}



void drawButtonPage()
{
  activeScreen = 2;
  int butLen = 90;
  int col = 5;
  int hSpace = (HRES / 6);
  int vSpace = 40;
  int row = VRES - (vSpace * 3);
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  drawButton(14,butLen,col,row,TFT_GREEN,"TurnTable");  // go to home page
  // col = col + hSpace;
  // drawButton(12,butLen,col,row,TFT_GREEN,"Settings");  // go to settings page
  // col = col + hSpace;
  // drawButton(18,butLen,col,row,TFT_GREEN,"Configuration"); // go to config page
  col = col + hSpace;  
  drawButton(4,butLen,col,row,TFT_GREEN,"Re-Home");  // run find home routine
  col = col + 2*hSpace;  
  drawButton(9,butLen,col,row,TFT_GREEN,"Increment");  // move to next track
  col = col + hSpace;  
  drawButton(10,butLen,col,row,TFT_GREEN,"Open All");  // open all doors
  col = col + hSpace;  
  drawButton(8,butLen,col,row,TFT_GREEN,"RH Exterior");  // toggle RH exterior light

  row = VRES - (vSpace * 2);
  col = 5;
  drawButton(13,butLen,col,row,TFT_GREEN,"Diagnostics"); // go to diagnostics page
  col = col + hSpace;    
  // drawButton(3,90,col,row,TFT_GREEN,"Configure");  // run configuration routine
  col = col + 2*hSpace;  
  drawButton(5,butLen,col,row,TFT_GREEN,"Decrement");  // move one track back
  col = col + hSpace;  
  drawButton(11,butLen,col,row,TFT_GREEN,"Close All"); // close all doors
  col = col + hSpace; 
  drawButton(7,butLen,col,row,TFT_GREEN,"RH Interior");  // toggle RH interior light
  row = VRES - vSpace; 
  drawButton(6,250,((HRES/2)-375),row,TFT_GREEN,"Clockwise Bump Bar");  // execute bump bar routine
  drawButton(6,250,((HRES/2)+125),row,TFT_GREEN,"Counterclockwise Bump Bar");  // execute bump bar routine
  setHotSpot(6, 1, row, HRES - 2, row + 23);

  butLen = 60;
  // col = 10;
  row = 10;
  int box = TrackBox;
  // char buttonText[12] = "";
  // tft.setTextColor(TFT_BLUE);
  tft.setTextDatum(MC_DATUM);  // Set text plotting reference datum to Middle Center 
  tft.setTextPadding(tft.textWidth("99", 2)); // get the width of the text in pixels
  // tft.drawString(" ", 0, 0, 2); 
  // tft.setTextColor(TFT_WHITE);
  for (int track = 1; track <= ConfigMemHelper_config_data.attributes.TrackCount; track++) {
    box = (TrackBox +(track*3));
    row = 1 + ((track-1) % 9) * 40;
    col = 5 + (HRES / 2) * (int)((track-1) / 9);
    tft.drawString(ConfigMemHelper_config_data.attributes.tracks[track].trackName, col+60, row+6, 2);  // draw the name of the track
    // tft.drawString(TrackTag[track], col+15, row, 2);  // draw the tag of the track
    col = col + 140;
    // buttonText[12] = '\0'; // null terminate the string
    // strcat(buttonText, TrackTag[track]); 
    // strcat(buttonText,"Front");
    drawButton(box,butLen,col,row,TFT_BLUE,"Front");  // track button
    // buttonText[12] = '\0'; // null terminate the string
    // strcat(buttonText, TrackTag[track]); 
    // strcat(buttonText,"Back");
    drawButton(box+1,butLen,col+butLen+10,row,TFT_YELLOW,"Back");  // track button
    if (ConfigMemHelper_config_data.Tracks[track].doorPresent) {
      drawDoorButton(track);  // door button
    }    
  }
}

void drawDoorButton(int track)
{ 
  int butLen = 60;
  int box = (TrackBox +(track*3));
  int row = 1 + ((track-1) % 9) * 40;
  int col = 145 + (HRES / 2) * (int)((track-1) / 9)+butLen+butLen+20;
  
  switch (ConfigMemHelper_config_data.producer_status[2+ConfigMemHelper_config_data.Tracks[track].servoNumber]) {

    case EVENT_STATUS_UNKNOWN:
      drawButton(box+2,butLen,col,row,TFT_YELLOW,"Door");  // door button 
      break;

    case EVENT_STATUS_SET:
      drawButton(box+2,butLen,col,row,TFT_RED,"Door");  // door button
      break;

    case EVENT_STATUS_CLEAR:
      drawButton(box+2,butLen,col,row,TFT_GREEN,"Door");  // door button
      break;
  }
    
}

void drawDiagnosticPage()
{
  activeScreen = 4;
  int butLen = 90;
  int hSpace = (HRES / 6);
  int col = 10;
  int vSpace = 40;
  int row = VRES - (vSpace * 1);
  // Clear the screen
  tft.fillScreen(TFT_BLACK);
  drawButton(12,butLen,col,row,TFT_GREEN,"Reset Defaults");  // go to settings page
  col = col + hSpace;
  drawButton(3,butLen,col,row,TFT_GREEN,"Configure");  // run configuration routine
  col = col + hSpace*2;
  drawButton(14,butLen,col,row,TFT_GREEN,"TurnTable");  // go to home page
  col = col + hSpace;
  drawButton(18,butLen,col,row,TFT_GREEN,"Buttons"); // go to config page

  drawDiagnostics();
  listServos(0);
  listReverences(0);
  tft.setCursor((HRES/2)+50, 0, 2);
  drawTrackMatrix((HRES/2)+50);
}


void drawDiagnostics()
{
  tft.setTextPadding(0); // reset to zero
  tft.setTextDatum(TL_DATUM); // Top Left is datum
   
  tft.setCursor(0, 0, 2);
  tft.print(F("LocoNet Turntable Control version ")); 
  tft.println(VERSION);
  
  tft.print(F("Full Turn Steps = ")); 
  tft.println(fullTurnSteps);
  
  tft.print(F("Last Track set = ")); 
  tft.println(LastTrack);
  
  tft.print(F("Current Track is  ")); 
  tft.print(TrackName[ConfigMemHelper_config_data.CurrentTrack]);
  tft.print(F(" # "));
  tft.print(ConfigMemHelper_config_data.CurrentTrack);
  tft.print(F(" @ step  ")); 
  tft.println(getCurrentPosition());
  
  tft.print(F("Home Track is  ")); 
  tft.print(TrackName[homeTrack]);
  tft.print(F(" # "));
  tft.println(homeTrack);
  
  tft.print(F("Number of tracks set is  ")); 
  tft.println(ConfigMemHelper_config_data.attributes.TrackCount);  

  tft.print(F("Number of reference points detected is  ")); 
  tft.println(ConfigMemHelper_config_data.attributes.ReferenceCount);  
  
}

void drawTrackMatrix(int col)
{
  
  tft.setTextDatum(TL_DATUM); // Top Left is datum
  tft.println(F("Track Settings ")); 
  tft.println(F("Address   Front   Back   Door Present   Door Servo")); 
  tft.setCursor(col+5, tft.getCursorY()+10);
  tft.setTextDatum(TR_DATUM); // Top Right is datum, so decimal point stays in same place
  // for (int i = 0; i < (sizeof(ConfigMemHelper_config_data.Tracks) / sizeof(TrackAddress)); i++) {
  for (int i = 0; i <= (ConfigMemHelper_config_data.attributes.TrackCount); i++) {
  tft.setCursor(col+15, tft.getCursorY());
  tft.print(ConfigMemHelper_config_data.Tracks[i].address );
  tft.setCursor(col+70,tft.getCursorY());
  tft.print(ConfigMemHelper_config_data.Tracks[i].trackFront);
  tft.setCursor(col+140,tft.getCursorY());
  tft.print(ConfigMemHelper_config_data.Tracks[i].trackBack);
  tft.setCursor(col+210,tft.getCursorY());
  tft.print(ConfigMemHelper_config_data.Tracks[i].doorPresent);
  tft.setCursor(col+280,tft.getCursorY());
  tft.println(ConfigMemHelper_config_data.Tracks[i].servoNumber);
    }
}

void listServos(int col)
{
  tft.setTextDatum(TL_DATUM); // Top Left is datum
  tft.setCursor(col, tft.getCursorY());
  tft.println(F("Servo Settings ")); 
  tft.setCursor(col, tft.getCursorY());
  tft.println(F("Address   Minimum   Maximum   Door position")); 
  tft.setTextDatum(TR_DATUM); // Top Right is datum, so decimal point stays in same place
  // for (int i = 0; i < (sizeof(Servos) / sizeof(ServoAddress)); i++) {
  // tft.setCursor((HRES/2)+15, tft.getCursorY());
  // tft.print(Servos[i].address );
  // tft.setCursor((HRES/2)+70,tft.getCursorY());
  // tft.print(Servos[i].ServoMin);
  // tft.setCursor((HRES/2)+140,tft.getCursorY());
  // tft.print(Servos[i].ServoMax);
  // tft.setCursor((HRES/2)+210,tft.getCursorY());
  // tft.println(Servos[i].Position);
  //   }
}

void listReverences(int col)
{
  tft.setTextDatum(TL_DATUM); // Top Left is datum
  tft.setCursor(col, tft.getCursorY()+10);
  tft.println(F("Reference Points ")); 
  tft.setCursor(col, tft.getCursorY());
  tft.println(F("Number  Forward steps   Reverse steps")); 
  tft.setTextDatum(TR_DATUM); // Top Right is datum, so decimal point stays in same place
  for (int i = 0; i < (sizeof(References) / sizeof(ReferenceStep)); i++) {
  tft.setCursor(col+25, tft.getCursorY());
  tft.print(i);
  tft.setCursor(col+70,tft.getCursorY());
  tft.print(ConfigMemHelper_config_data.References[i].stepsForward);
  tft.setCursor(col+180,tft.getCursorY());
  tft.println(ConfigMemHelper_config_data.References[i].stepsReverse);
    }
}

void drawTurnTable()
{
  int32_t xC = TT_CX, yC = TT_CY, rad = TT_RAD;
  uint32_t  fg_color,  bg_color;
  fg_color = TFT_WHITE;
  bg_color = TFT_BLACK;

  tft.fillCircle(TT_CX, TT_CY, (TT_RAD+(TT_S_SPACE*2)+TT_S_WIDTH + 5), TFT_BLACK);
  // drawSmoothCircle(int32_t x, int32_t y, int32_t r, uint32_t fg_color, uint32_t bg_color);
  tft.drawSmoothCircle(xC, yC, rad, fg_color,  bg_color);
  // tft.drawCircle(xC, yC, rad+1, TFT_WHITE);

  if (fullTurnSteps != 0) {
    drawBridge((absPosition(getCurrentPosition())*360)/fullTurnSteps);
  }
}


void drawBridge(float angle)
{
  angle = angle + TT_ROTATION - 180;

int32_t xC = TT_CX, yC = TT_CY, offset = TT_B_WIDTH, side = TT_B_WIDTH*2;
tft.fillCircle(TT_CX, TT_CY, (TT_RAD-2), TFT_BLACK);

float length = TT_DIA - 25, width = TT_B_WIDTH;
float CosAngle = - cos(angle*0.01745329252);
float SineAngle = sin(angle*0.01745329252);
float CPtBackX = xC - (CosAngle * length / 2);
float CPtBackY = yC - (SineAngle * length / 2);
float CPtFrontX = xC + (CosAngle * length / 2);
float CPtFrontY = yC + (SineAngle * length / 2);

float BPt1X = CPtBackX + (SineAngle * width / 2);
float BPt1Y = CPtBackY - (CosAngle * width / 2);
float BPt2X = CPtBackX - (SineAngle * width / 2);
float BPt2Y = CPtBackY + (CosAngle * width / 2);
float BPt3X = CPtFrontX - (SineAngle * width / 2);
float BPt3Y = CPtFrontY + (CosAngle * width / 2);
float BPt4X = CPtFrontX + (SineAngle * width / 2);
float BPt4Y = CPtFrontY - (CosAngle * width / 2);

setHotSpot8(1,BPt1X, BPt1Y, BPt2X, BPt2Y,BPt3X, BPt3Y, BPt4X, BPt4Y);
// Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
// If bg_color is not included the background pixel colour will be read from TFT or sprite
//tft.drawWideLine(float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color = 0x00FFFFFF);
// draw bridge ends
tft.drawWideLine(BPt1X, BPt1Y, BPt2X, BPt2Y, 8, TFT_BLUE);
tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 8, TFT_YELLOW);
// draw bridge section
length = TT_DIA - 40;
 CPtBackX = xC - (CosAngle * length / 2);
 CPtBackY = yC - (SineAngle * length / 2);
 CPtFrontX = xC + (CosAngle * length / 2);
 CPtFrontY = yC + (SineAngle * length / 2);

 BPt1X = CPtBackX + (SineAngle * width / 2);
 BPt1Y = CPtBackY - (CosAngle * width / 2);
 BPt2X = CPtBackX - (SineAngle * width / 2);
 BPt2Y = CPtBackY + (CosAngle * width / 2);
 BPt3X = CPtFrontX - (SineAngle * width / 2);
 BPt3Y = CPtFrontY + (CosAngle * width / 2);
 BPt4X = CPtFrontX + (SineAngle * width / 2);
 BPt4Y = CPtFrontY - (CosAngle * width / 2);
 switch (ConfigMemHelper_config_data.consumer_status[NUM_EVENT])
 {
    case EVENT_STATUS_UNKNOWN:
      tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 4, RailColor);
      tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 4, RailColor);
      break;

    case EVENT_STATUS_SET:
      tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 4, TFT_RED);
      tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 4, TFT_RED);
      break;

    case EVENT_STATUS_CLEAR:
      tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 4, TFT_GREEN);
      tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 4, TFT_GREEN);
      break;
  }

// draw track occupancy and railcom info
char buf[20];
  if (_RailCom[0].occupied) {
      // e.g., draw "Loco 42 (Fwd)" next to the track label
      sprintf(buf, "%d%s",
          _RailCom[0].dcc_address,
          _RailCom[0].direction == dcc_detector_occupied_forward ? " Fwd" :
          _RailCom[0].direction == dcc_detector_occupied_reverse ? " Rev" : "");
  } else {
      sprintf(buf, "---");  // track empty
  }
  tft.setTextDatum(MC_DATUM);  // Set text plotting reference datum to Middle Center 
  tft.setTextPadding(tft.textWidth("999999999", 2)); // get the width of the text in pixels
  tft.drawString(buf, xC, yC, 2);  // draw the known loco on the track if occupied, or "---" if empty

float CPtShackX = BPt1X + (CosAngle * offset);
float CPtShackY = BPt1Y + (SineAngle * offset);
float SPt1X = CPtShackX + (SineAngle * side);
float SPt1Y = CPtShackY - (CosAngle * side);
float SPt2X = SPt1X + (CosAngle * side);
float SPt2Y = SPt1Y + (SineAngle * side);
float SPt3X = CPtShackX + (CosAngle * side);
float SPt3Y = CPtShackY + (SineAngle * side);
 
// draw bridge shack


  if (fullTurnSteps != 0) {
    drawShack((absPosition(getCurrentPosition())*360)/fullTurnSteps);
  }

// tft.drawWideLine(CPtShackX, CPtShackY, SPt1X, SPt1Y, 2, TFT_RED);
// tft.drawWideLine(SPt1X, SPt1Y, SPt2X, SPt2Y, 2, TFT_RED);
// tft.drawWideLine(SPt2X, SPt2Y, SPt3X, SPt3Y, 2, TFT_RED);
// tft.drawWideLine(SPt3X,SPt3Y,CPtShackX, CPtShackY, 2, TFT_RED);
// if (pixelsOn) tft.fillCircle((SPt1X+SPt3X)/2, (SPt1Y+SPt3Y)/2, TT_B_WIDTH/2, TFT_YELLOW);

setHotSpot8(2,CPtShackX, CPtShackY, SPt1X, SPt1Y,SPt2X, SPt2Y, SPt3X, SPt3Y);


tft.setTextDatum(ML_DATUM);  // Set text plotting reference datum to Middle Left 
tft.setTextPadding(tft.textWidth("9999999999999999999999999", 2)); // get the width of the text in pixels
tft.drawString(TrackName[ConfigMemHelper_config_data.CurrentTrack], 80, VRES/2, 2);  // draw the name of the track

}

void drawShack(float angle)
{
  angle = angle + TT_ROTATION - 180;

int32_t xC = TT_CX, yC = TT_CY, offset = TT_B_WIDTH, side = TT_B_WIDTH*2;

float length = TT_DIA - 40, width = TT_B_WIDTH;
float CosAngle = - cos(angle*0.01745329252);
float SineAngle = sin(angle*0.01745329252);
float CPtBackX = xC - (CosAngle * length / 2);
float CPtBackY = yC - (SineAngle * length / 2);
float BPt1X = CPtBackX + (SineAngle * width / 2);
float BPt1Y = CPtBackY - (CosAngle * width / 2);
float CPtShackX = BPt1X + (CosAngle * offset);
float CPtShackY = BPt1Y + (SineAngle * offset);
float SPt1X = CPtShackX + (SineAngle * side);
float SPt1Y = CPtShackY - (CosAngle * side);
float SPt2X = SPt1X + (CosAngle * side);
float SPt2Y = SPt1Y + (SineAngle * side);
float SPt3X = CPtShackX + (CosAngle * side);
float SPt3Y = CPtShackY + (SineAngle * side);
 
// draw bridge shack
tft.drawWideLine(CPtShackX, CPtShackY, SPt1X, SPt1Y, 2, TFT_RED);
tft.drawWideLine(SPt1X, SPt1Y, SPt2X, SPt2Y, 2, TFT_RED);
tft.drawWideLine(SPt2X, SPt2Y, SPt3X, SPt3Y, 2, TFT_RED);
tft.drawWideLine(SPt3X,SPt3Y,CPtShackX, CPtShackY, 2, TFT_RED);
if (pixelsOn) 
  tft.fillCircle((SPt1X+SPt3X)/2, (SPt1Y+SPt3Y)/2, TT_B_WIDTH/2, TFT_YELLOW);
  else
  tft.fillCircle((SPt1X+SPt3X)/2, (SPt1Y+SPt3Y)/2, TT_B_WIDTH/2, TFT_BLACK);

}

void drawTracks()
{
    _railcom_dirty = false;

  for (int i = 1; i <= ConfigMemHelper_config_data.attributes.TrackCount; i++) {
    if (fullTurnSteps != 0) {
      drawTrack(i, ((ConfigMemHelper_config_data.Tracks[i].trackFront*360)/fullTurnSteps));
    }
    // Serial.print(i);
    // Serial.print("  ");
    // Serial.print((ConfigMemHelper_config_data.Tracks[i].trackFront*360)/fullTurnSteps);
    // Serial.print(" degrees ");
    // Serial.print(ConfigMemHelper_config_data.Tracks[i].trackFront);
    // Serial.println(" steps");
  }
}

void drawTrack(int track, float angle)
//------------------------------------------
{
  angle = angle + TT_ROTATION - 180;

  int32_t xC = TT_CX, yC = TT_CY, offset = TT_S_SPACE;
  int box = (TrackBox +(track*3));
  float length = TT_DIA + TT_S_SPACE/2, width = TT_S_WIDTH;

  float CosAngle = cos(angle*0.01745329252);
  float SineAngle = - sin(angle*0.01745329252);


  length = TT_DIA;
  float CPtBackX = xC + (CosAngle * (length+(offset*0)) / 2);
  float CPtBackY = yC + (SineAngle * (length+(offset*0)) / 2);

  float BPt1X = CPtBackX + (SineAngle * width / 2);
  float BPt1Y = CPtBackY - (CosAngle * width / 2);
  float BPt2X = CPtBackX - (SineAngle * width / 2);
  float BPt2Y = CPtBackY + (CosAngle * width / 2);

  float CPtFrontX = xC + (CosAngle * (length+(offset*4)) / 2);
  float CPtFrontY = yC + (SineAngle * (length+(offset*4)) / 2);

  float BPt3X = CPtFrontX - (SineAngle * width / 2);
  float BPt3Y = CPtFrontY + (CosAngle * width / 2);
  float BPt4X = CPtFrontX + (SineAngle * width / 2);
  float BPt4Y = CPtFrontY - (CosAngle * width / 2);

  switch (ConfigMemHelper_config_data.consumer_status[NUM_TABLE_EVENTS + (track-1)*3 + 2])
  {
      case EVENT_STATUS_UNKNOWN:
        tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 2, RailColor);
        tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 2, RailColor);
        break;

      case EVENT_STATUS_SET:
        tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 2, TFT_RED);
        tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 2, TFT_RED);
        break;

      case EVENT_STATUS_CLEAR:
        tft.drawWideLine(BPt2X, BPt2Y, BPt3X, BPt3Y, 2, TFT_GREEN);
        tft.drawWideLine(BPt4X, BPt4Y, BPt1X, BPt1Y, 2, TFT_GREEN);
        break;
    }

  float CPtMidX = xC + (CosAngle * (length+(offset*.6)) / 2);
  float CPtMidY = yC + (SineAngle * (length+(offset*.6)) / 2);

  // tft.setTextColor(TFT_BLUE);
  tft.setTextDatum(MC_DATUM);  // Set text plotting reference datum to Middle Center 
  tft.setTextPadding(tft.textWidth("99", 2)); // get the width of the text in pixels
  tft.drawString(ConfigMemHelper_config_data.attributes.tracks[track].trackShort, CPtMidX, CPtMidY, 2);  // draw the short name of the track
  // tft.setTextColor(TFT_WHITE);

  // front position button (blue)
  length = TT_DIA +(offset*1.4);

  CPtFrontX = xC + (CosAngle * length / 2);
  CPtFrontY = yC + (SineAngle * length / 2);

  BPt3X = CPtFrontX - (SineAngle * width / 2);
  BPt3Y = CPtFrontY + (CosAngle * width / 2);
  BPt4X = CPtFrontX + (SineAngle * width / 2);
  BPt4Y = CPtFrontY - (CosAngle * width / 2);
  tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_BLUE);
  setHotSpot8(box, BPt3X, BPt3Y, BPt4X, BPt4Y, BPt3X, BPt3Y, BPt4X, BPt4Y);

  // back position button (yellow)
  length = TT_DIA +(offset*2.7);
  CPtFrontX = xC + (CosAngle * (length) / 2);
  CPtFrontY = yC + (SineAngle * (length) / 2);

  BPt3X = CPtFrontX - (SineAngle * width / 2);
  BPt3Y = CPtFrontY + (CosAngle * width / 2);
  BPt4X = CPtFrontX + (SineAngle * width / 2);
  BPt4Y = CPtFrontY - (CosAngle * width / 2);
  tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_YELLOW);
  setHotSpot8(box+1, BPt3X, BPt3Y, BPt4X, BPt4Y, BPt3X, BPt3Y, BPt4X, BPt4Y);

  // door position button (state determined)
  length = TT_DIA +(offset*4);
  CPtFrontX = xC + (CosAngle * (length) / 2);
  CPtFrontY = yC + (SineAngle * (length) / 2);
  BPt3X = CPtFrontX - (SineAngle * width / 2);
  BPt3Y = CPtFrontY + (CosAngle * width / 2);
  BPt4X = CPtFrontX + (SineAngle * width / 2);
  BPt4Y = CPtFrontY - (CosAngle * width / 2);

  _RailCom[track].dirty = false;
  char buf[20];
  if (_RailCom[track].occupied) {
      // e.g., draw "Loco 42 (Fwd)" next to the track label
      sprintf(buf, "%d%s",
          _RailCom[track].dcc_address,
          _RailCom[track].direction == dcc_detector_occupied_forward ? " Fwd" :
          _RailCom[track].direction == dcc_detector_occupied_reverse ? " Rev" : "");
  } else {
      sprintf(buf, "---");  // track empty
  }
  tft.setTextDatum(MR_DATUM);  // Set text plotting reference datum to Middle Right 
  tft.setTextPadding(tft.textWidth("9999", 2)); // get the width of the text in pixels
  tft.drawString(buf, BPt4X, BPt3Y, 2);  // draw the known loco on the track if occupied, or "---" if empty
  
  if (ConfigMemHelper_config_data.Tracks[track].doorPresent) {

// need to integrate with door state determined by events consumed from other node

    switch (ConfigMemHelper_config_data.producer_status[2+ConfigMemHelper_config_data.Tracks[track].servoNumber]) {

      case EVENT_STATUS_UNKNOWN:
        tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_YELLOW);
        break;

      case EVENT_STATUS_SET:
        tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_RED);
        break;

      case EVENT_STATUS_CLEAR:
        tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_GREEN);
        break;
    }
    
    setHotSpot8(box+2, BPt3X, BPt3Y, BPt4X, BPt4Y, BPt3X, BPt3Y, BPt4X, BPt4Y);
  }
// else tft.drawWideLine(BPt3X, BPt3Y, BPt4X, BPt4Y, 9, TFT_BLACK); 

}

void setHotSpot(int box, int X1, int Y1, int X2, int Y2)
{ // sets a hotspot given two opposing points
  HotBoxes[box].screen = activeScreen;
  if (X1<X2)
  {
    HotBoxes[box].X1 = X1-T_FRINGE;
    HotBoxes[box].X2 = X2+T_FRINGE;
  }
  else
  {
    HotBoxes[box].X1 = X2-T_FRINGE;
    HotBoxes[box].X2 = X1+T_FRINGE;
  }
  if (Y1<Y2)
  {
    HotBoxes[box].Y1 = Y1-T_FRINGE;
    HotBoxes[box].Y2 = Y2+T_FRINGE;
  }
  else
  {
    HotBoxes[box].Y1 = Y2-T_FRINGE;
    HotBoxes[box].Y2 = Y1+T_FRINGE;
  }
}

void setHotSpot8(int box, int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4)
{ // sets a hotspot for a screen rectangle that encloses four points
  int tol = T_FRINGE;  
  
    // Serial.print("Box: ");
    // Serial.println(box);

  HotBoxes[box].screen = activeScreen;
  if ((X1<=X2) && (X1<=X3) && (X1<=X4))
  {
    HotBoxes[box].X1 = X1 - tol;
  }
  else if ((X2<=X1) && (X2<=X3) && (X2<=X4)) {
    HotBoxes[box].X1 = X2 - tol;
    }
    else if ((X3<=X1) && (X3<=X2) && (X3<=X4)) {
      HotBoxes[box].X1 = X3 + tol;
      }
      else{
      HotBoxes[box].X1 = X4 - tol;
      }

  if ((Y1<=Y2) && (Y1<=Y3) && (Y1<=Y4))
  {
    HotBoxes[box].Y1 = Y1 - tol;
  }
  else if ((Y2<=Y1) && (Y2<=Y3) && (Y2<=Y4)) {
    HotBoxes[box].Y1 = Y2 - tol;
    }
    else if ((Y3<=Y1) && (Y3<=Y2) && (Y3<=Y4)) {
      HotBoxes[box].Y1 = Y3 - tol;
      }
      else{
      HotBoxes[box].Y1 = Y4 - tol;
      }
    
  if ((X1>=X2) && (X1>=X3) && (X1>=X4))
  {
    HotBoxes[box].X2 = X1 + tol;
  }
  else if ((X2>=X1) && (X2>=X3) && (X2>=X4)) {
    HotBoxes[box].X2 = X2 + tol;
    }
    else if ((X3>=X1) && (X3>=X2) && (X3>=X4)) {
      HotBoxes[box].X2 = X3 + tol;
      }
      else{
      HotBoxes[box].X2 = X4 + tol;
      }
    
  if ((Y1>=Y2) && (Y1>=Y3) && (Y1>=Y4))
  {
    HotBoxes[box].Y2 = Y1 + tol;
  }
  else if ((Y2>=Y1) && (Y2>=Y3) && (Y2>=Y4)) {
    HotBoxes[box].Y2 = Y2 + tol;
    }
    else if ((Y3>=Y1) && (Y3>=Y2) && (Y3>=Y4)) {
      HotBoxes[box].Y2 = Y3 + tol;
      }
      else{
      HotBoxes[box].Y2 = Y4 + tol;
      }
    
}

// typedef struct _fttouchinfo
// {
//   int count;
//   uint16_t x[5], y[5];
//   uint8_t pressure[5], area[5];
// } TOUCHINFO;

void touchIO()
{
  // tp.read();
  if (tp.getSamples(&ti)) // Note this function updates coordinates stored within library variables
  {
    int tc = ti.count;
    read_started_ms = millis();      
    _changed = false;  // Debounce time has not ellapsed.
    if (read_started_ms - _last_change > _db_time)          // Debounce time ellapsed.
    {
      _last_state = _current_state;				// Save last state.
      _current_state = ti;					// Assign new state as current state .
      if (!(_current_state.x[0] == _last_state.x[0])) {_changed = true;};
      if (!(_current_state.y[0] == _last_state.y[0])) {_changed = true;};
      if (_changed) {           // State changed.
        //   Serial.println(' ');
        // Serial.print(" changed   ");  
        _last_change = read_started_ms;        // Save current millis as last change time.
        X_Coord = ti.x[0];
        Y_Coord = ti.y[0];       
        // rotatePoints(ti.x[0],ti.y[0]);
        #ifdef TT2_DEBUG 
        //   for (int i=0; i<ti.count; i++){ // for each touch point
            int i = ti.count;
            Serial.print("Count : ");Serial.print(i);
            i = 0;
            Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");
            Serial.print("  x: ");Serial.print(ti.x[i]);
            Serial.print("  y: ");Serial.print(ti.y[i]);
            Serial.print("  pressure: ");Serial.print(ti.pressure[i]);
            Serial.print("  area: ");Serial.println(ti.area[i]);
            drawCross(X_Coord, Y_Coord, TFT_GREEN);  // for debugging, comment out to clean up screen in usage
          // } 
        #endif
        

        int boxCode = HotSpotBox(X_Coord,Y_Coord); 

        #ifdef TT2_DEBUG 
          Serial.print(F("DEBUG: box code: "));
          Serial.println(boxCode);
          Serial.print(F("DEBUG: active screen: "));
          Serial.println(activeScreen);
        #endif

        touchCommand(boxCode);
      }
    }  
  }
}

void drawCross(int x, int y, unsigned int color)
{ 
    // for debugging, comment out to clean up screen in usage

  tft.drawLine(x - 3, y, x + 3, y, color);
  tft.drawLine(x, y - 3, x, y + 3, color);
}

void rotatePoints(int x, int y)
{
  switch (1){
    case 0:
      X_Coord = HRES - y;
      Y_Coord = x;
      break;
    case 1:
      X_Coord = x;
      Y_Coord = y;
      break;
    case 2:
      X_Coord = y;
      Y_Coord = VRES - x;
      break;
    case 3:
      X_Coord = HRES - x;
      Y_Coord = VRES - y;
      break;
    default:
      X_Coord = x;
      Y_Coord = y;
      break;
  }
}
int HotSpotBox(int X, int Y)
{int retcode = 0;
          Serial.print("  x: ");Serial.print(X);
          Serial.print("  y: ");Serial.print(Y);
          Serial.println(' ');           
  if ((X>0) && (Y>0)) {
    for (int i = 0; i < (sizeof(HotBoxes) / sizeof(HotBox)); i++) {
      if ((HotBoxes[i].screen == activeScreen) && (HotBoxes[i].X1 <= X) && (X <= HotBoxes[i].X2) && (HotBoxes[i].Y1 <= Y) && (Y <= HotBoxes[i].Y2 ))
      {
        retcode = i;
        #ifdef TT2_DEBUG             
          Serial.print("Box ");Serial.print(i);Serial.print(" screen: ");Serial.print(HotBoxes[i].screen);
          Serial.print("  x1: ");Serial.print(HotBoxes[i].X1);
          Serial.print("  y1: ");Serial.print(HotBoxes[i].Y1);
          Serial.print("  x2: ");Serial.print(HotBoxes[i].X2);
          Serial.print("  y2: ");Serial.print(HotBoxes[i].Y2);
          Serial.println(' ');  
        #endif 
      }
    }
  }
  return retcode;
}

void TurnOnPixels()   // turn on the pixels
{
    for (int i = 0; i < PixelCount; i++) {     
    strip.neoPixelSetValue(i, 255,255,255,false); 
    }
    strip.neoPixelShow();
    pixelsOn = true;
    Serial.println("On ...");
}
void TurnOffPixels()      // turn off the pixels
{
    for (int i = 0; i < PixelCount; i++) {     
    strip.neoPixelSetValue(i,0,0,0,false); 
    }
    strip.neoPixelShow();
    pixelsOn = false;
}

void TogglePixels()   // toggle the pixels
{
  uint8_t SetRed = RedLevel;
  uint8_t SetBlue = BlueLevel;
  uint8_t SetGreen = GreenLevel;
  if (pixelsOn) { 
    SetRed = 0;
    SetBlue = 0;
    SetGreen = 0;
    pixelsOn = false;
    Serial.println("Off ...");
    }
    else {
      pixelsOn = true;
    Serial.println("On ...");
    }
  for (int i = 0; i < PixelCount; i++) { 
     strip.neoPixelSetValue(i, SetRed,SetGreen,SetBlue,false);
     }    
  strip.neoPixelShow();
}

void DimmerHigh()      // set to higher intensity
{
uint8_t luminance;
  // DimON = false;
  // luminance = _HighLuminosity;
  // strip.SetLuminance(luminance); // requires different neopixel library
  // strip.Show();
  Serial.println("All Bright ...");
}
void DimmerLow()      // dim intensity
{
uint8_t luminance; 
  // DimON = true;
  // luminance = _LowLuminosity;
  // strip.SetLuminance(luminance); // requires different neopixel library
  // strip.Show();        
  Serial.println("All Dim ...");
}
void ScreenPrint(String text, int col, int row, int size)
{
  tft.setTextDatum(TL_DATUM); // Top Left is datum
  tft.setCursor(col, row, size);
  tft.print(text);
}