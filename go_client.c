#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3033
#define BUFFER_SIZE 100
#define CORRUPT_PKT 3 // Simulated packet corruption

int main() {
    int sockfd, currentPacket, firstTime = 1;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    printf("Connecting to server...\n");
    connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Send initial request
    send(sockfd, "REQUEST", BUFFER_SIZE, 0);

    while (1) {
        // Receive packet
        recv(sockfd, buffer, BUFFER_SIZE, 0);
        currentPacket = atoi(buffer);
        printf("Received Packet: %d\n", currentPacket);

        if (currentPacket == CORRUPT_PKT && firstTime) {  
            printf("*** Simulating packet loss. Requesting retransmission of Packet %d.\n", currentPacket);
            send(sockfd, "R1", BUFFER_SIZE, 0);
            firstTime = 0;
        } else {
            // Packet received successfully
            printf("Packet %d received successfully. Sending ACK.\n", currentPacket);
            sprintf(buffer, "A%d", currentPacket);
            send(sockfd, buffer, BUFFER_SIZE, 0);
        }

        if (currentPacket == 9) break; // Stop when the last packet is received
    }

    printf("All packets received. Closing connection.\n");
    close(sockfd);
    return 0;
}
