#include "GLRender.h"
#include <math.h>

//#define WRITEOUT
#ifdef WRITEOUT
	HANDLE		hDebug = NULL;
#endif
CString		strTmp;
DWORD		dwWritten;
//QdTexture	qTex,qTex2,qTex3,qTexDrops;
QdTexture	*qTex	= NULL;
CPicture	*Pics	= NULL;
extern BOOL	bDrawingShadow;
extern float fWind[2];
extern float fWindDir[2];

extern INT iStartYear;
extern INT iPictureNum;


int arUtilMatMul( double s1[3][4], double s2[3][4], double d[3][4] )
{
	int     i, j;

	for( j = 0; j < 3; j++ ) {
		for( i = 0; i < 4; i++) {
			d[j][i] = s1[j][0] * s2[0][i]
			+ s1[j][1] * s2[1][i]
			+ s1[j][2] * s2[2][i];
		}
		d[j][3] += s1[j][3];
	}

	return 0;
}

typedef struct {
	double *m;
	int row;
	int clm;
} ARMat;
ARMat *arMatrixAlloc(int row, int clm)
{
	ARMat *m;

	m = (ARMat *)malloc(sizeof(ARMat));
	if( m == NULL ) return NULL;

	m->m = (double *)malloc(sizeof(double) * row * clm);
	if(m->m == NULL) {
		free(m);
		return NULL;
	}
	else {
		m->row = row;
		m->clm = clm;
	}

	return m;
}
int arMatrixFree(ARMat *m)
{
	free(m->m);
	free(m);

	return 0;
}
static double *minv( double *ap, int dimen, int rowa );

int arMatrixSelfInv(ARMat *m)
{
	if(minv(m->m, m->row, m->row) == NULL) return -1;

	return 0;
}


/********************************/
/*                              */
/*    MATRIX inverse function   */
/*                              */
/********************************/
static double *minv( double *ap, int dimen, int rowa )
{
	double *wap, *wcp, *wbp;/* work pointer                 */
	int i,j,n,ip,nwork;
	int nos[50];
	double epsl;
	double p,pbuf,work;
	double  fabs();

	epsl = 1.0e-10;         /* Threshold value      */

	switch (dimen) {
				case (0): return(NULL);                 /* check size */
				case (1): *ap = 1.0 / (*ap);
					return(ap);                   /* 1 dimension */
	}

	for(n = 0; n < dimen ; n++)
		nos[n] = n;

	for(n = 0; n < dimen ; n++) {
		wcp = ap + n * rowa;

		for(i = n, wap = wcp, p = 0.0; i < dimen ; i++, wap += rowa)
			if( p < ( pbuf = /*fabs*/(*wap)) ) {
				p = pbuf;
				ip = i;
			}
			if (p <= epsl)
				return(NULL);

			nwork = nos[ip];
			nos[ip] = nos[n];
			nos[n] = nwork;

			for(j = 0, wap = ap + ip * rowa, wbp = wcp; j < dimen ; j++) {
				work = *wap;
				*wap++ = *wbp;
				*wbp++ = work;
			}

			for(j = 1, wap = wcp, work = *wcp; j < dimen ; j++, wap++)
				*wap = *(wap + 1) / work;
			*wap = 1.0 / work;

			for(i = 0; i < dimen ; i++) {
				if(i != n) {
					wap = ap + i * rowa;
					for(j = 1, wbp = wcp, work = *wap;
						j < dimen ; j++, wap++, wbp++)
						*wap = *(wap + 1) - work * (*wbp);
					*wap = -work * (*wbp);
				}
			}
	}

	for(n = 0; n < dimen ; n++) {
		for(j = n; j < dimen ; j++)
			if( nos[j] == n) break;
		nos[j] = nos[n];
		for(i = 0, wap = ap + j, wbp = ap + n; i < dimen ;
			i++, wap += rowa, wbp += rowa) {
				work = *wap;
				*wap = *wbp;
				*wbp = work;
			}
	}
	return(ap);
}

int arUtilMatInv( double s[3][4], double d[3][4] )
{
	ARMat       *mat;
	int         i, j;

	mat = arMatrixAlloc( 4, 4 );
	for( j = 0; j < 3; j++ ) {
		for( i = 0; i < 4; i++ ) {
			mat->m[j*4+i] = s[j][i];
		}
	}
	mat->m[3*4+0] = 0; mat->m[3*4+1] = 0;
	mat->m[3*4+2] = 0; mat->m[3*4+3] = 1;
	arMatrixSelfInv( mat );
	for( j = 0; j < 3; j++ ) {
		for( i = 0; i < 4; i++ ) {
			d[j][i] = mat->m[j*4+i];
		}
	}
	arMatrixFree( mat );

	return 0;
}
int arUtilMat2QuatPos( double m[3][4], double q[4], double p[3] )
{
	double   w;

	w = m[0][0] + m[1][1] + m[2][2] + 1;
	if( w < 0.0 ) return -1;

	w = sqrt( w );
	q[0] = (m[1][2] - m[2][1]) / (w*2.0);
	q[1] = (m[2][0] - m[0][2]) / (w*2.0);
	q[2] = (m[0][1] - m[1][0]) / (w*2.0);
	q[3] = w / 2.0;

	p[0] = m[0][3];
	p[1] = m[1][3];
	p[2] = m[2][3];

	return 0;
}

int GLSetLight(GLvoid)
{
	// OpenGL Light Information
	float light0_diffuse[] = {0.8, 0.8, 0.8} ;
	float light0_specular[] = {0.2, 1.0, -0.2, 1.0} ;
	float light0_shininess[] = {50.0} ;
	float light0_pos[] = {-3000.0f, 0.0f, 3000.0f, 0.0} ;

	float light1_diffuse[] = {0.8f, 0.8f, 0.8f} ;
	float light1_specular[] = {0.8f, 0.8f, 0.8f, 1.0f} ;
	float light1_shininess[] = {50.0} ;
	float light1_pos[] = {3000.0f, 0.0f, -3000.0f, 0.0} ;
	float light2_pos[] = {-3000.0f, 0.0f, 3000.0f, 0.0} ;
	float light3_pos[] = {-3000.0f, 0.0f, -3000.0f, 0.0} ;

	glLightfv (GL_LIGHT0, GL_DIFFUSE, light0_diffuse) ;
	glLightfv (GL_LIGHT0, GL_SPECULAR, light0_specular) ;
	glLightfv (GL_LIGHT0, GL_SHININESS, light0_shininess) ;
	glLightfv (GL_LIGHT0, GL_POSITION, light0_pos) ;

	glLightfv (GL_LIGHT1, GL_DIFFUSE, light1_diffuse) ;
	glLightfv (GL_LIGHT1, GL_SPECULAR, light1_specular) ;
	glLightfv (GL_LIGHT1, GL_SHININESS, light1_shininess) ;
	glLightfv (GL_LIGHT1, GL_POSITION, light1_pos) ;

	glLightfv (GL_LIGHT2, GL_DIFFUSE, light1_diffuse) ;
	glLightfv (GL_LIGHT2, GL_SPECULAR, light1_specular) ;
	glLightfv (GL_LIGHT2, GL_SHININESS, light1_shininess) ;
	glLightfv (GL_LIGHT2, GL_POSITION, light2_pos) ;

	glLightfv (GL_LIGHT3, GL_DIFFUSE, light1_diffuse) ;
	glLightfv (GL_LIGHT3, GL_SPECULAR, light1_specular) ;
	glLightfv (GL_LIGHT3, GL_SHININESS, light1_shininess) ;
	glLightfv (GL_LIGHT3, GL_POSITION, light3_pos) ;

	glEnable (GL_LIGHTING) ;
	glEnable (GL_LIGHT0) ;
	glEnable (GL_LIGHT1) ;
	glEnable (GL_LIGHT2) ;
	glEnable (GL_LIGHT3) ;

	return TRUE;
}

int GLInit(GLvoid)										
{
	// Rendering environment setup
	glShadeModel(GL_SMOOTH);							// ShadeModel
//	glClearColor(0.8f, 0.7f, 0.6f, 0.5f);				// Background Color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Background Color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Bit Plain
	glClearDepth(1.0f);									// Clear Depth
	glEnable(GL_DEPTH_TEST);							// Enable Z-Buffer
	glDepthFunc(GL_LEQUAL);								// Set Depth Function to Less or Equal
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	;GLSetLight();

	CString strFile;
	FILE	*in;
	qTex = new QdTexture[IMAGE_NUM];
	Pics = new CPicture[IMAGE_NUM];
	in = fopen("Data.txt","r");
	iPictureNum = 0;
	if(in)
	{
		CHAR pBuffer[1024];
		WCHAR pWBuffer[1024];
		CString strTime;
		iStartYear = 99999;
		for(int i=0;i<IMAGE_NUM;i++)
		{
			int ret;
			ret=fscanf(in,"%s",pBuffer);
			if(ret == EOF)
				break;
			//Pics[i].strName.Format("../Pics/%.5d.jpg",i);
			Pics[i].strName = pBuffer;
			ret = fscanf(in,"%s",pBuffer);
			if(ret == EOF)
				break;
			strTime = pBuffer;
			strTime.Replace("."," ");
			sscanf(strTime.GetBuffer(0),"%d %d %d %d %d",&Pics[i].iTime[0],&Pics[i].iTime[1],&Pics[i].iTime[2],&Pics[i].iTime[3],&Pics[i].iTime[4]);
			qTex[i].SetMode(QD_NO_AUTO_TEXTURE);
			if(Pics[i].iTime[0]<iStartYear)
				iStartYear = Pics[i].iTime[0];
			ret = fscanf(in,"%s",pBuffer);
			if(ret == EOF)
				break;
			wmemset(Pics[i].strMemo,0,1024);
			MultiByteToWideChar(CP_ACP,0,pBuffer,strlen(pBuffer)+1,Pics[i].strMemo,1024);
			//Pics[i].strMemo = pWBuffer;
			//Pics[i].strMemo.Replace("_"," ");
			for(int p=0;p<1024;p++)
				if(Pics[i].strMemo[p]==L'_')
					Pics[i].strMemo[p]=L' ';
			qTex[i].LoadFromXImage(Pics[i].strName.GetBuffer(0));
			Pics[i].Tex = &qTex[i];
			Pics[i].TexMemo = NULL;
			iPictureNum++;
		}
		fclose(in);
	}
// 	qTex.SetMode(QD_NO_AUTO_TEXTURE);
// 	qTex.LoadFromBitmap("TreeShell.bmp");
// 	qTex2.SetMode(QD_NO_AUTO_TEXTURE);
// 	qTex2.LoadFromBitmap("SkyTexture.bmp");
// 	qTex3.SetMode(QD_NO_AUTO_TEXTURE);
// 	qTex3.LoadFromBitmap("GlassTexture.bmp");
// 	qTexDrops.SetMode(QD_NO_AUTO_TEXTURE);
// 	qTexDrops.LoadFromXImage("WaterDrop.png");
	/*

	if(BeeLoadBMPFile("TreeShell.bmp",&textBitmap,&textPalette))
	{
		GetObject(textBitmap, sizeof(BITMAP), &bit);
		pBmpBits=(PBYTE)bit.bmBits;
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D,0,3,bit.bmWidth,bit.bmHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBmpBits);
	}

	/*
	if(BeeLoadBMPFile("SkyTexture.bmp",&textBitmap_SkyPortrait,&textPalette))
	{
		GetObject(textBitmap_SkyPortrait, sizeof(BITMAP), &bit2);
		pBmpBits2=(PBYTE)bit2.bmBits;
		glGenTextures(2,&texture2);
		glBindTexture(GL_TEXTURE_2D,texture2);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D,0,3,bit2.bmWidth,bit2.bmHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBmpBits2);
	}

	if(BeeLoadBMPFile("GlassTexture.bmp",&textBitmap_GlassPortrait,&textPalette))
	{
		GetObject(textBitmap_GlassPortrait, sizeof(BITMAP), &bit3);
		pBmpBits3=(PBYTE)bit3.bmBits;
		glGenTextures(3,&texture3);
		glBindTexture(GL_TEXTURE_2D,texture3);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D,0,3,bit3.bmWidth,bit3.bmHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBmpBits3);
	}//*/

	return TRUE;							
}

int GLFine(GLvoid)
{
	delete[] qTex;
	delete[] Pics;
	qTex = NULL;
	Pics = NULL;
	return 0;
}

BOOL GLSetPixelformat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR *ppfd; 
	int pixelformat; 

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
			1,						// version number 
			PFD_DOUBLEBUFFER |	// double buffered 처음에 없는 프레임이 생기는 탛E
			//PFD_GENERIC_FORMAT |
			//PFD_STEREO_DONTCARE |
			//PFD_SUPPORT_GDI |		// support GDI 
			//PFD_DRAW_TO_BITMAP |	// support Bitmap 
			PFD_DRAW_TO_WINDOW |	// support Window 
			PFD_SUPPORT_OPENGL,		// support OpenGL 
			PFD_TYPE_RGBA,			// RGBA type 
			24,                    // 32-bit color depth 
			8, 0, 8, 0, 8, 0,      // color bits ignored 
			0,                     // no alpha buffer 
			0,                     // shift bit ignored 
			0,                     // no accumulation buffer 
			0, 0, 0, 0,            // accum bits ignored 
			32,                    // 16-bit z-buffer	 
			0,                     // no stencil buffer 
			0,                     // no auxiliary buffer 
			PFD_MAIN_PLANE,        // main layer 
			0,                     // reserved 
			0, 0, 0                // layer masks ignored 
	}; 

	ppfd = &pfd;

	pixelformat = ChoosePixelFormat( hdc, ppfd);
	if (pixelformat==0) // Let's choose a default index.
	{
		pixelformat = 1; 
		if (DescribePixelFormat(hdc, pixelformat, sizeof(PIXELFORMATDESCRIPTOR), ppfd)==0)
			return FALSE;
	}

	if (SetPixelFormat( hdc,pixelformat,ppfd)==FALSE)
		return FALSE;

	return TRUE;
}

static void argSetStencil( int flag )
{
	if( flag == 0 ) {
		glDisable(GL_STENCIL_TEST);
		glStencilFunc (GL_ALWAYS, 0, 0);
		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
	}
	else {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc (GL_EQUAL, flag, flag);
		glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);
	}
}

void argDrawMode2D( int X, int Y )
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, X, 0, Y, -1.0, 1.0);
	glViewport(0, 0, X, Y);

	argSetStencil( 0 );
}

void argDrawMode3D( void )
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

GLvoid GLReSizeScene(GLsizei width, GLsizei height)
{
	if (height==0)
		height=1;

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(float)width/(float)height,0.1f,10000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int GLDrawAxis()
{
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glColor3f(0.8f,0.0f,0.0f);
	glRotatef(90,0,1,0);
	glutSolidCone(5,100.0,8,20);
	glRotatef(-90,0,1,0);

	glColor3f(0.0f,0.8f,0.0f);
	glRotatef(-90,1,0,0);
	glutSolidCone(5,100.0,8,20);
	glRotatef(90,1,0,0);

	glColor3f(0.0f,0.0f,0.8f);
	glutSolidCone(5,100.0,8,20);

	return 0;
}

int GLDrawGlobalLine()
{
	glLineWidth(3);
	glColor3b(40,10,10);
	glBegin(GL_LINES);
	glVertex3f(-700.0f,0.0f,0.0f);
	glVertex3f(    0.0f,0.0f,0.0f);
	glEnd();
	glColor3b(127,10,10);
	glBegin(GL_LINES);
	glVertex3f(    0.0f,0.0f,0.0f);
	glVertex3f( 700.0f,0.0f,0.0f);
	glEnd();
	glColor3b(10,40,10);
	glBegin(GL_LINES);
	glVertex3f(0.0f,-700.0f,0.0f);
	glVertex3f(0.0f,    0.0f,0.0f);
	glEnd();	
	glColor3b(10,127,10);
	glBegin(GL_LINES);
	glVertex3f(0.0f,    0.0f,0.0f);
	glVertex3f(0.0f, 700.0f,0.0f);
	glEnd();
	glColor3b(10,10,40);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,-700.0f);
	glVertex3f(0.0f,0.0f,    0.0f);
	glEnd();
	glColor3b(10,10,127);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,    0.0f);
	glVertex3f(0.0f,0.0f, 700.0f);
	glEnd();
	glPolygonMode(GL_FRONT,GL_LINE);
	glPolygonMode(GL_BACK,GL_LINE);
	glColor4b(10,127,10,30);
	glBegin(GL_QUADS);
	glVertex3f(-700.0f,0.0f,-700.0f);
	glVertex3f( 700.0f,0.0f,-700.0f);
	glVertex3f( 700.0f,0.0f, 700.0f);
	glVertex3f(-700.0f,0.0f, 700.0f);
	glEnd();
	glColor4b(10,10,127,30);
	glBegin(GL_QUADS);
	glVertex3f(-700.0f,-700.0f,0.0f);
	glVertex3f( 700.0f,-700.0f,0.0f);
	glVertex3f( 700.0f, 700.0f,0.0f);
	glVertex3f(-700.0f, 700.0f,0.0f);
	glEnd();
	glColor4b(127,10,10,30);
	glBegin(GL_QUADS);
	glVertex3f(0.0f,-700.0f,-700.0f);
	glVertex3f(0.0f, 700.0f,-700.0f);
	glVertex3f(0.0f, 700.0f, 700.0f);
	glVertex3f(0.0f,-700.0f, 700.0f);
	glEnd();
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);

	return 0;
}

void GLDrawCamera(double trans[3][4])
{
	double	btrans[3][4];
	double	quat[4], pos[3], angle;

	arUtilMatInv( trans, btrans );

	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		//glMatrixMode(GL_MODELVIEW);
		arUtilMat2QuatPos( btrans, quat, pos );
		angle = -acos(quat[3])*360.0/PIE;
		glTranslatef( pos[0], pos[1], pos[2] );
		glRotated( angle, quat[0], quat[1], quat[2] );

		GLDrawAxis();

		glPushMatrix();
		{
			glColor3f( 0.9, 0.9, 0.0 );
			glTranslatef( 0.0, 0.0, -10.0 );
			glScalef( 10.0, 10.0, 20.0 );
			glutSolidCube(1.0);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glColor3f( 0.9, 0.0, 0.9 );
			glTranslatef( 0.0, 0.0, -40.0 );
			glScalef( 30.0, 30.0, 50.0 );
			glutSolidCube(1.0);
		}
		glPopMatrix();
	}
	glPopMatrix();

	return;
}
