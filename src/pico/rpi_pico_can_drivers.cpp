#include <stdbool.h>
/** \copyright
 * Copyright (c) 2025, Jim Kueneman
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file rpi_pico_can_drivers.cpp
 *
 * This file in the interface between the OpenLcbCLib and the specific MCU/PC implementation
 * to read/write on the CAN bus.  A new supported MCU/PC will create a file that handles the
 * specifics then hook them into this file through #ifdefs
 *
 * @author Jim Kueneman
 * @date 31 Dec 2025
 */

#include "rpi_pico_can_drivers.h"

#include "../drivers/canbus/can_rx_statemachine.h"
#include "../drivers/canbus/can_types.h"
#include "../openlcb/openlcb_gridconnect.h"
#include "../utilities/mustangpeak_string_helper.h"

#ifdef ARDUINO_COMPATIBLE
// include any header files the Raspberry Pi Pico need to compile under Arduino/PlatformIO
#include "Arduino.h"
#include <ACAN2517.h>
#include <SPI.h>
#endif

// Uncomment to enable logging
//#define LOG_SETUP
//#define LOG_SUCCESSFUL_SETUP_PARAMETERS

// static const byte MCP2517_CS = 17;   // CS input of MCP2517
// static const byte MCP2517_INT = 20;  // INT output of MCP2517

// Create a ACAN2517 Object
static ACAN2517 can(MCP2517_CS, MCP2517_SPI, MCP2517_INT);

void RPiPicoCanDriver_setup(void) {

  // Initialize Raspberry Pi Pico CAN features

//--- Define alternate pins for SPI
  MCP2517_SPI.setMOSI (MCP2517_SDI) ;
  MCP2517_SPI.setMISO (MCP2517_SDO) ;
  MCP2517_SPI.setSCK (MCP2517_SCK) ;
//--- Begin SPI — use false (manual CS) so GP17 stays plain GPIO until ACAN2517 takes it
  MCP2517_SPI.begin(false);

//--- Pre-reset: send MCP2517 SPI reset (0x0000) so the chip is guaranteed in
//    Configuration Mode before can.begin() polls for it.  Without this, a USB
//    re-flash without a full power cycle can leave the chip in Normal Mode and
//    cause a kRequestedConfigurationModeTimeOut (error 0x1).
  pinMode(MCP2517_CS, OUTPUT);
  digitalWrite(MCP2517_CS, HIGH);
  MCP2517_SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));
  digitalWrite(MCP2517_CS, LOW);
  MCP2517_SPI.transfer16(0x0000);          // MCP2517FD SPI reset instruction
  digitalWrite(MCP2517_CS, HIGH);
  MCP2517_SPI.endTransaction();
  delay(10);                               // post-reset stabilisation (generous margin)

  // Setup for 125kHz
  ACAN2517Settings settings(ACAN2517Settings::OSC_40MHz, 125UL * 1000UL);
  settings.mDriverTransmitFIFOSize = 10;

#ifdef LOG_SETUP  // Uncomment the define above to enable

//--- Two-stage SPI diagnostic:
//
//  Test 1 — MISO only (no write needed):
//    Read C1CON register (addr 0x000). After SPI reset the chip is in Configuration
//    Mode so byte[2] = 0x80 (WAKFIL=1) and byte[3] = 0x24 (OPMODE=100, REQOP=100).
//    All-zeros means MISO is stuck at GND — hardware fault on GP16 or MCP2517 SDO.
//
//  Test 2 — MOSI + MISO:
//    Write 0xDEADBEEF to message RAM at 0x400, then read it back.
//    Match = full SPI working.  All-zeros (with Test 1 non-zero) = MOSI not reaching chip.
  {
    MCP2517_SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));

    // Test 1: read C1CON (0x000) — READ cmd = (0b0011<<12)|0x000 = 0x3000
    uint8_t c1con[6] = {0x30, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
    digitalWrite(MCP2517_CS, LOW);
    MCP2517_SPI.transfer(c1con, 6);
    digitalWrite(MCP2517_CS, HIGH);
    Serial.printf("C1CON (MISO test):   %02X %02X %02X %02X\n",
                  c1con[2], c1con[3], c1con[4], c1con[5]);
    Serial.println("  byte[2]=0x80 byte[3]=0x24 => chip OK; 00 00 00 00 => MISO stuck low");

    // Test 2: write DE AD BE EF to RAM[0x400], read back
    // WRITE cmd = (0b0010<<12)|0x400 = 0x2400
    uint8_t wbuf[6] = {0x24, 0x00, 0xDE, 0xAD, 0xBE, 0xEF};
    digitalWrite(MCP2517_CS, LOW);
    MCP2517_SPI.transfer(wbuf, 6);
    digitalWrite(MCP2517_CS, HIGH);
    // READ cmd = (0b0011<<12)|0x400 = 0x3400
    uint8_t rbuf[6] = {0x34, 0x00, 0x00, 0x00, 0x00, 0x00};
    digitalWrite(MCP2517_CS, LOW);
    MCP2517_SPI.transfer(rbuf, 6);
    digitalWrite(MCP2517_CS, HIGH);

    MCP2517_SPI.endTransaction();
    Serial.printf("RAM[0x400] (MOSI+MISO): wrote DE AD BE EF, read %02X %02X %02X %02X\n",
                  rbuf[2], rbuf[3], rbuf[4], rbuf[5]);
    Serial.println("  DE AD BE EF => full SPI OK; 00 00 00 00 w/ C1CON OK => MOSI not reaching chip");
  }

  const uint16_t errorCode = can.begin(settings, [] {
    can.isr();
  });
  Serial.print("\nerrorCode=");
  Serial.println(errorCode);
  if (errorCode == 0) {
#ifdef LOG_SUCCESSFUL_SETUP_PARAMETERS  // Uncomment the define above to enable
    Serial.print("\nBit Rate prescaler: ");
    Serial.println(settings.mBitRatePrescaler);
    Serial.print("Phase segment 1: ");
    Serial.println(settings.mPhaseSegment1);
    Serial.print("Phase segment 2: ");
    Serial.println(settings.mPhaseSegment2);
    Serial.print("SJW: ");
    Serial.println(settings.mSJW);
    Serial.print("Actual bit rate: ");
    Serial.print(settings.actualBitRate());
    Serial.println(" bit/s");
    Serial.print("Exact bit rate ? ");
    Serial.println(settings.exactBitRate() ? "yes" : "no");
    Serial.print("Sample point: ");
    Serial.print(settings.samplePointFromBitStart());
    Serial.println("%");
#endif
  } else {
    Serial.print("\n\nACAN ERROR: Configuration error 0x");
    Serial.println(errorCode, HEX);
  }

#else  // !LOG_SETUP
  can.begin(settings, [] {
    can.isr();
  });
#endif
}

void RPiPicoCanDriver_process_receive(void) {

  can_msg_t can_msg;
  CANMessage frame;

  if (can.available()) {

    if (can.receive(frame)) {

      if (frame.ext) {  // Only Extended messages

        can_msg.state.allocated = true;
        can_msg.payload_count = frame.len;
        can_msg.identifier = frame.id;

        for (int i = 0; i < frame.len; i++) {

          can_msg.payload[i] = frame.data[i];
        }
  
        CanRxStatemachine_incoming_can_driver_callback(&can_msg);
     
      }
    }
  }
}

bool RPiPicoCanDriver_is_can_tx_buffer_clear(void) {

  // ACAN Library does not have a method to know if the buffer is full, I believe OpenLcbCLib will
  // function correctly if the Transmit fails as well.  This was just for a short cut if available.

  return true;
}

bool RPiPicoCanDriver_transmit_raw_can_frame(can_msg_t *msg) {

  CANMessage frame;

  frame.ext = true;
  frame.id = msg->identifier;
  frame.len = msg->payload_count;
  for (int i = 0; i < frame.len; i++) {

    frame.data[i] = msg->payload[i];
  }

  return can.tryToSend(frame);
}

void RPiPicoCanDriver_pause_can_rx(void) {

  // Not required as the ACAN2517 library uses and interrupt to get the next message in the background and we access it from the mainloop
}

void RPiPicoCanDriver_resume_can_rx(void) {

  // Not required as the ACAN2517 library uses and interrupt to get the next message in the background and we access it from the mainloop
}
