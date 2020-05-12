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

#include <queue>

#define MAX_PLAYERS 2

const uint16_t PORT = 8080;

typedef struct player {
  int paddle_y_pos;
  int ball_x_pos;
  int ball_y_pos;
  int opponent_id;
  struct sockaddr_in address;
} player;

player players[MAX_PLAYERS];
volatile int connected_players;
std::queue<int> ready;

socklen_t addr_size = sizeof(struct sockaddr_in);

void getServerIP() {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct ifreq ifr;
  ifr.ifr_addr.sa_family = AF_INET;

  strncpy(ifr.ifr_name, "wlp2s0", IFNAMSIZ - 1);
  ioctl(fd, SIOCGIFADDR, &ifr);

  close(fd);

  printf("Server Listening on: [%s::%d]\n",
         inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr), PORT);
}

void matchmaking(int16_t rdata[], int sock_server) {
  if (ready.size() >= 2) {
    int player_1 = ready.front();
    ready.pop();
    int player_2 = ready.front();
    ready.pop();

    players[player_1].opponent_id = player_2;
    players[player_2].opponent_id = player_1;

    int16_t sdata1[4], sdata2[4];
    for (int i = 0; i < 4; i++) sdata1[i] = sdata2[i] = 0;

    sdata1[0] = player_2;
    // player_1 takes care of the ball
    sdata1[1] = 1;

    sdata2[0] = player_1;

    sendto(sock_server, sdata1, sizeof(int16_t) * 4, 0,
           (struct sockaddr*)&players[player_1].address, addr_size);

    sendto(sock_server, sdata2, sizeof(int16_t) * 4, 0,
           (struct sockaddr*)&players[player_2].address, addr_size);
  }
}

void* listen(void* arg) {
  int sock_server = *((int*)arg);
  struct sockaddr_in client_addr;
  int16_t rdata[4];

  while (true) {
    int bytes = recvfrom(sock_server, rdata, sizeof(int16_t) * 4, 0,
                         (struct sockaddr*)&client_addr, &addr_size);
    if (bytes <= 0) continue;

    printf("From IP: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Received Datagram: ");
    for (int i = 0; i < 4; i++) printf("%d ", rdata[i]);
    printf("\n");

    // requst to join server
    if (rdata[0] == -1) {
      if (connected_players + 1 < MAX_PLAYERS) {
        printf("New Player successfully connected\n");

        connected_players++;
        int player_id = connected_players;

        players[player_id].opponent_id = -1;
        players[player_id].paddle_y_pos = rdata[1];
        players[player_id].ball_x_pos = rdata[2];
        players[player_id].ball_y_pos = rdata[3];
        players[player_id].address = client_addr;

        int16_t sdata[4];
        for (int i = 0; i < 4; i++) sdata[i] = 0;
        sdata[0] = player_id;
        sendto(sock_server, sdata, sizeof(int16_t) * 4, 0,
               (struct sockaddr*)&client_addr, addr_size);

        ready.push(player_id);
        matchmaking(rdata, sock_server);

      } else {
        printf("Server has reached MAX_PLAYERS limit\n");
        int16_t sdata[4];
        for (int i = 0; i < 4; i++) sdata[i] = 0;
        sdata[0] = -2;
        sendto(sock_server, sdata, sizeof(int16_t) * 4, 0,
               (struct sockaddr*)&client_addr, addr_size);
      }
    } else {
      if (players[rdata[0]].opponent_id != -1) {
        players[rdata[0]].paddle_y_pos = rdata[1];
        players[rdata[0]].ball_x_pos = rdata[2];
        players[rdata[0]].ball_y_pos = rdata[3];

        int16_t sdata[4];
        for (int i = 0; i < 4; i++) sdata[i] = 0;

        sdata[0] = players[rdata[0]].opponent_id;
        sdata[1] = players[sdata[0]].paddle_y_pos;
        sdata[2] = players[sdata[0]].ball_x_pos;
        sdata[3] = players[sdata[0]].ball_y_pos;

        sendto(sock_server, sdata, sizeof(int16_t) * 4, 0,
               (struct sockaddr*)&client_addr, addr_size);
      }
    }
  }
}

int main(void) {
  // Server init
  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(PORT);

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
  // set recvfrom as non-blocking
  if (fcntl(sock_server, F_SETFL, O_NONBLOCK, 1) == -1) {
    perror("failed to set non-blocking\n");
    exit(EXIT_FAILURE);
  }

  getServerIP();

  // no connected players
  connected_players = -1;

  pthread_t thread_id_server;

  pthread_create(&thread_id_server, NULL, listen, &sock_server);

  printf("Use Enter key to close server\n");
  char c;
  while ((c = getchar()) != '\n') {
    // Server remains active
  }

  close(sock_server);
  pthread_cancel(thread_id_server);

  return 0;
}