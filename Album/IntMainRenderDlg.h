#pragma once

#include "IntRenderDlg.h"
#include "GlRender.h"
#include <afxdisp.h>        // MFC Automation classes
#include "resource.h"		// main symbols

// CIntMainRenderDlg dialog

class CIntMainRenderDlg : public CIntRenderDlg
{
	DECLARE_DYNAMIC(CIntMainRenderDlg)

	INT iScreenX;
	INT iScreenY;
	float fViewAngle;
	BYTE *pData;

	int		iImageSelected;
	BOOL	bRotate;
	BOOL	bThumbNail;
	CPoint	LastPoint;
	float	fViewPoint;
	float	fViewPointTN;
	float	fAutoTarget;
	float	fAutoTargetTN;
	INT		iLastSelectedPic;

public:
	BOOL bStartRender;
	CIntMainRenderDlg(INT iScreenX, INT iScreenY, CWnd* pParent = NULL);   // standard constructor
	virtual ~CIntMainRenderDlg();
	void DrawTN(int i);
	void DrawPic(int i);
	int	GetImageNum(INT id);
	void MakeNTop(int N);
	void ProcessSelection(int xPos, int yPos);
	void GLDrawScene();
	void ResetPicturePos();
	//	void ReadData();

	// Dialog Data
	enum { IDD = IDD_SHOWDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
