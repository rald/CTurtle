#ifndef GL2D_H
#define GL2D_H

#include <GL/glfw.h>

#include <math.h>

// mingw header excluding GL_BGRA in windows
#ifndef GL_BGRA
#define GL_BGRA  0x80E1
#endif

#define ARGB_A(u) (((u) >> 24) & 0x000000FF)
#define ARGB_R(u) (((u) >> 16) & 0x000000FF)
#define ARGB_G(u) (((u) >> 8)  & 0x000000FF)
#define ARGB_B(u) (((u) >> 0)  & 0x000000FF)
#define RGBA( r, g, b, a ) ((a) << 24 | (r) << 16 | (g) << 8 | (b) )
#define GL2D_RGBA( r, g, b, a ) RGBA( b, g, r, a)

typedef enum GL2D_BLEND_MODE GL2D_BLEND_MODE;

enum GL2D_BLEND_MODE {
	GL2D_TRANS = 0,
	GL2D_SOLID,
	GL2D_BLENDED,
	GL2D_ALPHA,
	GL2D_BLACK,
	GL2D_BLEND_MODE_MAX
};

void glBlendMode( const GL2D_BLEND_MODE mode );

void Init2D(int w,int h) ;

void glClearScreen();

void glPutPixel( int x1, int y1, const GLuint color );
void glLine( int x1, int y1, int x2, int y2, const GLuint color );
void glBox( int x1, int y1, int x2, int y2, const GLuint color );
void glBoxFilled( int x1, int y1, int x2, int y2, const GLuint color );
void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color );
void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color );
void glEllipse(int x, int y, int a, int b, int degrees, const GLuint color);
void glEllipseFilled(int x, int y, int a, int b, int degrees, const GLuint color);



#ifdef GL2D_IMPLEMENTATION_H

void glBlendMode( const GL2D_BLEND_MODE mode ) {
  switch ( mode ) {
   	case GL2D_TRANS:
			glDisable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			break;
		case GL2D_SOLID:
			glDisable( GL_BLEND );
			glDisable( GL_ALPHA_TEST );
			break;
		case GL2D_BLENDED:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			break;
		case GL2D_BLACK:
			glEnable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
			glBlendFunc( GL_ZERO,GL_ONE_MINUS_SRC_ALPHA );
			break;
		default:
			glDisable( GL_BLEND );
			glEnable( GL_ALPHA_TEST );
	}
}

static void init2D(int w,int h) {

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE);

  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glDisable( GL_DEPTH_TEST );

  glEnable( GL_ALPHA_TEST );
  glAlphaFunc( GL_GREATER, 0 );

  glDisable( GL_STENCIL_TEST );
  glDisable( GL_TEXTURE_2D );
  glDisable( GL_LIGHTING );
  glDisable( GL_LOGIC_OP );
  glDisable( GL_DITHER );
  glDisable( GL_FOG );

  glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glHint( GL_LINE_SMOOTH_HINT , GL_NICEST );

  glPointSize( 1.0 );
  glLineWidth( 1.0 );


  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glOrtho( 0, w, h, 0, -1, 1 );


  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  glTranslatef( 0.375, 0.375, 0.0 );


}

void glScreen(const char *title,int w,int h) {

	glfwOpenWindowHint( GLFW_WINDOW_NO_RESIZE, GL_TRUE );

	if( !glfwOpenWindow( w,h, 0,0,0,0,0,0, GLFW_WINDOW ) ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

  glfwSetWindowTitle( title );

	init2D(w,h);

	glClearScreen();

}

void glClearScreen() {
	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void glPutPixel( int x1, int y1, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

  glColor4ubv((GLubyte*)(&color));

  glBegin( GL_POINTS );
	  glVertex2i( x1, y1 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glLine( int x1, int y1, int x2, int y2, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

	glColor4ubv((GLubyte*)(&color));

	glBegin( GL_LINES );
    glVertex2i( x1, y1 );
    glVertex2i( x2, y2 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glBox( int x1, int y1, int x2, int y2, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

	glColor4ubv((GLubyte*)(&color)) ;

	glBegin( GL_LINE_STRIP );
  	glVertex2i( x1, y1 );
  	glVertex2i( x2, y1 );
  	glVertex2i( x2, y2 );
  	glVertex2i( x1, y2 );
  	glVertex2i( x1, y1 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glBoxFilled( int x1, int y1, int x2, int y2, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

	glColor4ubv ( (GLubyte*)(&color)) ;

	x2++;
	y2++;

	glBegin( GL_QUADS );
		glVertex2i	( x1,y1 );
		glVertex2i	( x1,y2 );
		glVertex2i	( x2,y2 );
		glVertex2i	( x2,y1 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glTriangle( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

  glColor4ubv ( (GLubyte*)(&color)) ;

  glBegin( GL_LINE_STRIP );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y2 );
		glVertex2i( x3, y3 );
		glVertex2i( x1, y1 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glTriangleFilled( int x1, int y1, int x2, int y2, int x3, int y3, const GLuint color ) {
  glDisable( GL_TEXTURE_2D );

  glColor4ubv ( (GLubyte*)(&color)) ;

  glBegin( GL_TRIANGLES );
		glVertex2i( x1, y1 );
		glVertex2i( x2, y2 );
		glVertex2i( x3, y3 );
	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glEllipse(int x, int y, int a, int b, int degrees, const GLuint color) {

	// these constants decide the quality of the ellipse
	const float  pi = M_PI;
	const float  twopi  = 2 * pi;   //        two pi (radians in a circle)
	const int face_length  = 8;     //        approx. face length in pixels
	const int max_faces = 256;      //        maximum number of faces in ellipse
	const int min_faces = 16;       //        minimum number of faces in ellipse

	// approx. ellipse circumference (hudson's method)
	float h = ( a-b*a-b ) / (float)( a+b*a+b );
	float circumference = 0.25 * pi * (a+b) * (3* (1+h*0.25)+1 / (float)(1-h*0.25));

	// number of faces in ellipse
	int num_faces = circumference/face_length;

	// clamp number of faces
	if( num_faces > max_faces ) num_faces = max_faces;
	if( num_faces < min_faces ) num_faces = min_faces;

	// keep number of faces divisible by 4
	num_faces -= (num_faces & 3);

	// precalc cosine theta
	float angle = degrees * M_PI /180.0;
	float s   = sin(twopi/(float)num_faces);
	float c   = cos(twopi/(float)num_faces);
	float xx  = 1;
	float yy  = 0;
	float xt  = 0;
	float ax  = cos(angle);
	float ay  = sin(angle);

	// draw ellipse
  glDisable( GL_TEXTURE_2D );

	glColor4ubv( (GLubyte*)(&color) );

  int i;
	glBegin( GL_LINE_LOOP );

		for( i = 0; i < num_faces; i++ ) {
		  xt = xx;
			xx = c * xx - s * yy;
			yy = s * xt + c * yy;
			glVertex2f( x+a*xx*ax-b*yy*ay, y+a*xx*ay+b*yy*ax );
		}

		glVertex2f( x+a*ax, y+a*ay );

	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

void glEllipseFilled(int x, int y, int a, int b, int degrees, const GLuint color) {

	// these constants decide the quality of the ellipse
	const float  pi = M_PI;
	const float  twopi  = 2 * pi;   //        two pi (radians in a circle)
	const int face_length  = 8;     //        approx. face length in pixels
	const int max_faces = 256;      //        maximum number of faces in ellipse
	const int min_faces = 16;       //        minimum number of faces in ellipse

	// approx. ellipse circumference (hudson's method)
	float h = ( a-b*a-b ) / (float)( a+b*a+b );
	float circumference = 0.25 * pi * (a+b) * (3* (1+h*0.25)+1 / (float)(1-h*0.25));

	// number of faces in ellipse
	int num_faces = circumference/face_length;

	// clamp number of faces
	if( num_faces > max_faces ) num_faces = max_faces;
	if( num_faces < min_faces ) num_faces = min_faces;

	// keep number of faces divisible by 4
	num_faces -= (num_faces & 3);

	// precalc cosine theta
	float angle = degrees * M_PI /180.0;
	float s   = sin(twopi/(float)num_faces);
	float c   = cos(twopi/(float)num_faces);
	float xx  = 1;
	float yy  = 0;
	float xt  = 0;
	float ax  = cos(angle);
	float ay  = sin(angle);


	// draw ellipse
  glDisable( GL_TEXTURE_2D );

	glColor4ubv( (GLubyte*)(&color) );

  int i;
	glBegin( GL_TRIANGLE_FAN );

		for( i = 0; i < num_faces; i++ ) {
		  xt = xx;
			xx = c * xx - s * yy;
			yy = s * xt + c * yy;
			glVertex2f( x+a*xx*ax-b*yy*ay, y+a*xx*ay+b*yy*ax );
		}

		glVertex2f( x+a*ax, y+a*ay );

	glEnd();

  glEnable( GL_TEXTURE_2D );

  glColor4ub( 255, 255, 255, 255 );
}

#endif // GL2D_IMPLEMENTATION_H

#endif // GL2D_H
