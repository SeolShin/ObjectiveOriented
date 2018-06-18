// PictureAlbumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PictureAlbum.h"
#include "PictureAlbumDlg.h"
#include "GlRender.h"
#include ".\picturealbumdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT AFX_CDECL ThreadProcCalc(LPVOID pParam)
{
	CArtoolkit *cp=(CArtoolkit*)pParam;
	//FingerDetect *cp=(FingerDetect*)pParam;
	cp->Capture();
	return 0;
}

// CPictureAlbumDlg dialog

CPictureAlbumDlg::CPictureAlbumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPictureAlbumDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPictureAlbumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPictureAlbumDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_Start, OnBnClickedStart)
END_MESSAGE_MAP()

// CPictureAlbumDlg message handlers

BOOL CPictureAlbumDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPictureAlbumDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPictureAlbumDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPictureAlbumDlg::OnBnClickedStart()
{
	UpdateData();
	CRect R;
//	if(m_FullScreen)
//		m_pShowWnd = new CIntMainRenderDlg(GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
//	else
	
	//Drawing photoalbum thread
	m_pShowWnd = new CIntMainRenderDlg(1024,768);

	//AfxBeginThread(ThreadProcCalc,&m_Calc);
	//ThreadProcCalc(&m_Calc);
	m_pShowWnd->Create(IDD_SHOWDLG,NULL);
	m_pShowWnd->ShowWindow(SW_SHOW);
	m_pShowWnd->GetWindowRect(R);
	R.left = 0;
	R.top = 0;
	R.right = R.left+m_pShowWnd->iScreenX;
	R.bottom = R.top+m_pShowWnd->iScreenY;
	m_pShowWnd->MoveWindow(R);
	m_pShowWnd->bStartRender = TRUE;	

	

	//CaptureThread = new CaptureTag();  //ARToolkit interface
	//CaptureThread->StartCapture();

	//ARToolkit interface
}

/*
static UINT AFX_CDECL ThreadProcCalc(LPVOID pParam)
{
	CCalc *cp=(CCalc*)pParam;
	cp->execute();
	return 0;
}
*/