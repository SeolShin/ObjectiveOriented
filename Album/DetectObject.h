#ifndef DRAW_OBJECT_H
#define DRAW_OBJECT_H
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <strings.h>
#endif
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/matrix.h>
#include "CaptureTag.h"

class DetectObject
{

public:
	static void setup_light( void );
	static void draw_camera( double trans[3][4] );
	static int  draw_object( char *name, double gl_para[16], int xwin, int ywin );
	static void get_trans( double a, double b, double r, double trans[3][4] );
	static void draw_axis( void );
	
	void print_string( char *string );
	int  draw( char name[], double trans[3][4], int xwin, int ywin );
	int  draw_exview( double a, double b, double r, double trans[3][4], int xwin, int ywin );

};

