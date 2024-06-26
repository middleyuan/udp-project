/*
    This is the client code that sends messages to the server.
        References: 
            * https://www.geeksforgeeks.org/udp-server-client-implementation-c/
            * https://markus-x-buchholz.medium.com/docker-container-networking-c-client-server-app-e9750f003f8
            * https://docker-curriculum.com/#docker-network 
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "message.h"

#define SERVER_IP "172.17.0.1" // this is the default gateway IP address of the bridge network
#define SERVER_PORT 8080

int main() {
    std::cout << "Client starting..." << std::endl;
    // create a socket for user datagram protocol (UDP)
    // AF_INET: IPv4, SOCK_DGRAM: UDP, 0: default protocol (UDP)
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    // set address family to IPv4
    server_addr.sin_family = AF_INET;
    // convert port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    uint32_t sequence_number = 0;
    while (true) {
        Message msg(sequence_number, "Hello from client");
        sendto(sock, &msg, sizeof(msg), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        Message received_msg;
        sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);
        ssize_t received_bytes = recvfrom(sock, &received_msg, sizeof(received_msg), 0, (struct sockaddr*)&from_addr, &from_len);

        if (received_bytes < 0) {
            std::cerr << "Error receiving data" << std::endl;
        } else {
            if (received_msg.sequence_number != sequence_number) {
                std::cout << "Sequence mismatch: expected " << sequence_number << ", got " << received_msg.sequence_number << std::endl;
            }
            std::cout << "Received: " << received_msg.data << " (seq: " << received_msg.sequence_number << ")" << std::endl;
        }

        sequence_number++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 0.01Hz = 100ms
    }

    close(sock);
    return 0;
}