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

int main() {
  int sd;
  struct sockaddr_in address;
  sd = socket(AF_INET, SOCK_DGRAM, 0);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = htons(8080);
  int16_t sdata[4] = {-1, 0, 0, 0};
  int16_t rdata[4];

  socklen_t len = sizeof(address);
  int m = sendto(sd, sdata, sizeof(int16_t) * 4, 0, (struct sockaddr *)&address,
                 len);
  while (1) {
    int n = recvfrom(sd, rdata, sizeof(int16_t) * 4, 0,
                     (struct sockaddr *)&address, &len);
    printf("[%d] the server echo is:\n", n);
    for (int i = 0; i < 4; i++) printf("%d ", rdata[i]);
    printf("\n");
  }

  close(sd);
  return 0;
}
