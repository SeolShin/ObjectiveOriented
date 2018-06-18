#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
//#include "draw_object.h"

/* set up the video format globals */

#ifdef _WIN32
char    *vconf = "Data\\WDM_camera_flipV.xml";
#else
char    *vconf = "";
#endif

static void mainLoop(void);

class CArtoolkit
{
public:

	int outputMode;
	int disp_mode;
	int target_id;
	int thresh;

	CArtoolkit();
	int Capture();

	/* function definitions */
 int    init(void);
 void   cleanup(void);
 void   keyEvent( unsigned char key, int x, int y);
 void   mouseEvent(int button, int state, int x, int y);
 void   motionEvent( int x, int y );

 void   getResultRaw( ARMarkerInfo *marker_info );
 void   getResultQuat( ARMarkerInfo *marker_info );

private:

int             xsize;
int             ysize;
ARParam         cparam;

int             mouse_ox;
int             mouse_oy;
int             mouse_st;
double          a;
double          b;
double          r;

double          target_center[2];
double          target_width;

};


CArtoolkit::CArtoolkit()
{
int             xsize = 0;
int             ysize = 0;
int             thresh = 100;
ARParam         cparam;
int             outputMode = 0;

int             mouse_ox = 0;
int             mouse_oy = 0;
int             mouse_st = 0;
int             disp_mode = 1;
double          a =   0.0;
double          b = -45.0;
double          r = 500.0;

int             target_id = 0;
double          target_center[2] = {0.0, 0.0};
double          target_width = 80.0;
}