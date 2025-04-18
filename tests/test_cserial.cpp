
extern "C" {
#include <cserial/cserial.h>
}

#include <gtest/gtest.h>
#include <vector>

TEST(application, launch) {
  std::vector<const char *> cmd{""};
  EXPECT_EQ(0, cserial_run(cmd.size(), cmd.data()));
}

extern "C" {

typedef struct Args_s Args_t;
Args_t *Args_create();

char *Args_get_serial(Args_t *args);

int Args_get_baudrate(Args_t *args);

char *Args_get_udp_address(Args_t *args);

int Args_get_port(Args_t *args);

void Args_delete(Args_t *);

void parse_arguments(Args_t *args, int ac, const char *av[]);

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
  std::vector<const char *> cmd{"-s", "/dev/ttyUSB0", "-b",  "12345", "-a",
                                "l",  "-p",           "2222"};
  auto s = cmd[1];
  auto b = cmd[3];
  auto a = cmd[5];
  auto p = cmd[7];

  parse_arguments(args, cmd.size(), cmd.data());
  Args_print(args);

  EXPECT_EQ(strcmp(Args_get_serial(args), s), 0);
  EXPECT_EQ(Args_get_baudrate(args), atoi(b));
  EXPECT_EQ(strcmp(Args_get_udp_address(args), a), 0);
  EXPECT_EQ(Args_get_port(args), atoi(p));

  Args_delete(args);
}
