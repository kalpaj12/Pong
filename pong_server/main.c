#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PLAYERS 2

socklen_t addr_size = sizeof(struct sockaddr);
struct sockaddr_in clients_addresses[MAX_PLAYERS];
int players[MAX_PLAYERS];

void init_players() {
  for (int i = 0; i < MAX_PLAYERS; i++) players[i] = 0;
}

void* server_listen_loop(void* arg) {
  int sock_server = *((int*)arg);
  struct sockaddr_in client_addr;
  int16_t rdata[4];

  while (true) {
    recvfrom(sock_server, rdata, sizeof(int16_t) * 4, 0,
             (struct sockaddr*)&client_addr, &addr_size);

    // Debug start
    for (int i = 0; i < 4; i++) printf("%d ", rdata[i]);
    printf("\n");

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

    printf("Client IP: [%s]\n", client_ip);
    // Debug end

    // requst to join server
    if (rdata[0] == -1) {
      // @TODO:
      // check if connected player (client_addr) exceed max_player;
      // check if current connection is a re-connection
      int16_t sdata[4];
      for (int i = 0; i < 4; i++) sdata[i] = 0;
      sdata[0] = 1;
      sendto(sock_server, sdata, sizeof(int16_t) * 4, 0,
             (struct sockaddr*)&client_addr, addr_size);
      // Save to list of connected players
    }
  }
}

// void* server_send_loop(void* arg) {
// int sock_server = *((int *)arg);
// send ball pos, and paddle pos
// }

int main(void) {
  printf("Server Init!\n");

  init_players();

  // char* server_ip_addr = NULL;
  const uint16_t PORT = 8080;

  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

  int sock_server;

  memset(clients_addresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);
  if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket failed");
  }
  if (bind(sock_server, (struct sockaddr*)&server_addr,
           sizeof(struct sockaddr)) < 0) {
    perror("bind server error");
  }

  pthread_t thread_id_server, thread_id_server_send;

  pthread_create(&thread_id_server, NULL, server_listen_loop, &sock_server);
  // pthread_create(&thread_id_server_send, NULL, server_send_loop,
  // &sock_server);

  printf("Use Enter key to close server\n");
  char c;
  while ((c = getchar()) != '\n') {
  }

  close(sock_server);
  pthread_cancel(thread_id_server);
  // pthread_cancel(thread_id_server_send);

  return 0;
}