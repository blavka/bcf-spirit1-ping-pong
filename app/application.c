#include <application.h>

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

uint8_t *rx_buffer;
uint8_t *tx_buffer;

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_PRESS)
    {
        bc_led_pulse(&led, 100);

        strcpy((char *) tx_buffer, "PING");
        bc_spirit1_set_tx_length(5);
        bc_spirit1_tx();

        bc_log_debug("TX: PING");
    }
}

void spirit1_event_handler(bc_spirit1_event_t event, void *event_param)
{
    if (event == BC_SPIRIT1_EVENT_RX_DONE)
    {
        bc_log_dump(rx_buffer, bc_spirit1_get_rx_length(), "RX length=%d", bc_spirit1_get_rx_length());

        if (strncmp((char *) rx_buffer, "PING", bc_spirit1_get_rx_length()) == 0)
        {
            strcpy((char *) tx_buffer, "PONG");
            bc_spirit1_set_tx_length(5);
            bc_spirit1_tx();

            bc_log_debug("TX: PONG");
        }
    }
    else if (event == BC_SPIRIT1_EVENT_TX_DONE)
    {
        bc_spirit1_rx();
    }
}

void application_init(void)
{
    // Initialize logging
    bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_pulse(&led, 2000);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    bc_spirit1_init();
    bc_spirit1_set_event_handler(spirit1_event_handler, NULL);
    rx_buffer = bc_spirit1_get_rx_buffer();
    tx_buffer = bc_spirit1_get_tx_buffer();
    bc_spirit1_rx();
}

void application_task(void)
{
}
