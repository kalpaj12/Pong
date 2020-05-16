#include "network.hpp"

Network::Network() {
  ball_handler = false;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  serverAddress.sin_family = AF_INET;
  std::string server_address;

  std::cout << "Enter Server IP:" << std::endl;
  std::cin >> server_address;

  serverAddress.sin_addr.s_addr = inet_addr(server_address.c_str());
  serverAddress.sin_port = htons(1337);

  // Ready to connect
  sendServer();

  // Get my_id:
  recvfrom(sock, rdata, sizeof(int16_t) * 4, 0,
           (struct sockaddr *)&serverAddress, &len);
  printf("The server echo is:\n");
  for (int i = 0; i < 4; i++) printf("%d ", rdata[i]);
  printf("\n");

  if (rdata[0] == -2) {
    perror("Server full!");
    exit(EXIT_FAILURE);
  }
  my_player_id = rdata[0];

  // Get opponent_id:
  recvfrom(sock, rdata, sizeof(int16_t) * 4, 0,
           (struct sockaddr *)&serverAddress, &len);
  printf("The server echo is:\n");
  for (int i = 0; i < 4; i++) printf("%d ", rdata[i]);
  printf("\n");
  opponent_player_id = rdata[0];

  if (rdata[1] == 1) {
    std::cout << "You are the Ball Handler" << std::endl;
    ball_handler = true;
  }
}

Network::~Network() { close(sock); }

void Network::sendServer() {
  sendto(sock, sdata, sizeof(int16_t) * 4, 0, (struct sockaddr *)&serverAddress,
         len);
}

void Network::recServer() {
  recvfrom(sock, rdata, sizeof(int16_t) * 4, 0,
           (struct sockaddr *)&serverAddress, &len);
}

void Network::setrdata(int16_t data[]) {
  for (int i = 0; i < 4; i++) {
    sdata[i] = data[i];
  }
}