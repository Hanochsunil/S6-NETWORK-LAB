#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/types.h>
#include<unistd.h>

int main(){
    int serversocket, port, a, fact = 1;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // Create the socket
    serversocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serversocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    printf("Enter the port number: ");
    scanf("%d", &port);
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serversocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        close(serversocket);
        exit(1);
    }

    printf("Waiting for data...\n");

    // Receive data (number) from client
    if (recvfrom(serversocket, &a, sizeof(a), 0, (struct sockaddr*)&client, &len) < 0) {
        perror("Failed to receive data");
        close(serversocket);
        exit(1);
    }

    printf("Number received: %d\n", a);

    // Calculate factorial
    for (int i = 1; i <= a; i++) {
        fact *= i;
    }

    // Send factorial result back to client
    if (sendto(serversocket, &fact, sizeof(fact), 0, (struct sockaddr*)&client, len) < 0) {
        perror("Failed to send data");
        close(serversocket);
        exit(1);
    }

    printf("Factorial sent: %d\n", fact);

    // Close the socket
    close(serversocket);

    return 0;
}

