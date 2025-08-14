#pragma once
#include <cserial/circularbuffer.h>

typedef struct Serial_s Serial_t;

/**
 * @param serial_name path to the serial
 * @param speed serial baudrate
 */
Serial_t *open_serial(const char *serial_name, int speed);

int Serial_read(Serial_t *s, CircularBuffer *buffer, struct timeval *tout);

void Serial_destruct(Serial_t *s);
