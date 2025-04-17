#include <cserial/cserial.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Args_s {
  char *serial;
  int baudrate;
  char *udp_address;
  int port;
};

typedef struct Args_s Args_t;

Args_t *Args_create();

char *Args_get_serial(Args_t *args);
int Args_get_baudrate(Args_t *args);
char *Args_get_udp_address(Args_t *args);
int Args_get_port(Args_t *args);

void parse_arguments(Args_t *args, int ac, char *av[]);

void Args_delete(Args_t *args);

int cserial_run(int ac, const char *av[]) { return 0; }

Args_t *Args_create() {
  Args_t *args = (Args_t *)calloc(1, sizeof(Args_t));
  if (!args) {
    fprintf(stderr, "Arguments parsing error: cannot allocate memory.\n");
    return args;
  }
  args->serial = "/dev/ttyUSB0";
  args->baudrate = 250000;
  args->udp_address = "localhost";
  args->port = 2333;
  return args;
}

char *Args_get_serial(Args_t *args) { return args->serial; }

int Args_get_baudrate(Args_t *args) { return args->baudrate; }
char *Args_get_udp_address(Args_t *args) { return args->udp_address; }
int Args_get_port(Args_t *args) { return args->port; }

void print_help(Args_t *args) {
  printf("Usage for help message: cserial -h\n");
  printf("Usage: cserial -s <path/to/serial> -b <baudrate> -a <udp address> -p "
         "<port>\n");
  printf("Defaults(optional):\n");
  printf("\t-s: %s\n", args->serial);
  printf("\t-b: %d\n", args->baudrate);
  printf("\t-a: %s\n", args->udp_address);
  printf("\t-p: %d\n", args->port);
}

void parse_arguments(Args_t *args, int ac, char *av[]) {
  int c;

  while ((c = getopt(ac, av, "s::b::a::p::h")) != -1)
    switch (c) {
    case 's':
      args->serial = optarg;
      break;
    case 'b':
      args->baudrate = atoi(optarg);
      break;
    case 'a':
      args->udp_address = optarg;
      break;
    case 'p':
      args->port = atoi(optarg);
      break;
    case 'h':
      print_help(args);
      abort();
    case '?':
      print_help(args);
      return;
    default:
      abort();
    }
}

void Args_delete(Args_t *args) {
  if (!args) {
    free(args->serial);
    free(args->udp_address);
  }
  free(args);
}
