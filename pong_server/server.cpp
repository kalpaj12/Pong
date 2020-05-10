#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_PLAYERS 2

socklen_t addr_size = sizeof(struct sockaddr_in);
struct sockaddr_in clients_addresses[MAX_PLAYERS];
int players[MAX_PLAYERS];

void getLocalIP() {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, "wlp2s0", IFNAMSIZ - 1);
  ioctl(fd, SIOCGIFADDR, &ifr);

  close(fd);

  printf("Server IP: [%s]\n",
         inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
}

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
  printf("Server Init Successful!\n");
  getLocalIP();

  init_players();

  // char* server_ip_addr = NULL;
  const uint16_t PORT = 8080;

  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

  memset(clients_addresses, 0, sizeof(struct sockaddr_in) * MAX_PLAYERS);

  // Create a UDP socket
  int sock_server;
  if ((sock_server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if (bind(sock_server, (struct sockaddr*)&server_addr,
           sizeof(struct sockaddr)) < 0) {
    perror("bind server error");
    exit(EXIT_FAILURE);
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