#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 8080
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[]) {
    
    char rec_buffer[1024] = {0};
    
    int server_socket;
    
    // create socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Failed to create socket file descriptor.");
        exit(EXIT_FAILURE);
    }
    
    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    // binf the socket to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind socket file descriptor to defined address.");
        exit(EXIT_FAILURE);
    }
    
    // listen the socket, max pending connection 3
    if (listen(server_socket, 3) < 0) {
        perror("Failed to listen the socket");
        exit(EXIT_FAILURE);
    }
    
    for (;;) {
        // establish a new socket for data-transfer
        int client_socket;
        struct sockaddr_in client_address;
        int addresslen = sizeof(client_address);
        
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&addresslen)) < 0) {
            perror("Failed to accept client socket");
            exit(EXIT_FAILURE);
        }
        
        int rec_size;
        if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
            perror("Failed to receive client message");
            exit(EXIT_FAILURE);
        }
        
        
        while (rec_size > 0) {
            
            printf("Client message: %s\n", rec_buffer);
            
            int send_size = rec_size;
            char send_mesg[send_size];
            // add $$$ in front client message
//            strcpy(send_mesg, "from server: ");
//            strncat (send_mesg, rec_buffer, rec_size);
            
            if (send(client_socket, rec_buffer, send_size, 0) != send_size) {
                perror("Failed to send client message");
                exit(EXIT_FAILURE);
            }
            rec_size = 0;
//            if ((rec_size = recv(client_socket, rec_buffer, 1024, 0)) < 0) {
//                perror("Failed to receive client message");
//                exit(EXIT_FAILURE);
//            }
        }
        
        close(client_socket);
    }
    
    close(server_socket);
    
    return 0;
}
