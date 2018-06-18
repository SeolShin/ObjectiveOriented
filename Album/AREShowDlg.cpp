// AREShowDlg.cpp : implementation file
//

#include "AREShowDlg.h"


// CAREShowDlg dialog

//IMPLEMENT_DYNAMIC(CAREShowDlg, CDialog)
CAREShowDlg::CAREShowDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	bFullScreen = FALSE;
	ang1=ang2=3.141592f/6;
	zoom=1.0f;
	bMoving=FALSE;
	iUnderControl=1;
}

CAREShowDlg::~CAREShowDlg()
{
}

void CAREShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAREShowDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CAREShowDlg message handlers

UINT CAREShowDlg::OnNcHitTest(CPoint point)
{
	UINT result = CDialog::OnNcHitTest(point);
	int H,W;//,Y2;
	CRect R;
	GetWindowRect(&R);
	H = R.bottom - R.top;
	W = R.right - R.left;

	if(iUnderControl==-1)
	{
		// Resizable Windows 
		if(point.y<=R.bottom && point.y>R.bottom-10 &&
			point.x<=R.right && point.x>R.right-10)
			result = HTBOTTOMRIGHT;
		else if(point.y<=R.bottom && point.y>R.bottom-10)
			result = HTBOTTOM;
		else if(point.x<=R.right && point.x>R.right-10)
			result = HTRIGHT;
		else if (result == HTCLIENT)
			result = HTCAPTION;
	} else {
		bMoving=TRUE;
	}

	return result;
}

void CAREShowDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_RETURN)
	{
		if(bFullScreen)
		{
			MoveWindow(rcWindowed);
		} else {
			GetWindowRect(rcWindowed);
			CRect R(0,0,0,0);
			R.right=GetSystemMetrics(SM_CXSCREEN);
			R.bottom=GetSystemMetrics(SM_CYSCREEN);
			MoveWindow(R);
			SetWindowPos(&wndTopMost,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),SWP_SHOWWINDOW);
		}
		bFullScreen=!bFullScreen;
	} else if(nChar==VK_LEFT)
	{
		ang1+=0.05f;
		//Invalidate();
	} else if(nChar==VK_RIGHT)
	{
		ang1-=0.05f;
		//Invalidate();
	} else if(nChar==VK_UP)
	{
		zoom*=0.9f;
		//Invalidate();
	} else if(nChar==VK_DOWN)
	{
		zoom*=1.1f;
		//Invalidate();
	} else if(nChar==VK_SPACE)
	{
		iUnderControl=iUnderControl*(-1);
		MessageBox("Edit Mode has been Changed!","Change");
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}