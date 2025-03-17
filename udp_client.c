#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<unistd.h>

int main(){
    int clientsocket, port, a, fact;
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // Create a UDP socket
    clientsocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientsocket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server.sin_family = AF_INET;  // Set the family to IPv4
    printf("Enter the server port number: ");
    scanf("%d", &port);

    server.sin_port = htons(port);  // Set port number
    server.sin_addr.s_addr = INADDR_ANY;  // Bind to any available network interface

    printf("Connected to the server\n");

    // Input the number for which the factorial is to be calculated
    printf("Enter a number to calculate its factorial: ");
    scanf("%d", &a);

    // Send the number to the server
    if (sendto(clientsocket, &a, sizeof(a), 0, (struct sockaddr*)&server, len) < 0) {
        perror("Failed to send data");
        close(clientsocket);
        exit(1);
    }

    // Receive the factorial from the server
    if (recvfrom(clientsocket, &fact, sizeof(fact), 0, (struct sockaddr*)&server, &len) < 0) {
        perror("Failed to receive data");
        close(clientsocket);
        exit(1);
    }

    // Print the received factorial
    printf("Factorial of %d is %d\n", a, fact);

    // Close the socket
    close(clientsocket);

    return 0;
}

