#include "bleserial.h"
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/services/nus.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bleserial);

#define RING_BUFFER_SIZE 256

static struct ring_buf nus_rx_ring_buf;
static uint8_t nus_rx_buffer[RING_BUFFER_SIZE];
static struct bt_conn *current_conn = NULL;

// === connection handling ===

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("Connection failed (err %u)", err);
        return;
    }

    LOG_INF("Connected");
    current_conn = bt_conn_ref(conn); // Save the connection handle
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected (reason %u)", reason);
    if (current_conn) {
        bt_conn_unref(current_conn); // Release the handle
        current_conn = NULL;
    }
}

/* Register the callbacks */
static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

// === connection handling

static void notif_enabled(bool enabled, void *ctx)
{
    ARG_UNUSED(ctx);
    LOG_INF("%s() - %s", __func__, (enabled ? "Enabled" : "Disabled"));
}

// Buffer for collecting received characters
#define MAX_EXPRESSION_LENGTH 256

// NUS data received callback
static void received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx) {
    ARG_UNUSED(conn);
    ARG_UNUSED(ctx);
    if (!data || len == 0) {
        LOG_WRN("Received empty or invalid data");
        return;
    }

    //LOG_INF("Received %d bytes: %.*s", len, len, (const char *)data);

    /* Write received data into the ring buffer */
    uint32_t written = ring_buf_put(&nus_rx_ring_buf, data, len);
    
    if (written < len) {
        LOG_WRN("RX buffer overflow: dropped %d bytes", len - written);
    }
}

struct bt_nus_cb nus_listener = {
    .notif_enabled = notif_enabled,
    .received = received,
};

#define DEVICE_NAME         CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN     (sizeof(DEVICE_NAME) - 1)

/* Initialize BLE NUS */
int bleserial_init(void)
{
    int err;

    /* Enable Bluetooth */
    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err);
        return err;
    }

    LOG_INF("Bluetooth initialized");

    err = bt_nus_cb_register(&nus_listener, NULL);
    if (err) {
        LOG_ERR("Failed to register NUS callback: %d\n", err);
        return err;
    }

    struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
        BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
    };

    struct bt_data sd[] = {
        BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_SRV_VAL),
    };

    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (err) {
        LOG_ERR("Failed to start advertising: %d\n", err);
        return err;
    }

    LOG_INF("NUS service initialized");

    /* Register connection callbacks */
    bt_conn_cb_register(&conn_callbacks);

    /* Initialize ring buffer */
    ring_buf_init(&nus_rx_ring_buf, sizeof(nus_rx_buffer), nus_rx_buffer);

    return 0;
}

int bleserial_size(void) 
{
    return ring_buf_size_get(&nus_rx_ring_buf);
}

int bleserial_send_char(uint8_t c)
{
    if (!current_conn) {
        //LOG_ERR("No active connection");
        return -ENOTCONN;
    }

    int err = bt_nus_send(current_conn, &c, 1);

    if (err) {
        LOG_ERR("Failed to send data over NUS (err %d)", err);
    }

    return err;
}

int bleserial_receive_char(uint8_t *c)
{

    if (!c) {
        LOG_WRN("ln%i no read",__LINE__);
        return -EINVAL;
    }

    /* Read a single byte from the ring buffer */
    uint32_t bytes_read = ring_buf_get(&nus_rx_ring_buf, c, 1);

    if (bytes_read == 0) {
        return -ENODATA; /* No data available */
    }

    return 0;
}


