#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 3033
#define MAX_PKT 10
#define WINDOW_SIZE 4
#define BUFFER_SIZE 100

void itoa(int num, char str[]) {
    sprintf(str, "%d", num);
}

int main() {
    int sockfd, newSockFd, windowStart = 1, windowEnd = WINDOW_SIZE;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server, sizeof(server));
    listen(sockfd, 1);
    newSockFd = accept(sockfd, (struct sockaddr*)&client, &len);
    
    recv(newSockFd, buffer, BUFFER_SIZE, 0);
    fcntl(newSockFd, F_SETFL, O_NONBLOCK);

    printf("Client connected. Sending packets...\n");

    for (int pkt = windowStart; pkt < MAX_PKT; pkt++) {
        itoa(pkt, buffer);
        send(newSockFd, buffer, BUFFER_SIZE, 0);
        printf("Packet Sent: %d\n", pkt);
        
        recv(newSockFd, buffer, BUFFER_SIZE, 0);
        if (buffer[0] == 'R') { // Retransmit request
            int resendPkt = atoi(&buffer[1]);
            printf("Retransmitting packet: %d\n", resendPkt);
            itoa(resendPkt, buffer);
            send(newSockFd, buffer, BUFFER_SIZE, 0);
            pkt = resendPkt - 1;
        } else if (buffer[0] == 'A') { // Acknowledgment
            int ackPkt = atoi(&buffer[1]);
            printf("ACK received for %d, sliding window\n", ackPkt);
            windowStart = ackPkt + 1;
            windowEnd = windowStart + WINDOW_SIZE - 1;
        }
    }

    printf("Transmission complete. Closing connection.\n");
    close(newSockFd);
    close(sockfd);
    return 0;
}
