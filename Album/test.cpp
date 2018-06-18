#include "test.h"

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_bDraw=FALSE;
}

static UINT AFX_CDECL ThreadProcCalc(LPVOID pParam)
{
	CCalc *cp=(CCalc*)pParam;
	cp->execute();
	cp->setEnd();
	return 0;
}

const int MAX=1000;

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_Progress1.SetRange(0,MAX);
	m_Calc.init(MAX);
	AfxBeginThread(ThreadProcCalc,&m_Calc);	//ワーカースレッドの開始：m_Calc.execute()を意味する

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

LRESULT CAboutDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if(message==WM_PAINT) m_bDraw=TRUE;

	if(m_Progress1.m_hWnd && m_bDraw){
		if(m_nValue<MAX){
			m_nValue++;
		}
		m_Progress1.SetPos(m_nValue);
		UpdateData(FALSE);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}