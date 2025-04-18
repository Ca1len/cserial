#include <arpa/inet.h>
#include <cserial/udp.h>
#include <memory.h>
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

int send_message(Socket_t *sock, const void *payload, size_t size) {
  int bytes_sent =
      sendto(sock->socket_descriptor, payload, size, 0,
             (struct sockaddr *)&sock->server_addr, sizeof(sock->server_addr));
  if (bytes_sent < 0) {
    printf("Failed to send message\n");
    close(sock->socket_descriptor);
    return 1;
  }

  return 0;
}

void destruct_socket(Socket_t *sock) {
  if (sock) {
    close(sock->socket_descriptor);
    free(sock);
  }
}
