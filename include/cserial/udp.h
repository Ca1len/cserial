#pragma once

#include <bits/sockaddr.h>
#include <netinet/in.h>

typedef struct Socket_s Socket_t;

Socket_t *create_udp_socket();

void fill_server_details(Socket_t *sock, sa_family_t sf, in_port_t port,
                         const char *addr);

/*
 * @brief sends message
 * @param payload buffer
 * @param size buffer size
 */
int send_message(Socket_t *sock, const void *payload, size_t size);

void destruct_socket(Socket_t *sock);
