/*
 * Allows to set arbitrary speed for the serial device on Linux.
 *
 */
#include <asm-generic/termbits.h>
#include <cserial/serial.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/select.h>
// #include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>

struct Serial_s {
  int descriptor;
  int speed;
};

Serial_t *open_serial(const char *serial_name, int speed) {

  Serial_t *serial = (Serial_t *)calloc(1, sizeof(Serial_t));
  if (!serial) {
    fprintf(stderr, "Error during memory allocation for Serial_t\n");
    return NULL;
  }

  serial->descriptor = open(serial_name, O_RDWR);

  if (serial->descriptor == -1) {
    fprintf(stderr, "Error during serial open\n");
    return NULL;
  }

  struct termios2 tio;
  ioctl(serial->descriptor, TCGETS2, &tio);
  tio.c_cflag &= ~CBAUD;
  tio.c_cflag |= BOTHER;
  tio.c_ispeed = speed;
  tio.c_ospeed = speed;
  int r = ioctl(serial->descriptor, TCSETS2, &tio);

  if (r == 0) {
    printf("Serial speed changed successfully.\n");
  } else {
    perror("ioctl failed change speed\n");
    return NULL;
  }

  return serial;
}

int Serial_read(Serial_t *s, CircularBuffer *buffer, struct timeval *tout) {
  fd_set inputs;
  int num, ret;

  unsigned char value;

  num = 0;

  FD_ZERO(&inputs);
  FD_SET(s->descriptor, &inputs);

  ret =
      select(s->descriptor + 1, &inputs, (fd_set *)NULL, (fd_set *)NULL, tout);
  // printf("select = %d\n", ret);
  if (ret < 0) {
    perror("select error!!\n");
    return ret;
  }
  if (ret > 0) {
    if (FD_ISSET(s->descriptor, &inputs)) {
      num = read(s->descriptor, &value, 1);
      if (num > 0) {
        write_buffer(buffer, value);
      }
    }
  }

  return num;
}

void Serial_destruct(Serial_t *s) {
  close(s->descriptor);
  free(s);
}
