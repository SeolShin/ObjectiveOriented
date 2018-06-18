#pragma once

#include "qd.h"
#include "CxImage/ximage.h"

class QdTexture
{
public:
	QdTexture();
	~QdTexture();

	//Select this texture to be automapped to future objects
	int SelectTexture();
	//Load the texture from a bitmap file
	int LoadFromBitmap(char *szPath);
	//Load a bitmap from a resource
	int LoadFromBitmapResource(int nResource);
	//Set the mode for the texture
	int SetMode(int nMode);
	int LoadFromXImage(char *szPath);
	int LoadFromXResource(HRSRC hRes, DWORD dwImageType);
	int LoadFromText(int x, int y, char szText[5][256]);

	int x, y;
protected:

	unsigned int m_nTexture;
	bool m_bAutoTexture;
};