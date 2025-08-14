
extern "C" {
#include <cserial/cserial.h>
}

#include <gtest/gtest.h>
#include <vector>

extern "C" {

typedef struct Args_s Args_t;
Args_t *Args_create();

char *Args_get_serial(Args_t *args);

int Args_get_baudrate(Args_t *args);

char *Args_get_udp_address(Args_t *args);

int Args_get_port(Args_t *args);

void Args_delete(Args_t *);

void parse_arguments(Args_t *args, int ac, char *av[]);

void Args_print(Args_t *arg);
}

TEST(application, arguments_default) {
  Args_t *args = Args_create();
  EXPECT_EQ(strcmp(Args_get_serial(args), "/dev/ttyUSB0"), 0);
  EXPECT_EQ(Args_get_baudrate(args), 250000);
  EXPECT_EQ(strcmp(Args_get_udp_address(args), "localhost"), 0);
  EXPECT_EQ(Args_get_port(args), 2333);
  Args_delete(args);
}

TEST(application, parse_arguments) {
  Args_t *args = Args_create();
  char s[] = "-s";
  char sv[] = "/dev/ttyUSB0";
  char b[] = "-b";
  char bv[] = "12345";
  char a[] = "-a";
  char av[] = "l";
  char p[] = "-p";
  char pv[] = "2222";
  std::vector<char *> cmd{s, sv, b, bv, a, av, p, pv};

  parse_arguments(args, cmd.size(), cmd.data());

  EXPECT_EQ(strcmp(Args_get_serial(args), sv), 0);
  EXPECT_EQ(Args_get_baudrate(args), atoi(bv));
  EXPECT_EQ(strcmp(Args_get_udp_address(args), av), 0);
  EXPECT_EQ(Args_get_port(args), atoi(pv));

  Args_delete(args);
}
