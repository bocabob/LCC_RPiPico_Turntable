
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
 * \file callbacks.h
 *
 *
 * @author Jim Kueneman
 * @date 31 Dec 2025
 */

// This is a guard condition so that contents of this file are not included
// more than once.
#ifndef __CALLBACKS__
#define __CALLBACKS__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "src/openlcb/openlcb_types.h"
#include "src/drivers/canbus/can_types.h"
#include "src/openlcb/openlcb_gridconnect.h"
#include "src/openlcb/openlcb_application_dcc_detector.h"
#include "BoardSettings.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    extern void Callbacks_initialize(void);

    extern void Callbacks_on_100ms_timer_callback(void);

    extern void Callbacks_on_can_rx_callback(can_msg_t *can_msg);

    extern void Callbacks_on_can_tx_callback(can_msg_t *can_msg);

    extern void Callbacks_alias_change_callback(uint16_t new_alias, node_id_t node_id);

    extern void Callbacks_operations_request_factory_reset(openlcb_statemachine_info_t *statemachine_info, config_mem_operations_request_info_t *config_mem_operations_request_info);

    extern void Callbacks_write_firmware(openlcb_statemachine_info_t *statemachine_info, config_mem_write_request_info_t *config_mem_write_request_info, write_result_t write_result);
    
    extern void Callbacks_freeze(openlcb_statemachine_info_t *statemachine_info, config_mem_operations_request_info_t *config_mem_operations_request_info);
    
    extern void Callbacks_unfreeze(openlcb_statemachine_info_t *statemachine_info, config_mem_operations_request_info_t *config_mem_operations_request_info);

    extern void Callbacks_on_consumed_event_pcer(openlcb_node_t *openlcb_node, uint16_t index, event_id_t *event_id, event_payload_t *payload);

    extern void Callbacks_on_consumed_event_identified(openlcb_node_t *openlcb_node, uint16_t index, event_id_t *event_id, event_status_enum status, event_payload_t *payload);

    extern bool Callbacks_toggle_log_messages(void);

    extern bool Callbacks_on_login_complete(openlcb_node_t *openlcb_node);

    extern void Callbacks_on_broadcast_time_time_changed(broadcast_clock_t *clock);

    
#ifdef __cplusplus
}
#endif /* __cplusplus */

// Deferred EEPROM write flag – set to true whenever the RAM config changes and
// needs to be persisted.  The 100ms timer callback drains it every ~3 seconds.
// Declared outside extern "C" because volatile bool is a C++ type.
#ifdef __cplusplus
extern volatile bool _config_dirty;
#endif

// Per-track RailCom / DCC-detector state.  Populated by the range-hit handler
// in callbacks.cpp whenever a DCC Detection Protocol event fires.  The display
// layer polls _railcom_dirty as a gate, then walks the per-slot dirty flags.
#ifdef OPENLCB_COMPILE_DCC_DETECTOR
typedef struct {
    uint16_t dcc_address;                         // decoded DCC address (0 = empty)
    dcc_detector_address_type_enum address_type;  // short / long / consist / empty
    dcc_detector_direction_enum direction;         // forward / reverse / unknown / unoccupied
    bool occupied;
    bool dirty;                                   // true = display needs refresh for this slot
} railcom_info_t;

#ifdef __cplusplus
extern railcom_info_t _RailCom[MAX_TRACKS];
extern volatile bool _railcom_dirty;
#endif
#endif /* OPENLCB_COMPILE_DCC_DETECTOR */

#endif /* __CALLBACKS__ */