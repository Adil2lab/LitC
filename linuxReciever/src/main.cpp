#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

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
    
    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
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
                                     (sockaddr*)&client_addr, &client_len);
        
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