// IntRenderDlg.cpp : implementation file
//

#include "IntRenderDlg.h"
#include ".\IntRenderDlg.h"
#include "GlRender.h"


// CIntRenderDlg dialog
IMPLEMENT_DYNAMIC(CIntRenderDlg, CDialog)
CIntRenderDlg::CIntRenderDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CAREShowDlg(nIDTemplate, pParent)
{
	hglRC				= NULL;
	hScreenDC			= NULL;
}

CIntRenderDlg::~CIntRenderDlg()
{
}

void CIntRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIntRenderDlg, CDialog)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CIntRenderDlg message handlers
BOOL CIntRenderDlg::OnInitDialog()
{
	CAREShowDlg::OnInitDialog();

	// TODO:  Add extra initialization here
	// Primary View Window
	hScreenDC = GetDC()->GetSafeHdc();
	if(!GLSetPixelformat(hScreenDC))
	{
		AREMsg(TEXT("GLSetPixelformat Failed!"));
		return FALSE;
	}
	hglRC = wglCreateContext( hScreenDC );
	wglMakeCurrent( hScreenDC, hglRC );
	GLInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
