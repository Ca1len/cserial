extern "C" {
#include <cserial/udp.h>
}

#include <gmock/gmock.h>

TEST(UDP, ConstructDestruct) {
  Socket_t *sock = create_udp_socket();
  EXPECT_NE((void *)sock, (void *)NULL);
  destruct_socket(sock);
}
