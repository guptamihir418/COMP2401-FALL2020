#include <stdio.h>
#include <stdlib.h>

#include "obstacles.h"
#include "display.h"


int main() {
  	Environment   environment;

	// Open a window for display
	initializeWindow();

	// Initialize the environment and add some obstacles
	short xs[7] = {290, 570,  50, 420,  20, 150, 280};
	short ys[7] = {590, 510, 460, 390, 300, 220, 130};
	short ws[7] = {210, 110, 210, 100, 310, 110, 470};
	short hs[7] = {160, 360, 100, 200,  40, 170,  50};
	environment.numObstacles = 7;
	environment.numVertices = 0;
	environment.obstacles = (Obstacle *)malloc(7*sizeof(Obstacle));
	for (int i=0; i<7; i++) {
		environment.obstacles[i].x = xs[i];
		environment.obstacles[i].y = ys[i];
		environment.obstacles[i].w = ws[i];
		environment.obstacles[i].h = hs[i];
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
