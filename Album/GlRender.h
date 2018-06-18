#pragma once

#include "CommonStuff.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <float.h>
#include "Texture.h"
#include "3DLib/Angle.h"
#include "3DLib/Vector.h"

int GLSetLight(GLvoid);
int GLInit(GLvoid);
int GLFine(GLvoid);
BOOL GLSetPixelformat(HDC hdc);
GLvoid GLReSizeScene(GLsizei width, GLsizei height);
int GLDrawAxis();
int GLDrawGlobalLine();
void GLDrawCamera(double trans[3][4]);

static void argSetStencil( int flag );
void argDrawMode2D( int X, int Y );
void argDrawMode3D( void );

#define PIE 3.1415926535897932384626433832795
#define SYLINDER_SEGMENT 6
#define SYLINDER_SEG_ANGLE 360.0f/(float)SYLINDER_SEGMENT
#define MAX_ORANGE_NUM	10
#define MAX_LEAVES_NUM	20
#define BOTTOM_KNAR	4
#define LATCENTER (353942.8+2954.6753)
#define LONGCENTER (1394256.5+3053.2778)

#define IMAGE_NUM 419//15//419
#define FRAME_PER_SECOND 30
#define AREA_LEFTTOP	(1<<28)
#define AREA_TOP		(2<<28)
#define AREA_RIGHTTOP	(3<<28)
#define AREA_RIGHT		(4<<28)
#define AREA_RIGHBOTTOM	(5<<28)
#define AREA_BOTTOM		(6<<28)
#define AREA_LEFTBOTTOM	(7<<28)
#define AREA_LEFT		(8<<28)
#define AREA_THUMBNAIL	(15<<28)

#define gl2DLine(x1,y1,x2,y2) \
	glBegin(GL_LINES);\
	glVertex3d(x1,y1,0.0);\
	glVertex3d(x2,y2,0.0);\
	glEnd();
#define gl2DVector(x,y,dx,dy) \
	glBegin(GL_LINES);\
	glVertex3d(x,y,0.0);\
	glVertex3d(x+dx,y+dy,0.0);\
	glEnd();

typedef struct {
	CVector3D	vPos;
	CAngle		aRot;
	float		fZoom;
	CString		strName;
	WCHAR		strMemo[1024];
	INT			iTime[5];//year/month/day/hour/miniute
	QdTexture	*Tex;
	QdTexture	*TexMemo;
} CPicture;