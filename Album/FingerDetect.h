#pragma once

//#include "AREShowDlg.h"
//#include "stdafx.h"

#include "IntMainRenderDlg.h"

#include <stdio.h>
#include <OpenCV/cv.h>
#include <OpenCV/cxcore.h>
#include <OpenCV/highgui.h>

class FingerDetect : public CIntRenderDlg
{
	FingerDetect *CaptureThread1;
	FingerDetect *CaptureThread2;
	
public:
	int iSelectRightFlag();
	int iSelectLeftFlag();
	int ikey;

	float fDistance;
	float fKeyRight;
	float fKeyLeft;

	void Capture();
	void SkinDetect();
	void GetSkinMask(IplImage* src_RGB, IplImage *mask_BW,int erosions, int dilations);
	void DetectFinger();
	void CalDistance();	
	
	//void CalDistance(float fPlace);
	//void CapturePlace();
	//void CalcWindowRect();
	//void CalcWindowRect(float fRect);

private:
    int i,j,k;
	CvVideoWriter* VideoWriter;
	char* filename;
	double dfps;
	IplImage* src_RGB;
	IplImage *mask_BW;
	int erosions;
	int dilations;

};