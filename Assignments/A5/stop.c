#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "simulator.h"

int main() {
    // ... ADD SOME VARIABLES HERE ... //
    int clientSocket, addrSize, bytesReceived;
    struct sockaddr_in serverAddr;
    char inStr[80];
    char buffer[80];

    //create socket
    clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket < 0) {
        printf("*** Client Error: could not open socket.\n");
        exit(-1);
    }

    // set up server address

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons((unsigned short)SERVER_PORT);

    // Register with the server
    // ... WRITE SOME CODE HERE ... //
    addrSize = sizeof(serverAddr);
    sprintf(buffer, "%d", STOP);

    printf("STOP: Sending \"%s\" to server. \n", buffer);
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addrSize);
    // bytesReceived = recvfrom(clientSocket, buffer, 80, 0, (struct sockaddr *) &serverAddr, &addrSize);
    // buffer[bytesReceived] = 0;
    // printf("STOP: Got back response \"%s\" from server.\n", buffer);

    close(clientSocket);
    printf("STOP: Shutting down. \n");
}