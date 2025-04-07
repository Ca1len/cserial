#include <arpa/inet.h>
#include <bits/sockaddr.h>
#include <cserial/udp.h>
#include <cstddef>
#include <memory.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct Socket_s {
  int socket_descriptor;
  struct sockaddr_in server_addr;
};

Socket_t *create_udp_socket() {
  Socket_t *sock = (Socket_t *)calloc(1, sizeof(Socket_t));
  if (!sock) {
    fprintf(stderr, "Error during memory allocation for Socket_t");
    return NULL;
  }
  sock->socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock->socket_descriptor == -1) {
    fprintf(stderr, "Error during socket initialization");
    return NULL;
  }

  memset(&sock->server_addr, 0, sizeof(struct sockaddr_in));
  return sock;
}

void fill_server_details(Socket_t *sock, sa_family_t sf, in_port_t port,
                         const char *addr) {

  sock->server_addr.sin_family = sf;
  sock->server_addr.sin_port = htons(port);            // Port number
  sock->server_addr.sin_addr.s_addr = inet_addr(addr); // Localhost IP
}

int send_message(Socket_t *sock, const void *payload, size_t size) { return 0; }

int main() {

  // Message to send
  const char *message = "Hello, UDP Server!";

  // Send the message
  sendto(int fd, const void *buf, size_t n, int flags,
         const struct sockaddr *addr, socklen_t addr_len) int bytes_sent =
      sendto(sock, message, strlen(message), 0, (struct sockaddr *)&server_addr,
             sizeof(server_addr));

  if (bytes_sent < 0) {
    printf("Failed to send message\n");
    close(sock);
    return 1;
  }

  printf("Sent %d bytes: %s\n", bytes_sent, message);

  // Clean up
  close(sock);
  return 0;
}
