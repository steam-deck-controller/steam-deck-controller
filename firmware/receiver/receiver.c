#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btstack.h"
#include <FreeRTOS.h>
#include <task.h>
#include <pico/cyw43_arch.h>

#define TEST_PACKET_SIZE 1000

static enum {
    TC_OFF,
    TC_IDLE,
    TC_W4_SCAN_RESULT,
    TC_W4_CONNECT,
    TC_W4_CHANNEL,
    TC_TEST_DATA
} state = TC_OFF;

const uint16_t TSPX_le_psm = 0x25;

// addr and type of device with correct name
static bd_addr_t      le_cbm_server_addr;
static bd_addr_type_t le_cbm_server_addr_type;

static hci_con_handle_t connection_handle;
static btstack_packet_callback_registration_t hci_event_callback_registration;
static btstack_packet_callback_registration_t sm_event_callback_registration;

static uint8_t cbm_receive_buffer[TEST_PACKET_SIZE];

// support for multiple clients
typedef struct {
    char name;
    hci_con_handle_t connection_handle;
    uint16_t cid;
    int  counter;
    char test_data[TEST_PACKET_SIZE];
    int  test_data_len;
    uint32_t test_data_sent;
    uint32_t test_data_start;
} le_cbm_connection_t;

static le_cbm_connection_t le_cbm_connection;

// returns true if name is found in advertisement
static bool advertisement_report_contains_name(const char * name, uint8_t * advertisement_report){
    // get advertisement from report event
    const uint8_t * adv_data = gap_event_advertising_report_get_data(advertisement_report);
    uint8_t         adv_len  = gap_event_advertising_report_get_data_length(advertisement_report);
    uint16_t        name_len = (uint16_t) strlen(name);

    // iterate over advertisement data
    ad_context_t context;
    for (ad_iterator_init(&context, adv_len, adv_data) ; ad_iterator_has_more(&context) ; ad_iterator_next(&context)){
        uint8_t data_type    = ad_iterator_get_data_type(&context);
        uint8_t data_size    = ad_iterator_get_data_len(&context);
        const uint8_t * data = ad_iterator_get_data(&context);
        switch (data_type){
            case BLUETOOTH_DATA_TYPE_SHORTENED_LOCAL_NAME:
            case BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME:
                // compare prefix
                if (data_size < name_len) break;
                if (memcmp(data, name, name_len) == 0) return true;
                break;
            default:
                break;
        }
    }
    return 0;
}

/*
 * @section Streamer
 *
 * @text The streamer function checks if notifications are enabled and if a notification can be sent now.
 * It creates some test data - a single letter that gets increased every time - and tracks the data sent.
 */
static void streamer(void){

    // create test data
    le_cbm_connection.counter++;
    if (le_cbm_connection.counter > 'Z') le_cbm_connection.counter = 'A';
    memset(le_cbm_connection.test_data, le_cbm_connection.counter, le_cbm_connection.test_data_len);

    // send
    l2cap_send(le_cbm_connection.cid, (uint8_t *) le_cbm_connection.test_data, le_cbm_connection.test_data_len);

    // request another packet
    l2cap_request_can_send_now_event(le_cbm_connection.cid);
}

// Start scan for device with "steam-deck-controller" in advertisement
static void le_cbm_client_start(void){
    printf("Start scanning!\n");
    state = TC_W4_SCAN_RESULT;
    gap_set_scan_parameters(0,0x0030, 0x0030);
    gap_start_scan();
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);

    bd_addr_t event_address;
    uint16_t psm;
    uint16_t cid;
    uint16_t conn_interval;
    hci_con_handle_t handle;
    uint8_t status;

    // do an early return for non BT packets
    if(packet_type != HCI_EVENT_PACKET) return;

    switch (hci_event_packet_get_type(packet)) {
        case BTSTACK_EVENT_STATE:
            // BTstack activated, get started
            if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
                le_cbm_client_start();
            } else {
                state = TC_OFF;
            }
            break;
        case GAP_EVENT_ADVERTISING_REPORT:
            if (state != TC_W4_SCAN_RESULT) return;
            // check name in advertisement
            if (!advertisement_report_contains_name("steam-deck-controller", packet)) return;
            // store address and type
            gap_event_advertising_report_get_address(packet, le_cbm_server_addr);
            le_cbm_server_addr_type = gap_event_advertising_report_get_address_type(packet);
            // stop scanning, and connect to the device
            state = TC_W4_CONNECT;
            gap_stop_scan();
            printf("Stop scan. Connect to device with addr %s.\n", bd_addr_to_str(le_cbm_server_addr));
            gap_connect(le_cbm_server_addr, le_cbm_server_addr_type);
            break;
        case HCI_EVENT_META_GAP:
            // wait for connection complete
            if (hci_event_gap_meta_get_subevent_code(packet) != GAP_SUBEVENT_LE_CONNECTION_COMPLETE) break;
            if (state != TC_W4_CONNECT) return;
            connection_handle = gap_subevent_le_connection_complete_get_connection_handle(packet);
            // print connection parameters (without using float operations)
            conn_interval = gap_subevent_le_connection_complete_get_conn_interval(packet);
            printf("Connection Interval: %u.%02u ms\n", conn_interval * 125 / 100, 25 * (conn_interval & 3));
            printf("Connection Latency: %u\n", gap_subevent_le_connection_complete_get_conn_latency(packet));
            // initialize gatt client context with handle, and add it to the list of active clients
            // query primary services
            printf("Connect to performance test service.\n");
            state = TC_W4_CHANNEL;
            l2cap_cbm_create_channel(&packet_handler, connection_handle, TSPX_le_psm, cbm_receive_buffer,
                                     sizeof(cbm_receive_buffer), L2CAP_LE_AUTOMATIC_CREDITS, LEVEL_0, &le_cbm_connection.cid);
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            printf("Disconnected %s\n", bd_addr_to_str(le_cbm_server_addr));
            if (state == TC_OFF) break;
            le_cbm_client_start();
            break;
        case L2CAP_EVENT_CBM_CHANNEL_OPENED:
            // inform about new l2cap connection
            l2cap_event_cbm_channel_opened_get_address(packet, event_address);
            psm = l2cap_event_cbm_channel_opened_get_psm(packet);
            cid = l2cap_event_cbm_channel_opened_get_local_cid(packet);
            handle = l2cap_event_cbm_channel_opened_get_handle(packet);
            status = l2cap_event_cbm_channel_opened_get_status(packet);
            if (status == ERROR_CODE_SUCCESS) {
                printf("L2CAP: CBM Channel successfully opened: %s, handle 0x%04x, psm 0x%02x, local cid 0x%02x, remote cid 0x%02x\n",
                       bd_addr_to_str(event_address), handle, psm, cid,  little_endian_read_16(packet, 15));
                le_cbm_connection.cid = cid;
                le_cbm_connection.connection_handle = handle;
                le_cbm_connection.test_data_len = btstack_min(l2cap_event_cbm_channel_opened_get_remote_mtu(packet), sizeof(le_cbm_connection.test_data));
                state = TC_TEST_DATA;
                printf("Test packet size: %u\n", le_cbm_connection.test_data_len);
                l2cap_request_can_send_now_event(le_cbm_connection.cid);
            } else {
                printf("L2CAP: Connection to device %s failed. status code 0x%02x\n", bd_addr_to_str(event_address), status);
            }
            break;

        case L2CAP_EVENT_CAN_SEND_NOW:
            streamer();
            break;

        case L2CAP_EVENT_CHANNEL_CLOSED:
            cid = l2cap_event_channel_closed_get_local_cid(packet);
            printf("L2CAP: Channel closed 0x%02x\n", cid);
            break;

        default:
            break;
    }
}

/*
 * @section SM Packet Handler
 *
 * @text The packet handler is used to handle pairing requests
 */
static void sm_packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET) return;

    switch (hci_event_packet_get_type(packet)) {
        case SM_EVENT_JUST_WORKS_REQUEST:
            printf("Just Works requested\n");
            sm_just_works_confirm(sm_event_just_works_request_get_handle(packet));
            break;
        case SM_EVENT_NUMERIC_COMPARISON_REQUEST:
            printf("Confirming numeric comparison: %"PRIu32"\n", sm_event_numeric_comparison_request_get_passkey(packet));
            sm_numeric_comparison_confirm(sm_event_passkey_display_number_get_handle(packet));
            break;
        case SM_EVENT_PASSKEY_DISPLAY_NUMBER:
            printf("Display Passkey: %"PRIu32"\n", sm_event_passkey_display_number_get_passkey(packet));
            break;
        default:
            break;
    }
}

void  btstack_main() {
    l2cap_init();
    sm_init();
    sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    sm_event_callback_registration.callback = &sm_packet_handler;
    sm_add_event_handler(&sm_event_callback_registration);

    // turn on!
    hci_power_control(HCI_POWER_ON);
}

int main()
{
    if (cyw43_arch_init()) {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }

    printf("start up the pico\n");
    xTaskCreate(btstack_main, "BT Stack", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true)
        ;
}
