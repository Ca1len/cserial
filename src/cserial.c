#include <cserial/circularbuffer.h>
#include <cserial/cserial.h>
#include <cserial/serial.h>
#include <cserial/udp.h>
#include <pthread.h>
#include <signal.h>
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

void Args_print(Args_t *arg);

void parse_arguments(Args_t *args, int ac, char *av[]);

void Args_delete(Args_t *args);

void print_values(Args_t *args) {
  printf("\t-s: %s\n", args->serial);
  printf("\t-b: %d\n", args->baudrate);
  printf("\t-a: %s\n", args->udp_address);
  printf("\t-p: %d\n", args->port);
}

volatile static bool quit = false;

static void terminate(int sig) {
  quit = true;
  (void)sig;
}

typedef struct {
  CircularBuffer *cb;
  Serial_t *serial;
  struct timeval tout;
} ProducerResources;

void *producer(void *arg) {
  ProducerResources *scb = (ProducerResources *)arg;
  while (!quit) {
    Serial_read(scb->serial, scb->cb, &scb->tout);
  }
  return NULL;
}

int cserial_run(int ac, char *av[]) {
  Args_t *args = Args_create();
  parse_arguments(args, ac, av);

  printf("Parsed arguments:\n");
  print_values(args);

  signal(SIGQUIT, terminate);
  signal(SIGTERM, terminate);
  signal(SIGINT, terminate);

  CircularBuffer cbuffer;
  init_buffer(&cbuffer);

  Serial_t *serial = open_serial(args->serial, args->baudrate);
  if (!serial) {
    fprintf(stderr, "Exiting, cannot open serial.\n");
    exit(-1);
  }

  Socket_t *sock = create_udp_socket();
  if (!serial) {
    fprintf(stderr, "Exiting, cannot create socket.\n");
    exit(-1);
  }
  fill_server_details(sock, AF_INET, args->port, args->udp_address);

  ProducerResources serial_producer;
  serial_producer.serial = serial;
  serial_producer.cb = &cbuffer;
  serial_producer.tout.tv_sec = 1;
  serial_producer.tout.tv_usec = 100 * 1000;
  pthread_t prod_thread;
  pthread_create(&prod_thread, NULL, producer, &serial_producer);

  while (!quit) {
    unsigned char payload = read_buffer(&cbuffer);
    send_message(sock, &payload, 1);
  }

  pthread_join(prod_thread, NULL);

  destruct_socket(sock);
  Serial_destruct(serial);
  destroy_buffer(&cbuffer);
  Args_delete(args);
  printf("Complete exit.\n");
  return 0;
}

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
  print_values(args);
}

void parse_arguments(Args_t *args, int ac, char *av[]) {
  int c;

  while ((c = getopt(ac, av, "s:b:a:p:h")) != -1)
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
      fprintf(stderr, "Error: Got unrecognized option.\n");
      print_help(args);
      abort();
    default:
      abort();
    }
}

void Args_print(Args_t *arg) {
  printf("Serial: %s\n", arg->serial);
  printf("Baudrate: %d\n", arg->baudrate);
  printf("UDP address: %s\n", arg->udp_address);
  printf("Port: %d\n", arg->port);
}

void Args_delete(Args_t *args) { free(args); }
