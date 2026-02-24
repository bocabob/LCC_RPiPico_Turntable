/*
 * Auto-generated Configuration Memory Reset Implementation
 * Generated: 2026-02-23T15:46:47.933Z
 */

#include "config_mem_reset.h"
#include "config_mem_map.h"
#include "openlcb_application.h"
#include "openlcb_types.h"
#include "openlcb_utilities.h"
#include "openlcb_node.h"

void ConfigMemReset_set_to_defaults(openlcb_node_t *openlcb_node)
{
    configuration_memory_buffer_t buffer;
    
    /*
     * Node -> Node ID -> Node Name
     * NODE_NODE_ID_NODE_NAME
     * Address: 0x0000 (0)
     * Size: 62 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    buffer[25] = 0x00;
    buffer[26] = 0x00;
    buffer[27] = 0x00;
    buffer[28] = 0x00;
    buffer[29] = 0x00;
    buffer[30] = 0x00;
    buffer[31] = 0x00;
    buffer[32] = 0x00;
    buffer[33] = 0x00;
    buffer[34] = 0x00;
    buffer[35] = 0x00;
    buffer[36] = 0x00;
    buffer[37] = 0x00;
    buffer[38] = 0x00;
    buffer[39] = 0x00;
    buffer[40] = 0x00;
    buffer[41] = 0x00;
    buffer[42] = 0x00;
    buffer[43] = 0x00;
    buffer[44] = 0x00;
    buffer[45] = 0x00;
    buffer[46] = 0x00;
    buffer[47] = 0x00;
    buffer[48] = 0x00;
    buffer[49] = 0x00;
    buffer[50] = 0x00;
    buffer[51] = 0x00;
    buffer[52] = 0x00;
    buffer[53] = 0x00;
    buffer[54] = 0x00;
    buffer[55] = 0x00;
    buffer[56] = 0x00;
    buffer[57] = 0x00;
    buffer[58] = 0x00;
    buffer[59] = 0x00;
    buffer[60] = 0x00;
    buffer[61] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, NODE_NODE_ID_NODE_NAME_ADDR, 62, &buffer);

    /*
     * Node -> Node ID -> Node Description
     * NODE_NODE_ID_NODE_DESCRIPTION
     * Address: 0x003E (62)
     * Size: 63 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    buffer[25] = 0x00;
    buffer[26] = 0x00;
    buffer[27] = 0x00;
    buffer[28] = 0x00;
    buffer[29] = 0x00;
    buffer[30] = 0x00;
    buffer[31] = 0x00;
    buffer[32] = 0x00;
    buffer[33] = 0x00;
    buffer[34] = 0x00;
    buffer[35] = 0x00;
    buffer[36] = 0x00;
    buffer[37] = 0x00;
    buffer[38] = 0x00;
    buffer[39] = 0x00;
    buffer[40] = 0x00;
    buffer[41] = 0x00;
    buffer[42] = 0x00;
    buffer[43] = 0x00;
    buffer[44] = 0x00;
    buffer[45] = 0x00;
    buffer[46] = 0x00;
    buffer[47] = 0x00;
    buffer[48] = 0x00;
    buffer[49] = 0x00;
    buffer[50] = 0x00;
    buffer[51] = 0x00;
    buffer[52] = 0x00;
    buffer[53] = 0x00;
    buffer[54] = 0x00;
    buffer[55] = 0x00;
    buffer[56] = 0x00;
    buffer[57] = 0x00;
    buffer[58] = 0x00;
    buffer[59] = 0x00;
    buffer[60] = 0x00;
    buffer[61] = 0x00;
    buffer[62] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, NODE_NODE_ID_NODE_DESCRIPTION_ADDR, 63, &buffer);

    /*
     * Reset Control -> Flag
     * RESET_CONTROL_FLAG
     * Address: 0x007D (125)
     * Size: 1 byte(s)
     * Default value: 238
     */
    buffer[0] = 0xEE;
    OpenLcbApplication_write_configuration_memory(openlcb_node, RESET_CONTROL_FLAG_ADDR, 1, &buffer);

    /*
     * Parameters -> Track -> Track Count
     * PARAMETERS_TRACK_TRACK_COUNT
     * Address: 0x007E (126)
     * Size: 1 byte(s)
     * Default value: 14
     */
    buffer[0] = 0x0E;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACK_COUNT_ADDR, 1, &buffer);

    /*
     * Parameters -> Track -> Home Track
     * PARAMETERS_TRACK_HOME_TRACK
     * Address: 0x007F (127)
     * Size: 1 byte(s)
     * Default value: 3
     */
    buffer[0] = 0x03;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_HOME_TRACK_ADDR, 1, &buffer);

    /*
     * Parameters -> Track -> Enable Reference Correction
     * PARAMETERS_TRACK_ENABLE_REFERENCE_CORRECTION
     * Address: 0x0080 (128)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_ENABLE_REFERENCE_CORRECTION_ADDR, 1, &buffer);

    /*
     * Parameters -> Track -> Rehome
     * PARAMETERS_TRACK_REHOME
     * Address: 0x0081 (129)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_REHOME_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Increment Track
     * PARAMETERS_TRACK_INCREMENT_TRACK
     * Address: 0x0089 (137)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_INCREMENT_TRACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Decrement Track
     * PARAMETERS_TRACK_DECREMENT_TRACK
     * Address: 0x0091 (145)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_DECREMENT_TRACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Rotate Track 180 degrees
     * PARAMETERS_TRACK_ROTATE_TRACK_180_DEGREES
     * Address: 0x0099 (153)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_ROTATE_TRACK_180_DEGREES_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Toggle Bridge Lights
     * PARAMETERS_TRACK_TOGGLE_BRIDGE_LIGHTS
     * Address: 0x00A1 (161)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TOGGLE_BRIDGE_LIGHTS_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_0 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_0_DESCRIPTION
     * Address: 0x00A9 (169)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_0_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_0 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_0_SHORT_NAME
     * Address: 0x00C2 (194)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_0_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_0 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_0_ALIGN_FRONT
     * Address: 0x00C7 (199)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_0_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_0 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_0_ALIGN_BACK
     * Address: 0x00CF (207)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_0_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_0 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_0_STEPPER_POSITION_IN_STEPS
     * Address: 0x00D7 (215)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_0_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_1 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_1_DESCRIPTION
     * Address: 0x00DB (219)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_1_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_1 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_1_SHORT_NAME
     * Address: 0x00F4 (244)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_1_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_1 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_1_ALIGN_FRONT
     * Address: 0x00F9 (249)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_1_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_1 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_1_ALIGN_BACK
     * Address: 0x0101 (257)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_1_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_1 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_1_STEPPER_POSITION_IN_STEPS
     * Address: 0x0109 (265)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_1_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_2 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_2_DESCRIPTION
     * Address: 0x010D (269)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_2_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_2 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_2_SHORT_NAME
     * Address: 0x0126 (294)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_2_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_2 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_2_ALIGN_FRONT
     * Address: 0x012B (299)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_2_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_2 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_2_ALIGN_BACK
     * Address: 0x0133 (307)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_2_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_2 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_2_STEPPER_POSITION_IN_STEPS
     * Address: 0x013B (315)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_2_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_3 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_3_DESCRIPTION
     * Address: 0x013F (319)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_3_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_3 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_3_SHORT_NAME
     * Address: 0x0158 (344)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_3_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_3 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_3_ALIGN_FRONT
     * Address: 0x015D (349)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_3_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_3 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_3_ALIGN_BACK
     * Address: 0x0165 (357)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_3_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_3 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_3_STEPPER_POSITION_IN_STEPS
     * Address: 0x016D (365)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_3_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_4 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_4_DESCRIPTION
     * Address: 0x0171 (369)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_4_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_4 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_4_SHORT_NAME
     * Address: 0x018A (394)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_4_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_4 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_4_ALIGN_FRONT
     * Address: 0x018F (399)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_4_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_4 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_4_ALIGN_BACK
     * Address: 0x0197 (407)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_4_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_4 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_4_STEPPER_POSITION_IN_STEPS
     * Address: 0x019F (415)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_4_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_5 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_5_DESCRIPTION
     * Address: 0x01A3 (419)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_5_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_5 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_5_SHORT_NAME
     * Address: 0x01BC (444)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_5_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_5 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_5_ALIGN_FRONT
     * Address: 0x01C1 (449)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_5_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_5 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_5_ALIGN_BACK
     * Address: 0x01C9 (457)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_5_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_5 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_5_STEPPER_POSITION_IN_STEPS
     * Address: 0x01D1 (465)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_5_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_6 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_6_DESCRIPTION
     * Address: 0x01D5 (469)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_6_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_6 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_6_SHORT_NAME
     * Address: 0x01EE (494)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_6_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_6 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_6_ALIGN_FRONT
     * Address: 0x01F3 (499)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_6_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_6 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_6_ALIGN_BACK
     * Address: 0x01FB (507)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_6_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_6 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_6_STEPPER_POSITION_IN_STEPS
     * Address: 0x0203 (515)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_6_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_7 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_7_DESCRIPTION
     * Address: 0x0207 (519)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_7_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_7 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_7_SHORT_NAME
     * Address: 0x0220 (544)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_7_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_7 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_7_ALIGN_FRONT
     * Address: 0x0225 (549)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_7_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_7 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_7_ALIGN_BACK
     * Address: 0x022D (557)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_7_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_7 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_7_STEPPER_POSITION_IN_STEPS
     * Address: 0x0235 (565)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_7_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_8 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_8_DESCRIPTION
     * Address: 0x0239 (569)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_8_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_8 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_8_SHORT_NAME
     * Address: 0x0252 (594)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_8_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_8 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_8_ALIGN_FRONT
     * Address: 0x0257 (599)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_8_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_8 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_8_ALIGN_BACK
     * Address: 0x025F (607)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_8_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_8 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_8_STEPPER_POSITION_IN_STEPS
     * Address: 0x0267 (615)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_8_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_9 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_9_DESCRIPTION
     * Address: 0x026B (619)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_9_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_9 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_9_SHORT_NAME
     * Address: 0x0284 (644)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_9_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_9 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_9_ALIGN_FRONT
     * Address: 0x0289 (649)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_9_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_9 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_9_ALIGN_BACK
     * Address: 0x0291 (657)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_9_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_9 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_9_STEPPER_POSITION_IN_STEPS
     * Address: 0x0299 (665)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_9_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_10 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_10_DESCRIPTION
     * Address: 0x029D (669)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_10_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_10 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_10_SHORT_NAME
     * Address: 0x02B6 (694)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_10_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_10 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_10_ALIGN_FRONT
     * Address: 0x02BB (699)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_10_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_10 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_10_ALIGN_BACK
     * Address: 0x02C3 (707)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_10_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_10 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_10_STEPPER_POSITION_IN_STEPS
     * Address: 0x02CB (715)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_10_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_11 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_11_DESCRIPTION
     * Address: 0x02CF (719)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_11_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_11 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_11_SHORT_NAME
     * Address: 0x02E8 (744)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_11_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_11 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_11_ALIGN_FRONT
     * Address: 0x02ED (749)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_11_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_11 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_11_ALIGN_BACK
     * Address: 0x02F5 (757)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_11_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_11 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_11_STEPPER_POSITION_IN_STEPS
     * Address: 0x02FD (765)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_11_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_12 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_12_DESCRIPTION
     * Address: 0x0301 (769)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_12_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_12 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_12_SHORT_NAME
     * Address: 0x031A (794)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_12_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_12 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_12_ALIGN_FRONT
     * Address: 0x031F (799)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_12_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_12 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_12_ALIGN_BACK
     * Address: 0x0327 (807)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_12_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_12 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_12_STEPPER_POSITION_IN_STEPS
     * Address: 0x032F (815)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_12_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_13 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_13_DESCRIPTION
     * Address: 0x0333 (819)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_13_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_13 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_13_SHORT_NAME
     * Address: 0x034C (844)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_13_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_13 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_13_ALIGN_FRONT
     * Address: 0x0351 (849)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_13_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_13 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_13_ALIGN_BACK
     * Address: 0x0359 (857)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_13_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_13 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_13_STEPPER_POSITION_IN_STEPS
     * Address: 0x0361 (865)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_13_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_14 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_14_DESCRIPTION
     * Address: 0x0365 (869)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_14_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_14 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_14_SHORT_NAME
     * Address: 0x037E (894)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_14_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_14 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_14_ALIGN_FRONT
     * Address: 0x0383 (899)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_14_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_14 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_14_ALIGN_BACK
     * Address: 0x038B (907)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_14_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_14 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_14_STEPPER_POSITION_IN_STEPS
     * Address: 0x0393 (915)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_14_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_15 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_15_DESCRIPTION
     * Address: 0x0397 (919)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_15_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_15 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_15_SHORT_NAME
     * Address: 0x03B0 (944)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_15_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_15 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_15_ALIGN_FRONT
     * Address: 0x03B5 (949)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_15_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_15 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_15_ALIGN_BACK
     * Address: 0x03BD (957)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_15_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_15 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_15_STEPPER_POSITION_IN_STEPS
     * Address: 0x03C5 (965)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_15_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_16 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_16_DESCRIPTION
     * Address: 0x03C9 (969)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_16_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_16 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_16_SHORT_NAME
     * Address: 0x03E2 (994)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_16_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_16 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_16_ALIGN_FRONT
     * Address: 0x03E7 (999)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_16_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_16 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_16_ALIGN_BACK
     * Address: 0x03EF (1007)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_16_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_16 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_16_STEPPER_POSITION_IN_STEPS
     * Address: 0x03F7 (1015)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_16_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_17 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_17_DESCRIPTION
     * Address: 0x03FB (1019)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_17_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_17 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_17_SHORT_NAME
     * Address: 0x0414 (1044)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_17_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_17 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_17_ALIGN_FRONT
     * Address: 0x0419 (1049)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_17_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_17 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_17_ALIGN_BACK
     * Address: 0x0421 (1057)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_17_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_17 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_17_STEPPER_POSITION_IN_STEPS
     * Address: 0x0429 (1065)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_17_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_18 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_18_DESCRIPTION
     * Address: 0x042D (1069)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_18_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_18 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_18_SHORT_NAME
     * Address: 0x0446 (1094)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_18_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_18 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_18_ALIGN_FRONT
     * Address: 0x044B (1099)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_18_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_18 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_18_ALIGN_BACK
     * Address: 0x0453 (1107)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_18_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_18 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_18_STEPPER_POSITION_IN_STEPS
     * Address: 0x045B (1115)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_18_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_19 -> Description
     * PARAMETERS_TRACK_TRACKS_REP_19_DESCRIPTION
     * Address: 0x045F (1119)
     * Size: 25 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    buffer[16] = 0x00;
    buffer[17] = 0x00;
    buffer[18] = 0x00;
    buffer[19] = 0x00;
    buffer[20] = 0x00;
    buffer[21] = 0x00;
    buffer[22] = 0x00;
    buffer[23] = 0x00;
    buffer[24] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_19_DESCRIPTION_ADDR, 25, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_19 -> Short Name
     * PARAMETERS_TRACK_TRACKS_REP_19_SHORT_NAME
     * Address: 0x0478 (1144)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_19_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_19 -> Align Front
     * PARAMETERS_TRACK_TRACKS_REP_19_ALIGN_FRONT
     * Address: 0x047D (1149)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_19_ALIGN_FRONT_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_19 -> Align Back
     * PARAMETERS_TRACK_TRACKS_REP_19_ALIGN_BACK
     * Address: 0x0485 (1157)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_19_ALIGN_BACK_ADDR, 8, &buffer);

    /*
     * Parameters -> Track -> Tracks -> REP_19 -> Stepper Position in Steps
     * PARAMETERS_TRACK_TRACKS_REP_19_STEPPER_POSITION_IN_STEPS
     * Address: 0x048D (1165)
     * Size: 4 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_TRACK_TRACKS_REP_19_STEPPER_POSITION_IN_STEPS_ADDR, 4, &buffer);

    /*
     * Parameters -> Doors -> Active Doors
     * PARAMETERS_DOORS_ACTIVE_DOORS
     * Address: 0x0491 (1169)
     * Size: 1 byte(s)
     * Default value: 10
     */
    buffer[0] = 0x0A;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_ACTIVE_DOORS_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Open All Doors
     * PARAMETERS_DOORS_OPEN_ALL_DOORS
     * Address: 0x0492 (1170)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_OPEN_ALL_DOORS_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Close All Doors
     * PARAMETERS_DOORS_CLOSE_ALL_DOORS
     * Address: 0x049A (1178)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_CLOSE_ALL_DOORS_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_0 -> Description
     * PARAMETERS_DOORS_DOOR_REP_0_DESCRIPTION
     * Address: 0x04A2 (1186)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_0_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_0 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_0_SHORT_NAME
     * Address: 0x04B2 (1202)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_0_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_0 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_0_TOGGLE_DOOR
     * Address: 0x04B7 (1207)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_0_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_0 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_0_TRACK_NUMBER
     * Address: 0x04BF (1215)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_0_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_1 -> Description
     * PARAMETERS_DOORS_DOOR_REP_1_DESCRIPTION
     * Address: 0x04C0 (1216)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_1_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_1 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_1_SHORT_NAME
     * Address: 0x04D0 (1232)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_1_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_1 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_1_TOGGLE_DOOR
     * Address: 0x04D5 (1237)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_1_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_1 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_1_TRACK_NUMBER
     * Address: 0x04DD (1245)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_1_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_2 -> Description
     * PARAMETERS_DOORS_DOOR_REP_2_DESCRIPTION
     * Address: 0x04DE (1246)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_2_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_2 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_2_SHORT_NAME
     * Address: 0x04EE (1262)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_2_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_2 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_2_TOGGLE_DOOR
     * Address: 0x04F3 (1267)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_2_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_2 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_2_TRACK_NUMBER
     * Address: 0x04FB (1275)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_2_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_3 -> Description
     * PARAMETERS_DOORS_DOOR_REP_3_DESCRIPTION
     * Address: 0x04FC (1276)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_3_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_3 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_3_SHORT_NAME
     * Address: 0x050C (1292)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_3_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_3 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_3_TOGGLE_DOOR
     * Address: 0x0511 (1297)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_3_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_3 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_3_TRACK_NUMBER
     * Address: 0x0519 (1305)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_3_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_4 -> Description
     * PARAMETERS_DOORS_DOOR_REP_4_DESCRIPTION
     * Address: 0x051A (1306)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_4_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_4 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_4_SHORT_NAME
     * Address: 0x052A (1322)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_4_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_4 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_4_TOGGLE_DOOR
     * Address: 0x052F (1327)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_4_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_4 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_4_TRACK_NUMBER
     * Address: 0x0537 (1335)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_4_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_5 -> Description
     * PARAMETERS_DOORS_DOOR_REP_5_DESCRIPTION
     * Address: 0x0538 (1336)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_5_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_5 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_5_SHORT_NAME
     * Address: 0x0548 (1352)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_5_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_5 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_5_TOGGLE_DOOR
     * Address: 0x054D (1357)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_5_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_5 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_5_TRACK_NUMBER
     * Address: 0x0555 (1365)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_5_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_6 -> Description
     * PARAMETERS_DOORS_DOOR_REP_6_DESCRIPTION
     * Address: 0x0556 (1366)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_6_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_6 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_6_SHORT_NAME
     * Address: 0x0566 (1382)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_6_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_6 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_6_TOGGLE_DOOR
     * Address: 0x056B (1387)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_6_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_6 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_6_TRACK_NUMBER
     * Address: 0x0573 (1395)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_6_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_7 -> Description
     * PARAMETERS_DOORS_DOOR_REP_7_DESCRIPTION
     * Address: 0x0574 (1396)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_7_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_7 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_7_SHORT_NAME
     * Address: 0x0584 (1412)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_7_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_7 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_7_TOGGLE_DOOR
     * Address: 0x0589 (1417)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_7_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_7 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_7_TRACK_NUMBER
     * Address: 0x0591 (1425)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_7_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_8 -> Description
     * PARAMETERS_DOORS_DOOR_REP_8_DESCRIPTION
     * Address: 0x0592 (1426)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_8_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_8 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_8_SHORT_NAME
     * Address: 0x05A2 (1442)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_8_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_8 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_8_TOGGLE_DOOR
     * Address: 0x05A7 (1447)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_8_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_8 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_8_TRACK_NUMBER
     * Address: 0x05AF (1455)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_8_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_9 -> Description
     * PARAMETERS_DOORS_DOOR_REP_9_DESCRIPTION
     * Address: 0x05B0 (1456)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_9_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_9 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_9_SHORT_NAME
     * Address: 0x05C0 (1472)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_9_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_9 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_9_TOGGLE_DOOR
     * Address: 0x05C5 (1477)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_9_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_9 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_9_TRACK_NUMBER
     * Address: 0x05CD (1485)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_9_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_10 -> Description
     * PARAMETERS_DOORS_DOOR_REP_10_DESCRIPTION
     * Address: 0x05CE (1486)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_10_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_10 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_10_SHORT_NAME
     * Address: 0x05DE (1502)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_10_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_10 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_10_TOGGLE_DOOR
     * Address: 0x05E3 (1507)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_10_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_10 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_10_TRACK_NUMBER
     * Address: 0x05EB (1515)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_10_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_11 -> Description
     * PARAMETERS_DOORS_DOOR_REP_11_DESCRIPTION
     * Address: 0x05EC (1516)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_11_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_11 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_11_SHORT_NAME
     * Address: 0x05FC (1532)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_11_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_11 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_11_TOGGLE_DOOR
     * Address: 0x0601 (1537)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_11_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_11 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_11_TRACK_NUMBER
     * Address: 0x0609 (1545)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_11_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_12 -> Description
     * PARAMETERS_DOORS_DOOR_REP_12_DESCRIPTION
     * Address: 0x060A (1546)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_12_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_12 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_12_SHORT_NAME
     * Address: 0x061A (1562)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_12_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_12 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_12_TOGGLE_DOOR
     * Address: 0x061F (1567)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_12_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_12 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_12_TRACK_NUMBER
     * Address: 0x0627 (1575)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_12_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_13 -> Description
     * PARAMETERS_DOORS_DOOR_REP_13_DESCRIPTION
     * Address: 0x0628 (1576)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_13_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_13 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_13_SHORT_NAME
     * Address: 0x0638 (1592)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_13_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_13 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_13_TOGGLE_DOOR
     * Address: 0x063D (1597)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_13_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_13 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_13_TRACK_NUMBER
     * Address: 0x0645 (1605)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_13_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_14 -> Description
     * PARAMETERS_DOORS_DOOR_REP_14_DESCRIPTION
     * Address: 0x0646 (1606)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_14_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_14 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_14_SHORT_NAME
     * Address: 0x0656 (1622)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_14_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_14 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_14_TOGGLE_DOOR
     * Address: 0x065B (1627)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_14_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_14 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_14_TRACK_NUMBER
     * Address: 0x0663 (1635)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_14_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_15 -> Description
     * PARAMETERS_DOORS_DOOR_REP_15_DESCRIPTION
     * Address: 0x0664 (1636)
     * Size: 16 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    buffer[8] = 0x00;
    buffer[9] = 0x00;
    buffer[10] = 0x00;
    buffer[11] = 0x00;
    buffer[12] = 0x00;
    buffer[13] = 0x00;
    buffer[14] = 0x00;
    buffer[15] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_15_DESCRIPTION_ADDR, 16, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_15 -> Short Name
     * PARAMETERS_DOORS_DOOR_REP_15_SHORT_NAME
     * Address: 0x0674 (1652)
     * Size: 5 byte(s)
     * Default value: 
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_15_SHORT_NAME_ADDR, 5, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_15 -> Toggle Door
     * PARAMETERS_DOORS_DOOR_REP_15_TOGGLE_DOOR
     * Address: 0x0679 (1657)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_15_TOGGLE_DOOR_ADDR, 8, &buffer);

    /*
     * Parameters -> Doors -> Door -> REP_15 -> Track Number
     * PARAMETERS_DOORS_DOOR_REP_15_TRACK_NUMBER
     * Address: 0x0681 (1665)
     * Size: 1 byte(s)
     * Default value: 0
     */
    buffer[0] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_DOORS_DOOR_REP_15_TRACK_NUMBER_ADDR, 1, &buffer);

    /*
     * Parameters -> Lights -> Toggle Bridge Lights
     * PARAMETERS_LIGHTS_TOGGLE_BRIDGE_LIGHTS
     * Address: 0x0682 (1666)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_TOGGLE_BRIDGE_LIGHTS_ADDR, 8, &buffer);

    /*
     * Parameters -> Lights -> Toggle Interior Lights
     * PARAMETERS_LIGHTS_TOGGLE_INTERIOR_LIGHTS
     * Address: 0x068A (1674)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_TOGGLE_INTERIOR_LIGHTS_ADDR, 8, &buffer);

    /*
     * Parameters -> Lights -> Toggle Exterior Lights
     * PARAMETERS_LIGHTS_TOGGLE_EXTERIOR_LIGHTS
     * Address: 0x0692 (1682)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_TOGGLE_EXTERIOR_LIGHTS_ADDR, 8, &buffer);

    /*
     * Parameters -> Lights -> Maximum Luminosity
     * PARAMETERS_LIGHTS_MAXIMUM_LUMINOSITY
     * Address: 0x069A (1690)
     * Size: 1 byte(s)
     * Default value: 150
     */
    buffer[0] = 0x96;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_MAXIMUM_LUMINOSITY_ADDR, 1, &buffer);

    /*
     * Parameters -> Lights -> Full Luminosity On
     * PARAMETERS_LIGHTS_FULL_LUMINOSITY_ON
     * Address: 0x069B (1691)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_FULL_LUMINOSITY_ON_ADDR, 8, &buffer);

    /*
     * Parameters -> Lights -> Minimum Luminosity
     * PARAMETERS_LIGHTS_MINIMUM_LUMINOSITY
     * Address: 0x06A3 (1699)
     * Size: 1 byte(s)
     * Default value: 50
     */
    buffer[0] = 0x32;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_MINIMUM_LUMINOSITY_ADDR, 1, &buffer);

    /*
     * Parameters -> Lights -> Low Luminosity On
     * PARAMETERS_LIGHTS_LOW_LUMINOSITY_ON
     * Address: 0x06A4 (1700)
     * Size: 8 byte(s)
     * Default value: 00.00.00.00.00.00.00.00
     */
    buffer[0] = 0x00;
    buffer[1] = 0x00;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    buffer[6] = 0x00;
    buffer[7] = 0x00;
    OpenLcbApplication_write_configuration_memory(openlcb_node, PARAMETERS_LIGHTS_LOW_LUMINOSITY_ON_ADDR, 8, &buffer);

}
