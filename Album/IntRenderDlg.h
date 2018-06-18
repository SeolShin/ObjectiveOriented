#pragma once

#include "AREShowDlg.h"

#include <afxdisp.h>        // MFC Automation classes
#include "resource.h"		// main symbols

// CIntRenderDlg dialog

class CIntRenderDlg : public CAREShowDlg
{
	DECLARE_DYNAMIC(CIntRenderDlg)

	HGLRC			hglRC;			// Rendering Context
	HDC				hScreenDC;		// GDI Device Context 

public:
	// Dialog Data
	enum { IDD = IDD_SHOWDLG };

	CIntRenderDlg(UINT nIDTemplate = IDD, CWnd* pParent = NULL);   // standard constructor
	virtual ~CIntRenderDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
