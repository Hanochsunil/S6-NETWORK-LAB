#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3075
#define BUFFER_SIZE 1024

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE], filename[100];
    FILE *file;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    listen(server_sock, 5);
    printf("Waiting for connection...\n");

    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
    printf("Client connected.\n");

    while (1) {
        recv(client_sock, buffer, BUFFER_SIZE, 0);
        
        if (strcmp(buffer, "PUT") == 0) {  // File upload
            recv(client_sock, filename, sizeof(filename), 0);
            file = fopen(filename, "w");

            while (1) {
                recv(client_sock, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, "END$") == 0) break;
                fprintf(file, "%s", buffer);
            }

            fclose(file);
            printf("File %s received.\n", filename);
        } 
        
        else if (strcmp(buffer, "GET") == 0) {  // File download
            recv(client_sock, filename, sizeof(filename), 0);
            file = fopen(filename, "r");

            if (!file) {
                strcpy(buffer, "404");
            } else {
                strcpy(buffer, "200");
            }
            send(client_sock, buffer, BUFFER_SIZE, 0);

            if (strcmp(buffer, "200") == 0) {
                while (fgets(buffer, BUFFER_SIZE, file)) {
                    send(client_sock, buffer, BUFFER_SIZE, 0);
                }
                strcpy(buffer, "END$");
                send(client_sock, buffer, BUFFER_SIZE, 0);
                fclose(file);
                printf("File %s sent.\n", filename);
            }
        } 
        
        else if (strcmp(buffer, "BYE") == 0) {  // Exit
            close(client_sock);
            close(server_sock);
            printf("Server shutting down.\n");
            exit(0);
        }
    }
}
