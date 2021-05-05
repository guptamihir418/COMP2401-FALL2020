#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "obstacles.h"
#include "display.h"


int main() {
	srand(time(NULL));

  	Environment   environment;

	// Open a window for display
	initializeWindow();

	// Initialize the environment and add 50 obstacles
	environment.numObstacles = 50;
	environment.numVertices = 0;
	environment.obstacles = (Obstacle *)malloc(50*sizeof(Obstacle));
	int currentObstacle = 0;
	while(currentObstacle < 50) {
		int randomWidth = (int)(rand()/(double)RAND_MAX*(ENV_WIDTH/3-20)) + 10;
		int randomHeight = (int)(rand()/(double)RAND_MAX*(ENV_HEIGHT/3-20)) + 10;
		int randomX = (int)(rand()/(double)RAND_MAX*(ENV_WIDTH - randomWidth));
		int randomY = (int)(rand()/(double)RAND_MAX*(ENV_HEIGHT - randomHeight) + randomHeight);

		char overlap = 0;
		for (int i=0; i<currentObstacle; i++) {
			// Check if this rectangle overlaps any others
			short topLeftX = randomX;
			short topLeftY = randomY;
			short bottomRightX = topLeftX+randomWidth;
			short bottomRightY = topLeftY-randomHeight;

			if (!((topLeftX > (environment.obstacles[i].x + environment.obstacles[i].w) || environment.obstacles[i].x > bottomRightX) ||
            	(topLeftY < (environment.obstacles[i].y - environment.obstacles[i].h) || environment.obstacles[i].y < bottomRightY)))
				overlap = 1;
		}
		if (!overlap) {
      	    // ok to add since not overlapping
			environment.obstacles[currentObstacle].x = randomX;
			environment.obstacles[currentObstacle].y = randomY;
			environment.obstacles[currentObstacle].w = randomWidth;
			environment.obstacles[currentObstacle].h = randomHeight;
			currentObstacle++;
		}
	}
	displayEnvironment(&environment); getchar(); // Display the environment then wait for user to press ENTER


	// Create the vertices
	printf("\nComplete Graph has %d vertices", createVertices(&environment));	
	displayEnvironment(&environment); getchar(); // Display the environment then wait for user to press ENTER

	// Create all the edges
	printf("Complete Graph has %d edges", createEdges(&environment));
	displayEnvironment(&environment); getchar(); // Display the environment then wait for user to press ENTER

	// Remove invalid edges
	printf("Visibility Graph has %d edges", removeEdges(&environment));
	displayEnvironment(&environment); getchar(); // Display the environment then wait for user to press ENTER

	// Cleanup by freeing all allocated memory
	cleanupEverything(&environment);

	// Close the environment window
	closeWindow();
}
