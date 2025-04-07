#include <cserial/cserial.h>
#include <gtest/gtest.h>
#include <vector>

TEST(test_suite_name, test_name) {
  std::vector<const char *> cmd{"asdf", "985425"};

  EXPECT_EQ(0, cserial_run(cmd.size(), cmd.data()));
}
