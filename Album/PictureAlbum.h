// PictureAlbum.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CPictureAlbumApp:
// See PictureAlbum.cpp for the implementation of this class
//

class CPictureAlbumApp : public CWinApp
{
public:
	CPictureAlbumApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPictureAlbumApp theApp;