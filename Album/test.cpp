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
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_Progress1.SetRange(0,MAX);
	m_Calc.init(MAX);
	AfxBeginThread(ThreadProcCalc,&m_Calc);	//���[�J�[�X���b�h�̊J�n�Fm_Calc.execute()���Ӗ�����

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

LRESULT CAboutDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
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