#ifndef EQUIVALENTANGLEAXIS_H
#define EQUIVALENTANGLEAXIS_H

#include <windows.h>
#include "Angle.h"
#include "Vector.h"

class CAngle3Axis
{
public:
	CAngle alpha;
	CAngle beta;
	CAngle gamma;
	CAngle3Axis();
	~CAngle3Axis();
};

class CEquivalentAngleAxis  
{
public:
	CVector3D m_K;
	CAngle m_theta;

public:
	CEquivalentAngleAxis();
	~CEquivalentAngleAxis();

public:
	int Euler_To_EqvAngle( double (*Rot)[3]); // 오일러 각도를 등가의 각축의 각도로 변환
	int EqvAngle_To_Euler( CEquivalentAngleAxis equvAngle, CAngle3Axis &EulerAngle);
	void EqvAngleSetFromVectorAToVectorB(CVector3D vA, CVector3D vB);
	int RotationMatrix_To_EulerParameter( double (*Rot)[3], CAngle3Axis &EulerAngle);
};

#endif
