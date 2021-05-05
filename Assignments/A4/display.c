#include <unistd.h>
#include <string.h>
#include <X11/Xlib.h>

#include "obstacles.h"
#include "display.h"

#define  VERTEX_RADIUS   5

const int   OBSTACLE_COLOR = 0x7799FF;
const int   BORDER_COLOR = 0x000000;
const int   EDGE_COLOR = 0x000000;
const int   SELECTED_COLOR = 0xDD0000;
const int   VERTEX_COLOR = 0xDD0000;


//Display-related variables
Display *display;
Window   win;
GC       gc;


// Initialize and open the simulator window with size ENV_WIDTH x ENV_HEIGHT.
void initializeWindow() {
  // Open connection to X server
  display = XOpenDisplay(NULL);

  // Create a simple window, set the title and get the graphics context then
  // make is visible and get ready to draw
  win = XCreateSimpleWindow(display,  RootWindow(display, 0), 0, 0,
			    ENV_WIDTH, ENV_HEIGHT, 0, 0x000000, 0xFFFFFF);
  XStoreName(display, win, "Path Planner");
  gc = XCreateGC(display, win, 0, NULL);
  XMapWindow(display, win);
  XFlush(display);
  usleep(20000);  // sleep for 20 milliseconds.
}

// Close the display window
void closeWindow() {
  XFreeGC(display, gc);
  XUnmapWindow(display, win);
  XDestroyWindow(display, win);
  XCloseDisplay(display);
}


// Redraw all the obstacles and the edges and the vertices
void displayEnvironment(Environment *env) {
	// Erase the background 
	XSetForeground(display, gc, 0xFFFFFF);
    XFillRectangle(display, win, gc, 0, 0, ENV_WIDTH, ENV_HEIGHT);

    // Draw all the obstacles
    for (int i=0; i<env->numObstacles; i++) {
		XSetForeground(display, gc, OBSTACLE_COLOR);
		XFillRectangle(display, win, gc, env->obstacles[i].x,ENV_HEIGHT-env->obstacles[i].y, env->obstacles[i].w, env->obstacles[i].h);
		XSetForeground(display, gc, BORDER_COLOR);
		XDrawRectangle(display, win, gc, env->obstacles[i].x,ENV_HEIGHT-env->obstacles[i].y, env->obstacles[i].w, env->obstacles[i].h);
    }

    // Draw all the edges
    XSetForeground(display, gc, EDGE_COLOR);
    for (int i=0; i<env->numVertices; i++) {
		Neighbour *n = env->vertices[i].firstNeighbour;
		while (n != NULL) {
	  		XDrawLine(display, win, gc,
		 		env->vertices[i].x,
		 		ENV_HEIGHT-(env->vertices[i].y),
		 		n->vertex->x,
		 		ENV_HEIGHT-(n->vertex->y));
			n = n->next;
		}
    }
    
    // Draw all the vertices
    for (int i=0; i<env->numVertices; i++) {
		XSetForeground(display, gc, VERTEX_COLOR);
		XFillArc(display, win, gc,
			env->vertices[i].x-VERTEX_RADIUS,
			ENV_HEIGHT-(env->vertices[i].y+VERTEX_RADIUS),
			2*VERTEX_RADIUS, 2*VERTEX_RADIUS, 0, 360*64);
		XSetForeground(display, gc, BORDER_COLOR);
		XDrawArc(display, win, gc,
			env->vertices[i].x-VERTEX_RADIUS,
			ENV_HEIGHT-(env->vertices[i].y+VERTEX_RADIUS),
			2*VERTEX_RADIUS, 2*VERTEX_RADIUS, 0, 360*64);
	}

    XFlush(display);
    usleep(2000);
}

