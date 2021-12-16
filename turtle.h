#ifndef TURTLE_H
#define TURTLE_H

#include <GL/glfw.h>

#include <stdlib.h>
#include <stdbool.h>

#include "util.h"
#include "gl2d.h"



typedef struct Turtle Turtle;

struct Turtle {
	double x,y;
	double heading;
	double size;
	bool isVisible;
	bool isPenDown;
	GLuint penColor;
  unsigned char *pixels;
  double x1,y1,x2,y2;
  double w,h;
};



typedef enum TURTLE_DRAWMODE TURTLE_DRAWMODE;

enum TURTLE_DRAWMODE {
	TURTLE_DRAWMODE_NORMAL=0,
	TURTLE_DRAWMODE_TRACE,
	TURTLE_DRAWMODE_MAX
};

void CreateTurtleWorld(int w,int h);
void DestroyTurtleWorld();
void UpdateTurtleWorld();

Turtle *CreateTurtle(double x, double y, double h, double size, GLuint penColor, bool isVisible);
void DestroyTurtle(Turtle **turtle);



void Move(Turtle *turtle, double distance);
void Turn(Turtle *turtle, double angle);
void Jump(Turtle *turtle, double newX, double newY);
void Hide(Turtle *turtle);
void Show(Turtle *turtle);
void PenUp(Turtle *turtle);
void PenDown(Turtle *turtle);
void PenColor(Turtle *turtle, GLuint penColor);



#ifdef TURTLE_IMPLEMENTATION_H

static int W=0,H=0;

Turtle **turtles=NULL;
size_t nturtles=0;

static double pointsx[]={ 0,-1, 2,-1};
static double pointsy[]={ 0,-1, 0, 1};
static size_t npoints=4;

static void plot(Turtle *turtle, int x1,int y1, TURTLE_DRAWMODE turtleDrawMode);
static void line(Turtle *turtle, int x1,int y1, int x2,int y2, TURTLE_DRAWMODE turtleDrawMode);

static void AddTurtle(size_t *nturtles,Turtle ***turtles,Turtle *turtle);

static void DrawTurtle( Turtle *turtle );
static void EraseTurtle( Turtle *turtle );

static void DrawTurtles(size_t nturtles, Turtle **turtles );
static void EraseTurtles(size_t nturtles, Turtle **turtles );

static void Put( unsigned char *pixels, double x,double y, double w,double h);
static unsigned char *Get( double x,double y, double w,double h );


void CreateTurtleWorld(int w,int h) {

	W=w; H=h;

	if(!glfwInit()) {
		exit( EXIT_FAILURE );
	}

	glScreen("Turtle World",w,h);

}



void DestroyTurtleWorld() {

  for(int i=0;i<nturtles;i++) {
    DestroyTurtle(&turtles[i]);
  }
  free(turtles);
  nturtles=0;

  W=0; H=0;

  glfwTerminate();
}



void UpdateTurtleWorld() {
	glfwSwapBuffers();
}



Turtle *CreateTurtle(double x, double y, double h, double size, GLuint penColor, bool isVisible) {
    Turtle *turtle = malloc(sizeof(*turtle));
    if(turtle) {
      turtle->x = x;
      turtle->y = y;
      turtle->heading = h;
      turtle->size = size;
      turtle->penColor = penColor;
      turtle->isVisible = isVisible;
      turtle->isPenDown = true;
      turtle->pixels = NULL;
      turtle->x1=0;
      turtle->y1=0;
      turtle->x2=0;
      turtle->y2=0;
      turtle->w=0;
      turtle->h=0;

			AddTurtle(&nturtles,&turtles,turtle);

    } else {
        printf("Error: CreateTurtle: malloc: turtle\n");
        exit(1);
    }
    return turtle;
}



void DestroyTurtle(Turtle **turtle) {
  (*turtle)->x=0;
  (*turtle)->y=0;
  (*turtle)->isVisible=false;
  (*turtle)->isPenDown=false;
  (*turtle)->penColor=0;
  (*turtle)->x1=0;
  (*turtle)->y1=0;
  (*turtle)->x2=0;
  (*turtle)->y2=0;
  (*turtle)->w=0;
  (*turtle)->h=0;
  free((*turtle)->pixels);
  (*turtle)->pixels=NULL;
  free(*turtle);
  (*turtle)=NULL;
}



static void AddTurtle(size_t *nturtles,Turtle ***turtles,Turtle *turtle) {
	(*turtles)=realloc(*turtles,sizeof(*turtles)*((*nturtles)+1));
	if(*turtles) {
		(*turtles)[(*nturtles)++]=turtle;
	} else {
		printf("Error: AddTurtle: realloc: turtles\n");
		exit(1);
	}
}



static void DrawTurtle( Turtle *turtle ) {
  double wx[npoints];
  double wy[npoints];
  double cx,cy;

  for (int i = 0; i < npoints; i++) {
    wx[i] = pointsx[i] * turtle->size + turtle->x;
    wy[i] = pointsy[i] * turtle->size + turtle->y;
  }

  cx = wx[0];
  cy = wy[0];

  RotatePoints(npoints, wx,wy, cx,cy, turtle->heading * M_PI / 180.0);

  turtle->x1=wx[0];
  turtle->x2=wx[0];
  turtle->y1=wy[0];
  turtle->y2=wy[0];
  for ( int i = 1; i < npoints; i++ ) {
    if(turtle->x1>wx[i]) turtle->x1=wx[i];
    if(turtle->x2<wx[i]) turtle->x2=wx[i];
    if(turtle->y1>wy[i]) turtle->y1=wy[i];
    if(turtle->y2<wy[i]) turtle->y2=wy[i];
  }

  turtle->x1--;
  turtle->x2++;
  turtle->y1--;
  turtle->y2++;

  turtle->w=turtle->x2-turtle->x1+1;
  turtle->h=turtle->y2-turtle->y1+1;

  turtle->pixels=Get( turtle->x1,turtle->y1, turtle->w,turtle->h );

	if(turtle->isVisible) {
		for(int i=0;i<npoints-1;i++) {
			line(turtle, wx[i],wy[i], wx[i+1],wy[i+1], TURTLE_DRAWMODE_NORMAL);
		}
		line(turtle, wx[npoints-1],wy[npoints-1], wx[0],wy[0], TURTLE_DRAWMODE_NORMAL);
	}

}



static void EraseTurtle(Turtle *turtle) {
  Put(turtle->pixels, turtle->x1,turtle->y1, turtle->w,turtle->h );
}



static void DrawTurtles(size_t nturtles, Turtle **turtles) {
	for(int i=0;i<nturtles;i++) {
		DrawTurtle(turtles[i]);
	}
}



static void EraseTurtles(size_t nturtles, Turtle **turtles) {
	for(int i=0;i<nturtles;i++) {
		EraseTurtle(turtles[i]);
	}
}



static unsigned char *Get( double x,double y, double w,double h ) {
  unsigned char *pixels=calloc( w*h*4, sizeof(*pixels) );
  if(pixels) {
    glReadPixels( x,y, w,h, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
  } else {
    printf("Error: Get: calloc: img\n");
    exit(1);
  }
  return pixels;
}



static void Put( unsigned char *pixels, double x,double y, double w,double h) {
  if(pixels) {

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i( x,y );
    glDrawPixels( w,h, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
}



static void plot(Turtle *turtle, int x1, int y1, TURTLE_DRAWMODE turtleDrawMode) {

	switch(turtleDrawMode) {
	  case TURTLE_DRAWMODE_NORMAL:
      glPutPixel( x1,y1, turtle->penColor );
      break;
	  case TURTLE_DRAWMODE_TRACE:
	    EraseTurtles( nturtles, turtles );
      UpdateTurtleWorld();
      glPutPixel( x1,y1, turtle->penColor );
      turtle->x = x1;
      turtle->y = y1;
      DrawTurtles( nturtles,turtles );
      UpdateTurtleWorld();
      glfwSleep(0.1);
      break;
    default: break;
  }

}



static void line(Turtle *turtle, int x1,int y1, int x2,int y2, TURTLE_DRAWMODE turtleDrawMode) {
  int i, dx, dy, sdx, sdy, dxabs, dyabs, x, y, px, py;

  dx = x2 - x1; /* the horizontal distance of the line */
  dy = y2 - y1; /* the vertical distance of the line */
  dxabs = abs(dx);
  dyabs = abs(dy);
  sdx = sgn(dx);
  sdy = sgn(dy);
  x = dyabs >> 1;
  y = dxabs >> 1;
  px = x1;
  py = y1;

  plot(turtle, px,py, turtleDrawMode);

  if (dxabs >= dyabs) { /* the line is more horizontal than vertical */
    for (i = 0; i < dxabs; i++) {
      y += dyabs;
      if (y >= dxabs) {
        y -= dxabs;
        py += sdy;
      }
      px += sdx;
      plot(turtle, px,py, turtleDrawMode);
    }
  } else { /* the line is more vertical than horizontal */
    for (i = 0; i < dyabs; i++) {
      x += dxabs;
      if (x >= dyabs) {
        x -= dyabs;
        px += sdx;
      }
      py += sdy;
      plot(turtle, px,py, turtleDrawMode);
    }
  }
}



void Move(Turtle *turtle, double distance) {
  double newX = distance * cos(turtle->heading * M_PI / 180) + turtle->x;
  double newY = distance * sin(turtle->heading * M_PI / 180) + turtle->y;
  if(turtle->isPenDown) {
  	line(turtle, turtle->x,turtle->y, newX,newY, TURTLE_DRAWMODE_TRACE);
  }
  turtle->x = newX;
  turtle->y = newY;
}



void Turn(Turtle *turtle, double angle) {
  for (int i = 0; i < fabs(angle); i++) {
    EraseTurtles( nturtles,turtles );
    UpdateTurtleWorld();
    turtle->heading += (angle >= sgn(angle) ? sgn(angle) : angle);
    DrawTurtles( nturtles,turtles );
    UpdateTurtleWorld();
  }
}



void Jump(Turtle *turtle, double newX, double newY) {
  if (turtle->isPenDown) {
    line(turtle, turtle->x,turtle->y, newX,newY, TURTLE_DRAWMODE_TRACE);
  }
  turtle->x = newX;
  turtle->y = newY;
}



void Hide(Turtle *turtle) {
  turtle->isVisible = false;
}



void Show(Turtle *turtle) {
  turtle->isVisible = true;
}



void PenUp(Turtle *turtle) {
  turtle->isPenDown = false;
}



void PenDown(Turtle *turtle) {
  turtle->isPenDown = true;
}



void PenColor(Turtle *turtle, GLuint penColor) {
  turtle->penColor = penColor;
}



#endif // TURTLE_IMPLEMENTATION_H

#endif // TURTLE_H
