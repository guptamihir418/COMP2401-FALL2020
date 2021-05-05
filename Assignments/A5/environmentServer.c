#include <math.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "simulator.h"

Environment environment;  // The environment that contains all the robots
// Handle client requests coming in through the server socket.  This code should run
// indefinitiely.  It should repeatedly grab an incoming messages and process them.
// The requests that may be handled are STOP, REGISTER, CHECK_COLLISION and STATUS_UPDATE.
// Upon receiving a STOP request, the server should get ready to shut down but must
// first wait until all robot clients have been informed of the shutdown.   Then it
// should exit gracefully.
void *handleIncomingRequests(void *e) {
    char online = 1;
    // ... ADD SOME VARIABLE HERE... //
    Environment *env = e;

    env->shutDown = 0;
    env->numRobots = 0;

    int moveTurn[MAX_ROBOTS][2] = {0};
    // [ [hasMoved, turnDirection], ... ]
    // where turnDirection is 1 when turning CW and -1 when turning CCW

    char shuttingDown = 0;
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr, clientAddrs[MAX_ROBOTS];
    int status, addrSize, bytesReceived;
    fd_set readfds, writefds;
    unsigned char buffer[80];
    unsigned char response[80];

    // Initialize the server
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket < 0) {
        printf("*** Server Error: could not open socket.\n");
        exit(-1);
    }

    // set up server address

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons((unsigned short)SERVER_PORT);

    // bind the server
    status = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (status < 0) {
        printf("*** Server Error: could not bind socket.\n");
        exit(-1);
    }

    unsigned char xbytes[sizeof(float)];
    unsigned char ybytes[sizeof(float)];
    unsigned char sign;
    unsigned char magnitude;

    // Wait for clients now
    while (online) {
        // ... WRITE YOUR CODE HERE ... //
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);
        FD_ZERO(&writefds);
        FD_SET(serverSocket, &writefds);

        status = select(FD_SETSIZE, &readfds, &writefds, NULL, NULL);

        if (status == 0) {
            printf("SERVER: Waiting for client ready...\n");
            // time out occur, no client ready
        }

        else if (status < 0) {
            printf("*** SERVER ERROR: Could not select socket.\n");
            exit(-1);
        } else {
            addrSize = sizeof(clientAddr);
            bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
                                     (struct sockaddr *)&clientAddr, &addrSize);
        }

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            printf("SERVER: Received client request: %s\n", buffer);
        }

        printf("SERVER: sending OK to client\n");
        // sendto(serverSocket, "OK", strlen("OK"), 0, (struct sockaddr *) &clientAddr, addrSize);

        // if(strcmp(buffer, "STOP") == 0) break;

        ////// REGISTRATION ///////

        if (atoi(buffer) == REGISTER) {
            if (++env->numRobots > MAX_ROBOTS) {
                printf("TOO MANY ROBOTS1 NO REGISTRATION..\n");
                sprintf(response, "%d", NOT_OK);
                --env->numRobots;
            } else {
                printf("%d/20 robots\n", env->numRobots);
                clientAddrs[env->numRobots - 1] = clientAddr;
                env->robots[env->numRobots - 1].x = rand() % ((ENV_SIZE - ROBOT_RADIUS) - (ROBOT_RADIUS)) + ROBOT_RADIUS;
                env->robots[env->numRobots - 1].y = rand() % ((ENV_SIZE - ROBOT_RADIUS) - (ROBOT_RADIUS)) + ROBOT_RADIUS;

                if (env->numRobots > 1) {
                    char colliding = 1;

                    while (colliding) {
                        for (int i = 0; i < env->numRobots - 1; i++) {
                            double centerDistance = sqrt(pow((env->robots[env->numRobots - 1].x - env->robots[i].x), 2) +
                                                         pow((env->robots[env->numRobots - 1].y - env->robots[i].y), 2));

                            double radiusSum = (double)2 * ROBOT_RADIUS;
                            // if centerDistance equals radiusSum, circles touch
                            // if centerDistance is less than radiusSum, circles intersect

                            if (centerDistance <= radiusSum) {
                                // not_Ok_collide
                                env->robots[env->numRobots - 1].x = rand() % ((ENV_SIZE - ROBOT_RADIUS) - (ROBOT_RADIUS)) + ROBOT_RADIUS;
                                env->robots[env->numRobots - 1].y = rand() % ((ENV_SIZE - ROBOT_RADIUS) - (ROBOT_RADIUS)) + ROBOT_RADIUS;
                                i = 0;
                            } else if (i == env->numRobots - 2) {
                                // does not collide with last robot
                                colliding = 0;
                            }
                        }
                    }
                }
                env->robots[env->numRobots - 1].direction = rand() % (180 - (-180)) + (-180);
                // printf("x: %f\n", env->robots[env->numRobots - 1].x);
                // printf("y: %f\n", env->robots[env->numRobots - 1].y);
                magnitude = abs(env->robots[env->numRobots - 1].direction);

                if (env->robots[env->numRobots - 1].direction >= 0) {
                    sign = 0;
                } else
                    sign = 1;

                memcpy(xbytes, &env->robots[env->numRobots - 1].x, sizeof(env->robots[env->numRobots - 1].x));
                memcpy(ybytes, &env->robots[env->numRobots - 1].y, sizeof(env->robots[env->numRobots - 1].y));

                // printf("magnitude: %u\n", magnitude);
                sprintf(response, "%d %u %u %u %u %u %u %u %u %u %u %u", OK, env->numRobots, xbytes[0], xbytes[1], xbytes[2], xbytes[3],
                        ybytes[0], ybytes[1], ybytes[2], ybytes[3], sign, magnitude);
            }

            // printf("SERVER: Sending \"%s\" to client\n", response);
            sendto(serverSocket, response, strlen(response), 0, (struct sockaddr *)&clientAddr, addrSize);
        }

        else if (buffer[0] - '0' == CHECK_COLLISION) {
            // printf("=========== CHECK_COLLISION ===========\n");
            if (shuttingDown) {
                // LOST_CONTACT
                env->numRobots--;
                if (env->numRobots == 0 && shuttingDown == 1) {
                    env->shutDown = 1;
                    online = 0;
                }
                sprintf(response, "%d", LOST_CONTACT);
                // printf("remaining robots: %d\n", env->numRobots);
            } else {
                sprintf(response, "%d", OK);

                // RECIEVING

                int robotld;
                float x, y;
                int direction;
                printf("print check before\n");
                char *delim = " ";
                char *parse = strtok(buffer + 1, delim);
                robotld = atoi(parse);
                printf("print check between\n");
                for (int i = 0; i < 4; i++) {
                    parse = strtok(NULL, delim);
                    xbytes[i] = atoi(parse);
                    printf("print check between loop\n");
                }
                printf("print check after\n");
                for (int i = 0; i < 4; i++) {
                    parse = strtok(NULL, delim);
                    ybytes[i] = atoi(parse);
                }
                parse = strtok(NULL, delim);
                sign = parse[0] - '0';

                parse = strtok(NULL, delim);
                magnitude = atoi(parse);

                memcpy(&x, xbytes, 4);
                memcpy(&y, ybytes, 4);

                if (sign == 0) {
                    direction = magnitude;
                } else {
                    direction = -magnitude;
                }

                float newX, newY;

                newX = x + ROBOT_SPEED * cos(direction);
                newY = y + ROBOT_SPEED * sin(direction);

                if (newX + ROBOT_RADIUS > 600 || newX - ROBOT_RADIUS < 0 || newY + ROBOT_RADIUS > 600 || newY - ROBOT_RADIUS < 0) {
                    // Not ok boundary
                    sprintf(response, "%d", NOT_OK_BOUNDARY);
                } else {
                    for (int i = 0; i < env->numRobots; ++i) {
                        if (i + 1 == robotld)
                            continue;

                        // distance between centers of two circles
                        double centerDistance = sqrt(pow((newX - env->robots[i].x), 2) +
                                                     pow((newY - env->robots[i].y), 2));

                        double radiusSum = (double)2 * ROBOT_RADIUS;

                        // if center distance equals radiusSum, circles touch
                        // if its less, circles intersect

                        if (centerDistance <= radiusSum) {
                            // not ok collide

                            sprintf(response, "%d", NOT_OK_COLLIDE);
                        }
                    }
                }
            }

            sendto(serverSocket, response, strlen(response), 0, (struct sockaddr *)&clientAddr, addrSize);

        }

        // STATUS UPDATE

        else if (buffer[0] - '0' == STATUS_UPDATE) {
            int robotID;
            char *delim = " ";
            char *parse = strtok(buffer + 1, delim);
            robotID = atoi(parse);
            printf("chk2\n");
            for (int i = 0; i < 4; i++) {
                printf("chk2\n");
                parse = strtok(NULL, delim);
                xbytes[i] = atoi(parse);
            }
            for (int i = 0; i < 4; i++) {
                parse = strtok(NULL, delim);
                ybytes[i] = atoi(parse);
            }
            parse = strtok(NULL, delim);
            sign = parse[0] - '0';
            parse = strtok(NULL, delim);
            magnitude = atoi(parse);

            memcpy(&env->robots[robotID - 1].x, xbytes, 4);
            memcpy(&env->robots[robotID - 1].y, ybytes, 4);

            if (sign == 0) {
                env->robots[robotID - 1].direction = magnitude;
            } else {
                env->robots[robotID - 1].direction = -magnitude;
            }
        }

        else if (atoi(buffer) == STOP) {
            shuttingDown = 1;

            if (env->numRobots == 0) {
                printf("No More robots, shutting down..\n");
                env->shutDown = 1;
                online = 0;
            }
        }
    }
    // ... WRITE ANY CLEANUP CODE HERE ... //
    printf("SERVER: SHutting down..\n");
    close(serverSocket);
}

int main() {
    // So far, the environment is NOT shut down
    environment.shutDown = 0;

    // Set up the random seed
    srand(time(NULL));
    printf("Creating threads...\n");
    // Spawn an infinite loop to handle incoming requests and update the display
    pthread_t drawThread, requestThread;
    pthread_create(&drawThread, NULL, redraw, &environment);
    pthread_create(&requestThread, NULL, handleIncomingRequests, &environment);

    printf("Threads created. \n");

    // wait for the update and draw threads to complete
    pthread_join(requestThread, NULL);
    printf("requestthread is completed.\n");
    pthread_join(drawThread, NULL);
    printf("drawThread is completed.\n");
    printf("Threads have completed. Exiting...\n");
}
