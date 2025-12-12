#include "controller-protocol.h"
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;

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
    perror("Failed to create socket");
    return 1;
  }

  // Bind to port 8888 on all interfaces
  sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8888);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    perror("Failed to bind to port 8888");
    close(sock);
    return 1;
  }

  cout << "Listening on port 8888...\n";
  cout << "Waiting for controller data...\n\n";

  // Receive loop
  controller::Packet packet;
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  uint32_t lastPacketId = 0;

  while (true) {
    ssize_t received = recvfrom(sock, &packet, sizeof(packet), 0,
                                reinterpret_cast<sockaddr*>(&client_addr), &client_len);

    if (received == sizeof(packet)) {
      uint16_t expectedChecksum = controller::calculateChecksum(&packet);
      cout << "Received packet with checksum: " << packet.checksum << " (expected: " << expectedChecksum << ")\n";
      if (packet.checksum != expectedChecksum) {
        cout << "Checksum mismatch! : data may be distorted.\n";
        continue;
      }
      if (packet.packetId > lastPacketId + 1) {
        cout << "Packet lost! Lost " << packet.packetId - lastPacketId - 1 << " packets.\n";
        continue;
      }
      lastPacketId = packet.packetId;
      printControllerState(packet);
    } else if (received > 0) {
      cout << "Received invalid packet size: " << received << " bytes\n";
    }
  }

  close(sock);
  return 0;
}