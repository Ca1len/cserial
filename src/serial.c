/*
 * Allows to set arbitrary speed for the serial device on Linux.
 *
 */
#include <asm-generic/termbits.h>
#include <cserial/serial.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    fprintf(stderr, "Error during memory allocation for Serial_t");
    return NULL;
  }

  serial->descriptor = open(serial_name, O_RDWR);

  if (serial->descriptor == -1) {
    fprintf(stderr, "Error during serial open");
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
    perror("ioctl failed change speed");
    return NULL;
  }

  return serial;
}

int read_port(int fd, unsigned char *buf, size_t len, struct timeval *tout) {
  fd_set inputs;
  int num, ret;

  num = 0;

  FD_ZERO(&inputs);
  FD_SET(fd, &inputs);

  ret = select(fd + 1, &inputs, (fd_set *)NULL, (fd_set *)NULL, tout);
  // printf("select = %d\n", ret);
  if (ret < 0) {
    perror("select error!!");
    return ret;
  }
  if (ret > 0) {
    if (FD_ISSET(fd, &inputs)) {
      num = read(fd, buf, len);
    }
  }

  return num;
}

int main(int argc, char *argv[]) {
  int fd, speed;
  struct timeval tout;
  unsigned char send_ch, recv_ch;

  if (argc != 3) {
    printf("%s device speed\n\nSet speed for a serial device.\nFor instance:\n "
           "   %s /dev/ttyUSB0 250000\n",
           argv[0], argv[0]);
    return -1;
  }

  fd = open(argv[1], O_RDWR);

  speed = atoi(argv[2]);

  // Test code
  sleep(2);

  printf("Test start\n");

  send_ch = '0';
  for (;;) {
    tout.tv_sec = 1;
    tout.tv_usec = 100 * 1000;
    read_port(fd, &recv_ch, 1, &tout);
    if (recv_ch == 'X')
      break;

    sleep(1);
  }

  return 0;
}
