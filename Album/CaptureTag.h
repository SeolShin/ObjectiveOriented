#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

//#include "DetectObject.h"

/* set up the video format globals */

#ifdef _WIN32
char    *vconf = "Data\\WDM_camera_flipV.xml";
#else
char    *vconf = "";
#endif

class CaptureTag
{
public:
int             xsize;
int             ysize;
int             thresh;
ARParam         cparam;
int             outputMode;

int             mouse_ox;
int             mouse_oy;
int             mouse_st;
int             disp_mode;
double          a;
double          b;
double          r;

int             target_id;
double          target_center[2];
double          target_width;

/* function definitions */
 int    init(void);
 void   cleanup(void);
 void   keyEvent( unsigned char key, int x, int y);
 void   mouseEvent(int button, int state, int x, int y);
 void   motionEvent( int x, int y );
 void   mainLoop(void);

 void   getResultRaw( ARMarkerInfo *marker_info );
 void   getResultQuat( ARMarkerInfo *marker_info );

int StartCapture();

};