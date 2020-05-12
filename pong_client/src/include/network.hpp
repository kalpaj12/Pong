#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

class Network {
 public:
  Network();
  ~Network();

  void setrdata(int16_t data[]);

  void sendServer();
  void recServer();

  int sock;
  struct sockaddr_in serverAddress;
  socklen_t len = sizeof(serverAddress);

  int my_player_id;
  int opponent_player_id;

  int16_t sdata[4] = {-1, 0, 0, 0};
  int16_t rdata[4];
};
