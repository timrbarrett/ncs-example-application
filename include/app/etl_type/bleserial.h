#ifndef BLESERIAL_H
#define BLESERIAL_H

#include <zephyr/kernel.h>
#include <zephyr/types.h>

/* Function to initialize BLE NUS as a serial stream */
int bleserial_init(void);

/* get the size of the rx_ring_buffer */
int bleserial_size(void); 

/* Function to send a character over BLE NUS */
int bleserial_send_char(uint8_t c);

/* Function to read a character from BLE NUS */
int bleserial_receive_char(uint8_t *c);

#endif /* BLESERIAL_H */
