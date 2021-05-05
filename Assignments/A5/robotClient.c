#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "simulator.h"




// This is the main function that simulates the "life" of the robot
// The code will exit whenever the robot fails to communicate with the server
int main() {
    // ... ADD SOME VARIABLE HERE ... //
    int clientSocket, addrSize, bytesReceived;
    struct sockaddr_in serverAddr;
    char inStr[80];

    unsigned char buffer[80];



    // Set up the random seed
    srand(time(NULL));

    int hasMoved = 0;
    int turnDirection = 0;
    Robot robot;
    int robotID;
    unsigned char xbytes[sizeof(float)];
    unsigned char ybytes[sizeof(float)];
    int receiveCode;
    float receiveX;
    float receiveY;
    unsigned char sign;
    int magnitude;


    // Register with the server
    clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clientSocket < 0){
        printf("ROBOTCLIENT ERROR: Could not open socket.\n");
        exit(-1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons((unsigned short) SERVER_PORT);
    // Send register command to server. Get back response data
    // and store it. If denied registration, then quit.
    addrSize = sizeof(serverAddr);

    sprintf(buffer, "%d", REGISTER);

    printf("ROBOTCLIENT: Registration - Sending \"%s\" to server. \n", buffer);

    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddr, addrSize);
    bytesReceived = recvfrom(clientSocket, buffer, 80, 0, (struct sockaddr *) &serverAddr, &addrSize);
    // printf("bytesReceived: %d\n", bytesReceived);
    buffer[bytesReceived] = 0;
    printf("ROBOTCLIENT: Registration - Got back response \"%s\" from server.\n", buffer);

    if(buffer[0]-'0' != OK){
        printf("ROBOCLIENT: Registration -> DENIED, exiting...\n");
        exit(0);
    }
    else{
        receiveCode = buffer[0]-'0';
    }
    char *delim = " ";
    char *parse = strtok(buffer + 1, delim);
    robotID = atoi(parse);
    for (int i = 0; i < 4; i++){
    parse = strtok(NULL, delim);
    xbytes[i] = atoi(parse);
    }
    for (int i = 0; i < 4; i++){
    parse = strtok(NULL, delim);
    ybytes[i] = atoi(parse);
    }
    parse = strtok(NULL, delim);
    sign = parse[0] - '0';
    parse = strtok(NULL, delim);
    magnitude = atoi(parse);

    memcpy(&robot.x, xbytes, 4);
    memcpy(&robot.y, ybytes, 4);

    if (sign == 0){
        robot.direction = magnitude;
    } 
    else {
        robot.direction = -magnitude;
    }

    // Go into an infinite loop exhibiting the robot behavior
    while (1) {

        usleep(10000);

        ////REMOVE////
    // Check if can move forward

    sprintf(buffer, "%d %u %u %u %u %u %u %u %u %u %u %u", CHECK_COLLISION, robotID,xbytes[0], xbytes[1], xbytes[2], xbytes[3],
    ybytes[0], ybytes[1], ybytes[2], ybytes[3], sign, magnitude);
    // printf("ROBOTCLIENT: Check collision - Sending \"%s\" to server. \n", buffer);
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddr, addrSize);

    // Get response from server.

    bytesReceived = recvfrom(clientSocket, buffer, 80, 0, (struct sockaddr *) &serverAddr, &addrSize);
    // printf("bytesReceived: %d\n", bytesReceived);
    buffer[bytesReceived] = 0;
    // printf("ROBOTCLIENT: Check collision - Got back response \"%s\" from server.\n", buffer);

    // If ok, move forward

    if(buffer[0]-'0' == OK){
        int direction;
        if(sign == 0){
            direction = magnitude;
        }
        else{
            direction = -magnitude;
        }

    robot.x += ROBOT_SPEED * cos(direction);
    robot.y += ROBOT_SPEED * sin(direction);
    hasMoved = 1;
    turnDirection = 0;

    }

    // Otherwise, we could not move forward, so make a turn.
    else if (buffer[0] - '0' == NOT_OK_BOUNDARY || buffer[0] - '0' == NOT_OK_COLLIDE ){

    // If we were turning from the last time we collided, keep
    // turning in the same direction, otherwise choose a random 
    // direction to start turning.
        printf("NOT_OK_BOUNDARY\n");

        if(hasMoved == 1){
            if(turnDirection == 0){
                int n = rand()%2;
                if(n == 1) turnDirection = 1; // CW
                else turnDirection = -1; // CCW
            }
        }

        robot.direction += turnDirection*ROBOT_TURN_ANGLE;

        //handle overturn
        if (robot.direction > 180){
            robot.direction = -180 + (robot.direction % 180);
        } 
        else if (robot.direction < -180){
            robot.direction = 180 + (robot.direction % 180);
        }

        hasMoved = 0;

    }
    else{
        printf("Server is shutting down, exiting...\n");
        close(clientSocket);
        exit(0);
    }
    //update variables to be passed into buffer

    magnitude = abs(robot.direction);
    if(robot.direction >= 0) sign = 0;
    else sign = 1;

    memcpy(xbytes, &robot.x , sizeof(robot.x ));
    memcpy(ybytes, &robot.y , sizeof(robot.y));

        
    // Send update to server
    sprintf(buffer, "%d %u %u %u %u %u %u %u %u %u %u %u", STATUS_UPDATE, robotID,xbytes[0], xbytes[1], xbytes[2], xbytes[3],
    ybytes[0], ybytes[1], ybytes[2], ybytes[3], sign, magnitude);
    // printf("ROBOTCLIENT: Status update - Sending \"%s\" to server. \n", buffer);
    sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddr, addrSize);

    }

    close(clientSocket);
}
