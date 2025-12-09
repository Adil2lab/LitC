#include "controller-protocol.h"
#include <arpa/inet.h>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::cerr;

void printControllerState(const controller::Packet &packet) {
  cout << "\n=== Packet #" << packet.packetId << "===\n";
  
  /* Buttons */
  cout << "Buttons: ";
  if (packet.buttons & controller::Button::A) cout << "[A] ";
  if (packet.buttons & controller::Button::B) cout << "[B] ";
  if (packet.buttons & controller::Button::X) cout << "[X] ";
  if (packet.buttons & controller::Button::Y) cout << "[Y] ";
  if (packet.buttons & controller::Button::L1) cout << "[L] ";
  if (packet.buttons & controller::Button::R1) cout << "[R] ";
  if (packet.buttons & controller::Button::L2) cout << "[ZL] ";
  if (packet.buttons & controller::Button::R2) cout << "[ZR] ";
  if (packet.buttons & controller::Button::SELECT) cout << "[SELECT] ";
  if (packet.buttons & controller::Button::START) cout << "[START] ";
  if (packet.buttons & controller::Button::LEFT) cout << "[LEFT] ";
  if (packet.buttons & controller::Button::RIGHT) cout << "[RIGHT] ";
  if (packet.buttons & controller::Button::UP) cout << "[UP] ";
  if (packet.buttons & controller::Button::DOWN) cout << "[DOWN] ";
  if (packet.buttons & controller::Button::LEFT_STICK_DOWN) cout << "[LEFT_STICK_DOWN] ";
  if (packet.buttons & controller::Button::RIGHT_STICK_DOWN) cout << "[RIGHT_STICK_DOWN] ";
  if (packet.buttons == 0) cout << "(none) ";
  cout << "\n";

  /* Sticks */
  cout << "Left Stick: X=" << std::setw(4) << (packet.leftX / 327) << ", Y=" << std::setw(4) << (packet.leftY / 327) << "\n";
  cout << "Right Stick: X=" << std::setw(4) << (packet.rightX / 327) << ", Y=" << std::setw(4) << (packet.rightY / 327) << "\n";

  /* Triggers */
  cout << "Left Trigger: " << std::setw(3) << static_cast<int>(packet.leftTrigger) << "\n";
  cout << "Right Trigger: " << std::setw(3) << static_cast<int>(packet.rightTrigger) << "\n";
}

int main() {
  std::cout << "=== Controller Receiver (Linux) ===\n";

  // Create UDP socket
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    std::cerr << "Failed to create socket\n";
    return 1;
  }

  // Bind to port 8888 on all interfaces
  sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8888);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cerr << "Failed to bind to port 8888\n";
    close(sock);
    return 1;
  }

  std::cout << "Listening on port 8888...\n";
  std::cout << "Waiting for controller data...\n\n";

  // Receive loop
  char buffer[1024];
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  while (true) {
    ssize_t received = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
                                (sockaddr *)&client_addr, &client_len);

    if (received > 0) {
      buffer[received] = '\0';

      // Get client IP
      char client_ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

      std::cout << "From " << client_ip << ": " << buffer << std::endl;
    }
  }

  close(sock);
  return 0;
}