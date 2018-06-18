#include "qd.h"
#include <stdio.h>
#include "Texture.h"
#include <GL/glaux.h>
#include <GL/glu.h>
#include <fstream>

int QdTexture::SelectTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	return true;

}
int QdTexture::LoadFromBitmapResource(int nResource)
{
	HBITMAP hBMP;
	BITMAP	BMP;

	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL),MAKEINTRESOURCE(nResource), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);


		if (hBMP)
		{
			GetObject(hBMP,sizeof(BMP), &BMP);
			glGenTextures(1, &m_nTexture);
			glBindTexture(GL_TEXTURE_2D, m_nTexture);
			//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, BMP.bmWidth, BMP.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);

			//Automatically generate texture coordinates if desired
			if(m_bAutoTexture)
			{
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
			}

			DeleteObject(hBMP);
			return QD_SUCCESS;
		}
		else
		{
			
			return QD_INVALID_RESOURCE;
		}
	}


int QdTexture::LoadFromBitmap(char *szPath)
{
    FILE *fp;
    int nMemory;
	QD_BITMAP_IMAGE pbiTexture;

    fp = fopen(szPath, "rb");
    fread(&pbiTexture.bmfHeader, 1, 14, fp);
    fread(&pbiTexture.bmiHeader, 1, 40, fp);  

    if (pbiTexture.bmiHeader.biSizeImage == 0)
    {
        nMemory = pbiTexture.bmiHeader.biWidth * pbiTexture.bmiHeader.biHeight * 4;
    }
    else
    {
       nMemory = pbiTexture.bmiHeader.biSizeImage;
    }
   
    pbiTexture.image_data= new GLubyte[nMemory];

    fread(pbiTexture.image_data , 1, nMemory, fp);
    fclose(fp);

	//glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1, &m_nTexture);

	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pbiTexture.bmiHeader.biWidth, pbiTexture.bmiHeader.biHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pbiTexture.image_data);
	glTexImage2D(GL_TEXTURE_2D,0,3,pbiTexture.bmiHeader.biWidth, pbiTexture.bmiHeader.biHeight,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pbiTexture.image_data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Automatically generate texture coordinates if desired
	if(m_bAutoTexture)
	{
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	delete[] pbiTexture.image_data;
   return true;
}

QdTexture::QdTexture()
{
	m_bAutoTexture = 1;  //Set to autotexture by default
}

QdTexture::~QdTexture()
{

}

int QdTexture::SetMode(int nMode)
{
	if(nMode == QD_NO_AUTO_TEXTURE)
	{
		m_bAutoTexture = 0;
	}
	else if(nMode == QD_AUTO_TEXURE)
	{
		m_bAutoTexture = 1;
	}
	return QD_SUCCESS;
}

char* FindExtension(char *szPath)
{
	int l = strlen(szPath);
	for(int i=l-1;i>=0;i--)
		if(szPath[i]=='.')
			return &szPath[i];
	return szPath;
}

int FindType(const char *ext)
{
	int type = 0;
	if (strcmp(ext,"bmp")==0)					type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (strcmp(ext,"jpg")==0||strcmp(ext,"jpeg")==0)	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (strcmp(ext,"gif")==0)				type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (strcmp(ext,"png")==0)				type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (strcmp(ext,"mng")==0||strcmp(ext,"jng")==0)	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (strcmp(ext,"ico")==0)				type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (strcmp(ext,"tiff")==0||strcmp(ext,"tif")==0)	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (strcmp(ext,"tga")==0)				type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (strcmp(ext,"pcx")==0)				type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (strcmp(ext,"wbmp")==0)				type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (strcmp(ext,"wmf")==0||strcmp(ext,"emf")==0)	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (strcmp(ext,"j2k")==0||strcmp(ext,"jp2")==0)	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (strcmp(ext,"jbg")==0)				type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (strcmp(ext,"jp2")==0||strcmp(ext,"j2k")==0)	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (strcmp(ext,"jpc")==0||strcmp(ext,"j2c")==0)	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (strcmp(ext,"pgx")==0)				type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (strcmp(ext,"ras")==0)				type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if (strcmp(ext,"pnm")==0||strcmp(ext,"pgm")==0||strcmp(ext,"ppm")==0) type = CXIMAGE_FORMAT_PNM;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;

	return type;
}

// call from FindResource(NULL,iResourceMAKEINTRESOURCE(IDR_PNG1),"PNG"), CXIMAGE_FORMAT_PNG
int QdTexture::LoadFromXResource(HRSRC hRes, DWORD dwImageType)
{
	CxImage *pImage = new CxImage();
	if (!pImage->LoadResource(hRes,dwImageType)){
		delete pImage;
		pImage =NULL;
		return QD_INVALID_RESOURCE;
	}

	if (!pImage->IsValid()){
		delete pImage;
		pImage = NULL;
		return QD_INVALID_RESOURCE;
	}

	if(!pImage)
		return QD_INVALID_RESOURCE;

	glGenTextures(1, &m_nTexture);

	glBindTexture(GL_TEXTURE_2D, m_nTexture);

	int pixelsize;
	if(pImage->AlphaIsValid())
	{
		pixelsize = 4;
		int len = pImage->GetWidth()*pImage->GetHeight()*pixelsize;
		PBYTE Alpha = pImage->AlphaGetPointer(), Pix = pImage->GetBits();
		PBYTE pBits = new BYTE[len];
		for(int i=0, j=0;i<len;i+=4,j++)
		{
			pBits[i+0]	= Pix[j*3];
			pBits[i+1]	= Pix[j*3+1];
			pBits[i+2]	= Pix[j*3+2];
			pBits[i+3]	= Alpha[j];
			//pBits[i+0] = pBits[i+1] = pBits[i+2] = 0;
		}
		glTexImage2D(GL_TEXTURE_2D,0,4,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,pBits);
		delete[] pBits;
	} else {
		glTexImage2D(GL_TEXTURE_2D,0,3,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pImage->GetBits());
	}
	x = pImage->GetWidth();
	y = pImage->GetHeight();
	delete pImage;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Automatically generate texture coordinates if desired
	if(m_bAutoTexture)
	{
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

	return TRUE;
}

int QdTexture::LoadFromXImage(char *szPath)
{
	char* ext(FindExtension(szPath));

	if (ext == "") return FALSE;

	int type = FindType(ext);
	// 대소문자 구분 추가해야 함
	CxImage *pImage = new CxImage(szPath, type);

	if (!pImage->IsValid()){
		delete pImage;
		pImage = NULL;
		return QD_INVALID_RESOURCE;
	}

	if(!pImage)
		return QD_INVALID_RESOURCE;

	glGenTextures(1, &m_nTexture);

	glBindTexture(GL_TEXTURE_2D, m_nTexture);

	int pixelsize;
	if(pImage->AlphaIsValid())
	{
		pixelsize = 4;
		int len = pImage->GetWidth()*pImage->GetHeight()*pixelsize;
		PBYTE Alpha = pImage->AlphaGetPointer(), Pix = pImage->GetBits();
		PBYTE pBits = new BYTE[len];
		for(int i=0, j=0;i<len;i+=4,j++)
		{
			pBits[i+0]	= Pix[j*3];
			pBits[i+1]	= Pix[j*3+1];
			pBits[i+2]	= Pix[j*3+2];
			pBits[i+3]	= Alpha[j];
			//pBits[i+0] = pBits[i+1] = pBits[i+2] = 0;
		}
		glTexImage2D(GL_TEXTURE_2D,0,4,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,pBits);
		delete[] pBits;
	} else {
		glTexImage2D(GL_TEXTURE_2D,0,3,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pImage->GetBits());
	}
	x = pImage->GetWidth();
	y = pImage->GetHeight();
	delete pImage;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Automatically generate texture coordinates if desired
	if(m_bAutoTexture)
	{
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

	return TRUE;
}

int QdTexture::LoadFromText(int x, int y, char szText[5][256])
{
	y=y*4/13;
	CxImage *pImage = new CxImage(x,y,24);
	RGBQUAD color;
	color.rgbRed = 0;
	color.rgbGreen = 0;
	color.rgbBlue = 0;
	color.rgbReserved = 0;
	pImage->Clear(0xFF);
	HDC dc;
	pImage->DrawString(0,10,y*4/20,szText[0],color,"Arial",y/5.5);
	pImage->DrawString(0,10,y*7/20,szText[1],color,"Arial",y/5.5);
	pImage->DrawString(0,10,y*10/20,szText[2],color,"Arial",y/5.5);
	pImage->DrawString(0,10,y*13/20,szText[3],color,"Arial",y/5.5);
	pImage->DrawString(0,10,y*16/20,szText[4],color,"Arial",y/5.5);

	glGenTextures(1, &m_nTexture);

	glBindTexture(GL_TEXTURE_2D, m_nTexture);

	int pixelsize;
	if(pImage->AlphaIsValid())
	{
		pixelsize = 4;
		int len = pImage->GetWidth()*pImage->GetHeight()*pixelsize;
		PBYTE Alpha = pImage->AlphaGetPointer(), Pix = pImage->GetBits();
		PBYTE pBits = new BYTE[len];
		for(int i=0, j=0;i<len;i+=4,j++)
		{
			pBits[i+0]	= Pix[j*3];
			pBits[i+1]	= Pix[j*3+1];
			pBits[i+2]	= Pix[j*3+2];
			pBits[i+3]	= Alpha[j];
			//pBits[i+0] = pBits[i+1] = pBits[i+2] = 0;
		}
		glTexImage2D(GL_TEXTURE_2D,0,4,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,pBits);
		delete[] pBits;
	} else {
		glTexImage2D(GL_TEXTURE_2D,0,3,pImage->GetWidth(),pImage->GetHeight(),0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pImage->GetBits());
	}
	x = pImage->GetWidth();
	y = pImage->GetHeight();
	delete pImage;

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//Automatically generate texture coordinates if desired
	if(m_bAutoTexture)
	{
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}

	return TRUE;
}

