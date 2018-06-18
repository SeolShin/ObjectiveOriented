// PictureAlbumDlg.h : header file
//

#pragma once

#include "IntMainRenderDlg.h"
//#include "FingerDetect.h"
#include "CArtoolkit.h"

// CPictureAlbumDlg dialog
class CPictureAlbumDlg : public CDialog
{

	CIntMainRenderDlg *m_pShowWnd;
	//CaptureTag *CaptureThread;

// Construction
public:
	CPictureAlbumDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PICTUREALBUM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();

private:
	//FingerDetect m_Calc;
	CArtoolkit m_Calc;
};

/*
static UINT AFX_CDECL ThreadProcCalc(LPVOID pParam)
{
	FingerDetect *cp=(FingerDetect*)pParam;
	cp->Capture();
	return 0;
}
*/

