#include <GL/glfw.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define UTIL_IMPLEMENTATION_H
#include "util.h"

#define GL2D_IMPLEMENTATION_H
#include "gl2d.h"

#define TURTLE_IMPLEMENTATION_H
#include "turtle.h"



#define W 320
#define H 200

void DrawStar(Turtle *turtle, double size);

int main(void) {

	int running=GL_TRUE;

	srand(time(NULL));

	CreateTurtleWorld(W,H);

	Turtle *turtle=CreateTurtle(W/2,H/2,0,5,GL2D_RGBA(255,255,255,255),true);

	PenUp(turtle);
	Jump(turtle,(W-50)/2,H/2);
	PenDown(turtle);

	DrawStar(turtle,50);

	while(running) {
		glfwSwapBuffers();
		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
	}

  DestroyTurtleWorld();

	exit( EXIT_SUCCESS );
}

void DrawStar(Turtle *turtle, double size) {
  for (int i = 0; i < 5; i++) {
    Move(turtle, size);
    Turn(turtle, 144);
  }
}

