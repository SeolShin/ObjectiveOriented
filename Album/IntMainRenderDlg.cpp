// IntMainRenderDlg.cpp : implementation file
//

#include "IntMainRenderDlg.h"
#include ".\IntMainRenderDlg.h"
#include "GlRender.h"

#define USE_TEST_DATA

static GLfloat		lightPosition[4];
static GLfloat		floorPlane[4];
static GLfloat		floorShadow[4][4];

// CIntMainRenderDlg dialog
INT iStartYear=99999;
INT iPictureNum=0;
INT *iDrawQueue=NULL;
extern CPicture	*Pics;
QdTexture g_qd_tr,g_qd_r,g_qd_br,g_qd_b,g_qd_bl; 

IMPLEMENT_DYNAMIC(CIntMainRenderDlg, CDialog)
CIntMainRenderDlg::CIntMainRenderDlg(INT iScreenX, INT iScreenY, CWnd* pParent /*=NULL*/)
: CIntRenderDlg(CIntMainRenderDlg::IDD, pParent)
{
	this->iScreenX = iScreenX;
	this->iScreenY = iScreenY;
	int iXX=iScreenX, iYY=iScreenY;
	int x,y;

	iImageSelected = -1;
	bRotate	= FALSE;
	bThumbNail = FALSE;
	fViewPoint = -252;
	fViewPointTN = -452;
	fAutoTarget = 0;
	fAutoTargetTN = 0;
	iLastSelectedPic = 0;

	pData = new BYTE[iScreenX*iScreenY*4];
	for(x=0;x<iXX;x++)
		for(y=0;y<iYY;y++)
		{
			PBYTE pTmp;
			pTmp = 	&pData[(y*iXX+x)*4];
			if(y>310)
				/*B*/pTmp[0]=(BYTE)(((float)y-310.0f)/458.0f*255.0f);
			else
				/*B*/pTmp[0]=0;
			if(y>350)
				/*G*/pTmp[1]=(BYTE)(((float)y-350.0f)/418.0f*255.0f);
			else
				/*G*/pTmp[1]=0;
			if(y>400)
				/*R*/pTmp[2]=(BYTE)(((float)y-400.0f)/368.0f*255.0f);
			else
				/*R*/pTmp[2]=0;

			/*A*/pTmp[3]=0xFF;
		}
}

CIntMainRenderDlg::~CIntMainRenderDlg()
{
	delete[] pData;
}


void CIntMainRenderDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	GLFine();
	delete[] iDrawQueue;
	CIntRenderDlg::OnClose();
}

void CIntMainRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CIntMainRenderDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CIntMainRenderDlg::ResetPicturePos()
{
	int yCounter=250;
	for(int i=0;i<iPictureNum;i++)
	{
		INT iTotTime = ((((Pics[i].iTime[0]-iStartYear)*12+Pics[i].iTime[1])*31+Pics[i].iTime[2])*24+Pics[i].iTime[3])*60+Pics[i].iTime[4];
		Pics[i].vPos.x = ((float)iTotTime)/100.0f;//i*140;//rand()%800-400;
		Pics[i].vPos.y = yCounter;//rand()%300-150;
		Pics[i].vPos.z = -iPictureNum*0.1f+(float)(iPictureNum-i)*0.1f;
		iDrawQueue[iPictureNum-i-1]=i;
		Pics[i].fZoom  = 0.2f;
		yCounter-=20;
		if(yCounter<-150)
			yCounter=250;
	}
}

// CIntMainRenderDlg message handlers
BOOL CIntMainRenderDlg::OnInitDialog()
{
	CIntRenderDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	// call from FindResource(NULL,iResourceMAKEINTRESOURCE(IDR_PNG1),"PNG"), CXIMAGE_FORMAT_PNG
	g_qd_tr.SetMode(QD_NO_AUTO_TEXTURE);
	g_qd_r.SetMode(QD_NO_AUTO_TEXTURE);
	g_qd_br.SetMode(QD_NO_AUTO_TEXTURE);
	g_qd_b.SetMode(QD_NO_AUTO_TEXTURE);
	g_qd_bl.SetMode(QD_NO_AUTO_TEXTURE);
	g_qd_tr.LoadFromXResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG_TR),"PNG"), CXIMAGE_FORMAT_PNG);
	g_qd_r.LoadFromXResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG_R),"PNG"), CXIMAGE_FORMAT_PNG);
	//g_qd_r.LoadFromXImage("WaterDrop.png");
	g_qd_br.LoadFromXResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG_BR),"PNG"), CXIMAGE_FORMAT_PNG);
	g_qd_b.LoadFromXResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG_B),"PNG"), CXIMAGE_FORMAT_PNG);
	g_qd_bl.LoadFromXResource(FindResource(NULL,MAKEINTRESOURCE(IDR_PNG_BL),"PNG"), CXIMAGE_FORMAT_PNG);

	CRect R;
	int yCounter=250;
	argDrawMode2D(iScreenX, iScreenY);
	GetWindowRect(R);
	glRasterPos2f(0,0);
	glRasterPos2f(0,(float)R.Height());

	srand(NULL);

	iDrawQueue = new INT[iPictureNum];
	ResetPicturePos();
	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CIntMainRenderDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==1)
	{
		KillTimer(1);
		SetTimer(2,1000.0f/((float)FRAME_PER_SECOND),NULL);
	}
	if(nIDEvent==2)
		Invalidate();
	if(nIDEvent==4)
		if(fAutoTarget!=0)
		{
			fViewPoint-=fAutoTarget/20;
			fAutoTarget*=(19.0f/20.0f);
			if(fAutoTarget<0.1 && fAutoTarget>-0.1)
			{
				fViewPoint-=fAutoTarget;
				fAutoTarget = 0;
				KillTimer(4);
			}
		}

	CIntRenderDlg::OnTimer(nIDEvent);
}

void DrawSphere(float sphx, float sphy, float sphz, float sphrad)
{
	GLUquadricObj* p_obj;
	p_obj = gluNewQuadric();
	gluQuadricNormals(p_obj,GLU_SMOOTH);
	gluQuadricOrientation(p_obj,GLU_INSIDE);
	glPushMatrix();
	glTranslatef(sphx,sphy,sphz);
	gluSphere(p_obj,sphrad,30,20);
	glPopMatrix();
}

BOOL CIntMainRenderDlg::OnEraseBkgnd(CDC* pDC)
{
	SwapBuffers(hScreenDC);

	CRect R;
	glClear(GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_COLOR_BUFFER_BIT);	
	argDrawMode2D(iScreenX, iScreenY);
	GetWindowRect(R);
	glRasterPos2f(0,0);
	glRasterPos2f(0,(float)R.Height());
	glDrawPixels(iScreenX,iScreenY,GL_BGRA_EXT,GL_UNSIGNED_BYTE,pData);
	glPixelZoom( ((float)R.Width())/((float)iScreenX), -((float)R.Height())/((float)iScreenY));

	////////////////////////////////////////////////////////////////
	// Start to Draw
	glViewport(0, 0, iScreenX, iScreenY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(22.5,4.0/3.0,1.0,2900.0);
	glClearDepth( 1.0 );
	GLDrawScene();
	if(bStartRender)
		return TRUE;
	return CIntRenderDlg::OnEraseBkgnd(pDC);
}


void CIntMainRenderDlg::OnPaint()
{
	 CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CIntRenderDlg::OnPaint() for painting messages
}

void CIntMainRenderDlg::DrawPic(int i)
{
	float Newy;
	if(Pics[i].vPos.x+fViewPoint<-1024 || Pics[i].vPos.x+fViewPoint>1024)
	{
		// Release Memo Texture
		if(Pics[i].TexMemo)
		{
			delete Pics[i].TexMemo;
			Pics[i].TexMemo = NULL;
		}
		return;
	}

	// Make Memo Texture
	if(Pics[i].TexMemo==NULL)
	{
		CHAR txt[5][256];
		CHAR pBuffer[1024];
		pBuffer[15]=0;
		sprintf(txt[0],"%d.%d.%d. %d:%d",Pics[i].iTime[0],Pics[i].iTime[1],Pics[i].iTime[2],Pics[i].iTime[3],Pics[i].iTime[4]);
		memset(pBuffer,0,1024);WideCharToMultiByte(CP_ACP,0,Pics[i].strMemo,16,pBuffer,1024,NULL,NULL);
		sprintf(txt[1],"%s",pBuffer);
		memset(pBuffer,0,1024);WideCharToMultiByte(CP_ACP,0,Pics[i].strMemo+16,16,pBuffer,1024,NULL,NULL);
		sprintf(txt[2],"%s",pBuffer);
		memset(pBuffer,0,1024);WideCharToMultiByte(CP_ACP,0,Pics[i].strMemo+32,16,pBuffer,1024,NULL,NULL);
		sprintf(txt[3],"%s",pBuffer);
		memset(pBuffer,0,1024);WideCharToMultiByte(CP_ACP,0,Pics[i].strMemo+48,16,pBuffer,1024,NULL,NULL);
		sprintf(txt[4],"%s",pBuffer);
		Pics[i].TexMemo = new QdTexture();
		Pics[i].TexMemo->SetMode(QD_NO_AUTO_TEXTURE);
		Pics[i].TexMemo->LoadFromText(Pics[i].Tex->x,Pics[i].Tex->y,txt);
	}

	glPushMatrix();
	Newy = Pics[i].vPos.y;
	if(Newy<-274)
		Newy = -274;
	if(Newy>384)
		Newy = 384;
	glTranslatef(Pics[i].vPos.x,Newy,Pics[i].vPos.z);
	glRotatef(Pics[i].aRot.Degree(),0,0,1.0f);
	glScalef(Pics[i].fZoom,Pics[i].fZoom,Pics[i].fZoom);
	float w=Pics[i].Tex->x,h=Pics[i].Tex->y;
	float	wa[9][2] = {
			{-0.6,0.6},
			{-1,-0.6},
			{-0.6,0.6},
			{0.6,1},
			{0.6,1},
			{0.6,1},
			{-0.6,0.6},
			{-1,-0.6},
			{-1,-0.6}};
	float	ha[9][2] = {
			{-0.6,0.6},
			{-1,-0.6},
			{-1,-0.6},
			{-1,-0.6},
			{-0.6,0.6},
			{0.6,1},
			{0.6,1},
			{0.6,1},
			{-0.6,0.6}};
	DWORD	wid[9] = {0,AREA_LEFTTOP,AREA_TOP,AREA_RIGHTTOP,AREA_RIGHT,AREA_RIGHBOTTOM,AREA_BOTTOM,AREA_LEFTBOTTOM,AREA_LEFT};
	// 123
	// 804
	// 765 <<------------- see Locations for Index

	FLOAT	fBorder=1.1;	// Polaroid White space <<-----------------------------------LOOK

	// Draw Polaroid White Space
	glColor3f(0.8,0.8,0.8);
	glLoadName((DWORD)(i)|AREA_RIGHT);

	// Right
	glBegin(GL_QUADS);
		glVertex3f		(w*fBorder,h*fBorder,0);
		glVertex3f		(w,h*fBorder,0);
		glVertex3f		(w,-h*fBorder,0);
		glVertex3f		(w*fBorder,-h*fBorder,0);
	glEnd();
	// Right Shadow
//*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	g_qd_tr.SelectTexture();
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w*fBorder+20,h*fBorder-20,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(w*fBorder,h*fBorder-20,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(w*fBorder,h*fBorder-40,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w*fBorder+20,h*fBorder-40,0);
	glEnd();
	g_qd_r.SelectTexture();
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w*fBorder+20,h*fBorder-40,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(w*fBorder,h*fBorder-40,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(w*fBorder,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w*fBorder+20,-h*1.6,0);
	glEnd();
	g_qd_br.SelectTexture();
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w*fBorder+20,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(w*fBorder,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(w*fBorder,-h*1.6-20,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w*fBorder+20,-h*1.6-20,0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
//*/
	// Left
	glLoadName((DWORD)(i)|AREA_LEFT);
	glBegin(GL_QUADS);
		glVertex3f		(-w,h*fBorder,0);
		glVertex3f		(-w*fBorder,h*fBorder,0);
		glVertex3f		(-w*fBorder,-h*fBorder,0);
		glVertex3f		(-w,-h*fBorder,0);
	glEnd();

	// Bottom(Actually Top on Screen)
	glLoadName((DWORD)(i)|AREA_BOTTOM);
	glBegin(GL_QUADS);
		glVertex3f		(w*fBorder,h*fBorder,0);
		glVertex3f		(-w*fBorder,h*fBorder,0);
		glVertex3f		(-w*fBorder,h,0);
		glVertex3f		(w*fBorder,h,0);
	glEnd();

	// Top(Actually Bottom on Screen)
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0,1.0,1.0,1.0);
	Pics[i].TexMemo->SelectTexture();
	glLoadName((DWORD)(i)|AREA_TOP);
	glBegin(GL_QUADS);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w*fBorder,-h,0);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(-w*fBorder,-h,0);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(-w*fBorder,-h*1.6,0);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w*fBorder,-h*1.6,0);
	glEnd();
//*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	g_qd_b.SelectTexture();
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w*fBorder,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(-w*fBorder+40,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(-w*fBorder+40,-h*1.6-20,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w*fBorder,-h*1.6-20,0);
	glEnd();
	g_qd_bl.SelectTexture();
	glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(-w*fBorder+40,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(-w*fBorder+20,-h*1.6,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(-w*fBorder+20,-h*1.6-20,0);
		glColor4f(1.0,1.0,1.0,0.5);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(-w*fBorder+40,-h*1.6-20,0);
	glEnd();
	glDisable(GL_BLEND);
//*/
	// Draw Picture (with 9 parts)
	Pics[i].Tex->SelectTexture();
	for(int p=0;p<9;p++)
	{
		glLoadName((DWORD)(i)|wid[p]);
		glBegin(GL_QUADS);
		glTexCoord2f	(wa[p][1]/2+0.5f, ha[p][1]/2+0.5f); 
		glVertex3f		(w*wa[p][1],h*ha[p][1],0);
		glTexCoord2f	(wa[p][0]/2+0.5f, ha[p][1]/2+0.5f); 
		glVertex3f		(w*wa[p][0],h*ha[p][1],0);
		glTexCoord2f	(wa[p][0]/2+0.5f, ha[p][0]/2+0.5f); 
		glVertex3f		(w*wa[p][0],h*ha[p][0],0);
		glTexCoord2f	(wa[p][1]/2+0.5f, ha[p][0]/2+0.5f); 
		glVertex3f		(w*wa[p][1],h*ha[p][0],0);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void CIntMainRenderDlg::DrawTN(int i)
{
	float Newy, Nv;
	float w=Pics[i].Tex->x,h=Pics[i].Tex->y;

	if((i*110+fViewPointTN)<-1024 || (i*110+fViewPointTN)>1024)
	{
		return;
	}

	Nv = Pics[i].Tex->x;
	if(Pics[i].Tex->x<Pics[i].Tex->y)
		Nv = Pics[i].Tex->y;
	Nv = 100 / Nv;
	glPushMatrix();
	glTranslatef(i*110,-384+57.5,11);
	glScalef(Nv/2,Nv/2,Nv/2);

	glColor3f(0.8,0.8,0.8);
	glLoadName((DWORD)(i)|AREA_THUMBNAIL);
	glBegin(GL_QUADS);
		glVertex3f		(w+5,h+5,1);
		glVertex3f		(-w-5,h+5,1);
		glVertex3f		(-w-5,-h-5,1);
		glVertex3f		(w+5,-h-5,1);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	Pics[i].Tex->SelectTexture();
	glBegin(GL_QUADS);
		glTexCoord2f	(1.0, 1.0); 
		glVertex3f		(w-10,h-10,11);
		glTexCoord2f	(0.0, 1.0); 
		glVertex3f		(-w+10,h-10,11);
		glTexCoord2f	(0.0, 0.0); 
		glVertex3f		(-w+10,-h+10,11);
		glTexCoord2f	(1.0, 0.0); 
		glVertex3f		(w-10,-h+10,11);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// Get Image Index with Clicked ID
int	CIntMainRenderDlg::GetImageNum(INT id)
{
	return (DWORD)id&0x0FFFFFFF;
}

#define BUFFER_LENGTH 1024*256

// Identify Clicked Picture
void CIntMainRenderDlg::ProcessSelection(int xPos, int yPos)
{
	// Space for selection buffer
	GLuint *selectBuff = new GLuint[BUFFER_LENGTH];

	// Hit counter and viewport storeage
	GLint hits, viewport[4];

	// Setup selection buffer
	glSelectBuffer(BUFFER_LENGTH, selectBuff);

	// Get the viewport
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Switch to projection and save the matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	// Change render mode
	glRenderMode(GL_SELECT);

	// Establish new clipping volume to be unit cube around
	// mouse cursor point (xPos, yPos) and extending two pixels
	// in the vertical and horzontal direction
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3] - yPos, 2,2, viewport);

	// Apply perspective matrix 
	gluPerspective(22.5,4.0/3.0,1.0,2900.0);

	// Draw the scene
	GLDrawScene();

	// Collect the hits
	hits = glRenderMode(GL_RENDER);

	iImageSelected = -2;
	// If a single hit occured, display the info.
	if(hits >= 1)
	{
		int nImages = hits;
		float z=-100;
		for(int i=0;i<nImages;i++)
		{
			int iImageNum;
			iImageNum = GetImageNum(selectBuff[3+i*4]);
			if(Pics[iImageNum].vPos.z>z)
			{
				z=Pics[iImageNum].vPos.z;
				iImageSelected = iImageNum;
				bRotate = FALSE;
				bThumbNail = FALSE;
				if((DWORD)selectBuff[3+i*4]&0xF0000000)
				{
					if(((DWORD)selectBuff[3+i*4]&0xF0000000)<=(DWORD)AREA_LEFT)
						bRotate = TRUE;
					else if(((DWORD)selectBuff[3+i*4]&0xF0000000)==(DWORD)AREA_THUMBNAIL)
						bThumbNail = TRUE;
				}
			}
		}
	}

	// Restore the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Go back to modelview for normal rendering
	glMatrixMode(GL_MODELVIEW);
	delete[] selectBuff;
}

void CIntMainRenderDlg::GLDrawScene()
{
	CRect R;

	wglMakeCurrent( hScreenDC, hglRC );
	// Make 2D Viewport
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0,0,1910,
		0,0,0,
		0,200,0);

	glEnable( GL_DEPTH_TEST );

	// Draw Test Full screen rectangle
// 	glBegin(GL_QUADS);
// 	glVertex3f		( 512, 384,0);
// 	glVertex3f		(-512, 384,0);
// 	glVertex3f		(-512,-384,0);
// 	glVertex3f		( 512,-384,0);
// 	glEnd();

	// Initialize the names stack
	glInitNames();
	glPushName(0);

	// Draw Pictures
	glPushMatrix();
	glTranslatef(fViewPoint,0,0);
	for(int i=0;i<iPictureNum;i++)
		DrawPic(iDrawQueue[i]);
	glPopMatrix();

	// Draw Thumbnails
	glPushMatrix();
	glTranslatef(fViewPointTN,0,0);
	for(int i=0;i<iPictureNum;i++)
		DrawTN(i);
	glPopMatrix();

	// Draw Thumbnail Outer Space
	//glLoadName(0);
	glColor4f(0.5f,0.5f,0.5f,0.5f);
	glBegin(GL_QUADS);
	glVertex3f		(  512, -384,0);
	glVertex3f		( -512, -384,0);
	glVertex3f		( -512, -274,0);
	glVertex3f		(  512, -274,0);
	glEnd();

	glDisable( GL_DEPTH_TEST );
	glFlush();

	return;
}

// Make A Picture TopMost
void CIntMainRenderDlg::MakeNTop(int N)
{
	float z = Pics[N].vPos.z;
	int i;
	BOOL bShift=FALSE;
	for(i=0;i<iPictureNum-1;i++)
	{
		if(iDrawQueue[i]==N)
			bShift = TRUE;
		if(bShift)
			iDrawQueue[i] = iDrawQueue[i+1];
	}
	iDrawQueue[i] = N;
	for(i=0;i<iPictureNum;i++)
		if(Pics[i].vPos.z>z)
			Pics[i].vPos.z-=0.1f;
	Pics[N].vPos.z = -iPictureNum*0.1f+(float)(iPictureNum)*0.1f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERFACE DEFINITION  <<--------------------------------------------------------------------LOOK

// Key Input
void CIntMainRenderDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// A or F5 for Rotation Angle to Zero
	if(iLastSelectedPic>=0 && (nChar=='A'|| nChar==VK_F5))
		Pics[iLastSelectedPic].aRot=0;
	if(nChar=='R')
		ResetPicturePos();
	// UP and DOWN for Spread Work Space(Same as Mouse Wheeling)
	if(nChar==VK_UP)
	{
		fViewPoint*=1.1f;
		for(int i=0;i<iPictureNum;i++)
			Pics[i].vPos.x*=1.1f;
	}
	if(nChar==VK_DOWN)
	{
		fViewPoint*=0.9f;
		for(int i=0;i<iPictureNum;i++)
			Pics[i].vPos.x*=0.9f;
	}
	CIntRenderDlg::OnKeyUp(nChar, nRepCnt, nFlags);
}

// Mouse L Click (for Drag)
void CIntMainRenderDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(point.y>658)
		iImageSelected=-3;
	else {
		ProcessSelection(point.x,point.y);
		if(iImageSelected>=0)
		{
			iLastSelectedPic =iImageSelected;
			MakeNTop(iImageSelected);
			/*CVector3D v;
			v.x = point.x;
			v.y = point.y;
			v=Pics[iImageSelected].vPos;
			v.x+=fViewPoint+512;
			v.y=-v.y+384;
			v.x=point.x-v.x;
			v.y=point.y-v.y;
			v.z=0;
			v.y=-v.y;*/
		}
	}

	LastPoint = point;

	CIntRenderDlg::OnLButtonDown(nFlags, point);
}

// Mouse L Release (for End Dragging)
void CIntMainRenderDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	iImageSelected = -1;
	bRotate = FALSE;

	CIntRenderDlg::OnLButtonUp(nFlags, point);
}

// Mouse L DblClick (for Select Thumbnail)
void CIntMainRenderDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(point.y>658)
	{
		ProcessSelection(point.x,point.y);
		if(iImageSelected>=0 && bThumbNail)
		{
			//CString str;
			//str.Format("%d",iImageSelected);
			if(fAutoTarget==0)
			{
				MakeNTop(iImageSelected);
				fAutoTarget = Pics[iImageSelected].vPos.x+fViewPoint;
				iImageSelected = -1;
				bThumbNail = FALSE;
				SetTimer(4,10,NULL);
				//MessageBox(str);
			}
		} else {
			iImageSelected = -1;
		}
	}

	CIntRenderDlg::OnLButtonDblClk(nFlags, point);
}

// Mouse Moving (to Get Dragging Status)
void CIntMainRenderDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint p = point-LastPoint;
	if(iImageSelected>=0)
	{
		if(bRotate==TRUE)	// Rotate and Zoom
		{
			CAngle p;
			CVector2D a, b;
			CVector3D c;
			float t;
			a.x=LastPoint.x-(Pics[iImageSelected].vPos.x+fViewPoint+512);
			a.y=LastPoint.y-(-Pics[iImageSelected].vPos.y+384);
			b.x=point.x-(Pics[iImageSelected].vPos.x+fViewPoint+512);
			b.y=point.y-(-Pics[iImageSelected].vPos.y+384);
			if(a.Scalar()==0 || b.Scalar()==0)
				return;
			t = (b*a)/(a.Scalar()*b.Scalar());
			p.SetRadian(acos(t));
			c = b^a;

			if( c.z>0)
				p=Pics[iImageSelected].aRot+p.Degree();
			else
				p=Pics[iImageSelected].aRot-p.Degree();
			if(_finite(p.Degree())) // Check Invalid Angle <<-----------------------------------------LOOK
				Pics[iImageSelected].aRot = p;
			Pics[iImageSelected].fZoom*=b.Scalar()/a.Scalar();
		} else {
			Pics[iImageSelected].vPos.x+=(float)(p.x);
			Pics[iImageSelected].vPos.y-=(float)(p.y);
		}
		LastPoint = point;
	} else if(iImageSelected == -2){ // Drag Work Space
		fViewPoint+=(float)(p.x);
		LastPoint = point;

	} else if(iImageSelected == -3){ // Drag Thumbnail Space
		fViewPointTN+=(float)(p.x);
		LastPoint = point;
	}

	CIntRenderDlg::OnMouseMove(nFlags, point);
}

// Mouse Wheeling (for Spread Work Space(Same as UP/DOWN))
BOOL CIntMainRenderDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float d=zDelta;
	if(d>0 && d>900)
		d = 900;
	if(d<0 && d<-900)
		d = -900;
	d=1.0f+d/1000.0f;
	fViewPoint*=d;
	for(int i=0;i<iPictureNum;i++)
		Pics[i].vPos.x*=d;
	return CIntRenderDlg::OnMouseWheel(nFlags, zDelta, pt);
}
