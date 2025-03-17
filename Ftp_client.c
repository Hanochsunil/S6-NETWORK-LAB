#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3075
#define BUFFER_SIZE 1024

int main() {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], filename[100];
    FILE *file;
    int choice;

    // Create socket
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    while (1) {
        printf("\n1. Upload a file (PUT)\n2. Download a file (GET)\n3. Exit (BYE)\nEnter choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline left by scanf

        switch (choice) {
            case 1: // Upload File (PUT)
                printf("Enter filename to upload: ");
                scanf("%s", filename);

                file = fopen(filename, "r");
                if (!file) {
                    printf("File doesn't exist!\n");
                    break;
                }

                send(client_sock, "PUT", BUFFER_SIZE, 0);
                send(client_sock, filename, BUFFER_SIZE, 0);

                while (fgets(buffer, BUFFER_SIZE, file)) {
                    send(client_sock, buffer, BUFFER_SIZE, 0);
                }
                
                send(client_sock, "END$", BUFFER_SIZE, 0);
                fclose(file);
                printf("File %s uploaded.\n", filename);
                break;

            case 2: // Download File (GET)
                printf("Enter filename to download: ");
                scanf("%s", filename);

                send(client_sock, "GET", BUFFER_SIZE, 0);
                send(client_sock, filename, BUFFER_SIZE, 0);

                recv(client_sock, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, "404") == 0) {
                    printf("File not found on server.\n");
                    break;
                }

                file = fopen(filename, "w");
                while (1) {
                    recv(client_sock, buffer, BUFFER_SIZE, 0);
                    if (strcmp(buffer, "END$") == 0) break;
                    fprintf(file, "%s", buffer);
                }
                
                fclose(file);
                printf("File %s downloaded.\n", filename);
                break;

            case 3: // Exit
                send(client_sock, "BYE", BUFFER_SIZE, 0);
                close(client_sock);
                printf("Client disconnected.\n");
                exit(0);
        }
    }
}
