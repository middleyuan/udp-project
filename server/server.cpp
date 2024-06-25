/*
    This is the server code that listens for incoming messages from the client.
        Reference: https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/

#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "message.h"

#define SERVER_PORT 8080

int main() {
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
    // allow any incoming connection on the specified port
    server_addr.sin_addr.s_addr = INADDR_ANY;
    // convert port number to network byte order
    server_addr.sin_port = htons(SERVER_PORT);

    // bind the socket to the server address
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << SERVER_PORT << std::endl;

    uint32_t expected_sequence = 0;
    while (true) {
        Message received_msg;
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        // a system call that waits for incoming data on the socket sock. 
        // it reads into received_msg and stores the client's address in client_addr. 
        // received_bytes indicates the number of bytes received.
        ssize_t received_bytes = recvfrom(sock, &received_msg, sizeof(received_msg), 0, (struct sockaddr*)&client_addr, &client_len);

        if (received_bytes < 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        if (received_msg.sequence_number != expected_sequence) {
            std::cout << "Sequence mismatch: expected " << expected_sequence << ", got " << received_msg.sequence_number << std::endl;
        }
        std::cout << "Received: " << received_msg.data << " (seq: " << received_msg.sequence_number << ")" << std::endl;

        // send a response to the client
        Message response(received_msg.sequence_number, "Hello from server");
        sendto(sock, &response, sizeof(response), 0, (struct sockaddr*)&client_addr, client_len);

        expected_sequence = received_msg.sequence_number + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // 0.01Hz = 100ms
    }

    close(sock);
    return 0;
}