
#include "EquivalentAngleAxis.h"
#include "MyDefine.h"

CAngle3Axis::CAngle3Axis()
{
}

CAngle3Axis::~CAngle3Axis()
{
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEquivalentAngleAxis::CEquivalentAngleAxis()
{

}

CEquivalentAngleAxis::~CEquivalentAngleAxis()
{

}

int CEquivalentAngleAxis::Euler_To_EqvAngle( double (*Rot)[3])
{
	m_theta.SetRadian(acos((Rot[0][0]+Rot[1][1]+Rot[2][2]-1)/2)); // acos : cos역함수
	
	// 회전축 k의 크기는 항상 1이다.
	m_K.x = (Rot[2][1]-Rot[1][2]) / 2*sin(m_theta.Radian());
	m_K.y = (Rot[0][2]-Rot[2][0]) / 2*sin(m_theta.Radian());
	m_K.z = (Rot[1][0]-Rot[0][1]) / 2*sin(m_theta.Radian());

	return 1;
}

//int CEquivalentAngleAxis::EqvAngle_To_Euler( CEquivalentAngleAxis equvAngle, ANGLE *EulerAngle)
//{
//	// rotation 행렬 만들기
//	double Rot[3][3];
//	double angle = D2R(equvAngle.m_theta);
//
//	Rot[0][0] = (equvAngle.m_Kx)*(equvAngle.m_Kx)*(1-cos(angle)) + cos(angle);
//	Rot[0][1] = (equvAngle.m_Kx)*(equvAngle.m_Ky)*(1-cos(angle))- (equvAngle.m_Kz)*sin(angle);
//	Rot[0][2] = (equvAngle.m_Kx)*(equvAngle.m_Kz)*(1-cos(angle))+ (equvAngle.m_Ky)*sin(angle);
//
//	Rot[1][0] = (equvAngle.m_Kx)*(equvAngle.m_Ky)*(1-cos(angle))+ (equvAngle.m_Kz)*sin(angle);
//	Rot[1][1] = (equvAngle.m_Ky)*(equvAngle.m_Ky)*(1-cos(angle))+ cos(angle);
//	Rot[1][2] = (equvAngle.m_Ky)*(equvAngle.m_Kz)*(1-cos(angle))- (equvAngle.m_Kx)*sin(angle);
//	
//	Rot[2][0] = (equvAngle.m_Kx)*(equvAngle.m_Kz)*(1-cos(angle))- (equvAngle.m_Ky)*sin(angle);
//	Rot[2][1] = (equvAngle.m_Ky)*(equvAngle.m_Kz)*(1-cos(angle))+ (equvAngle.m_Kx)*sin(angle);
//	Rot[2][2] = (equvAngle.m_Kz)*(equvAngle.m_Kz)*(1-cos(angle))+ cos(angle);


int CEquivalentAngleAxis::EqvAngle_To_Euler( CEquivalentAngleAxis equvAngle, CAngle3Axis &EulerAngle)
{
	// rotation 행렬 만들기
	double Rot[3][3];
	double angle = equvAngle.m_theta.Radian();

	Rot[0][0] = (equvAngle.m_K.x)*(equvAngle.m_K.x)*(1-cos(angle)) + cos(angle);
	Rot[0][1] = (equvAngle.m_K.x)*(equvAngle.m_K.y)*(1-cos(angle))- (equvAngle.m_K.z)*sin(angle);
	Rot[0][2] = (equvAngle.m_K.x)*(equvAngle.m_K.z)*(1-cos(angle))+ (equvAngle.m_K.y)*sin(angle);

	Rot[1][0] = (equvAngle.m_K.x)*(equvAngle.m_K.y)*(1-cos(angle))+ (equvAngle.m_K.z)*sin(angle);
	Rot[1][1] = (equvAngle.m_K.y)*(equvAngle.m_K.y)*(1-cos(angle))+ cos(angle);
	Rot[1][2] = (equvAngle.m_K.y)*(equvAngle.m_K.z)*(1-cos(angle))- (equvAngle.m_K.x)*sin(angle);
	
	Rot[2][0] = (equvAngle.m_K.x)*(equvAngle.m_K.z)*(1-cos(angle))- (equvAngle.m_K.y)*sin(angle);
	Rot[2][1] = (equvAngle.m_K.y)*(equvAngle.m_K.z)*(1-cos(angle))+ (equvAngle.m_K.x)*sin(angle);
	Rot[2][2] = (equvAngle.m_K.z)*(equvAngle.m_K.z)*(1-cos(angle))+ cos(angle);

	// rotation행렬로부터 alpha, beta,gamma 알아내기 
	double temp; // 라디안
	EulerAngle.beta.SetRadian(atan2( -Rot[2][0], sqrt( SQUARE(Rot[0][0])+SQUARE(Rot[1][0]) )));
	if( (temp = cos(EulerAngle.beta.Radian())) != 0)
	{
		EulerAngle.gamma.SetRadian(atan2( Rot[1][0]/temp, Rot[0][0]/temp));
		EulerAngle.alpha.SetRadian(atan2( Rot[2][1]/temp, Rot[2][2]/temp));
	} else {
		EulerAngle.gamma = 0;
		EulerAngle.beta = 90; // 또는 -90
		EulerAngle.alpha.SetRadian(atan2( Rot[0][1], Rot[1][1])); // 또는 -atan2( Rot[0][1], Rot[1][1])
		return -1;
	}

// 	// rotation행렬로부터 alpha, beta,gamma 알아내기 
// 	double temp; // 라디안
// 	EulerAngle->beta = atan2( -Rot[2][0], sqrt( SQUARE(Rot[0][0])+SQUARE(Rot[1][0]) ));
// 	if( (temp = cos(EulerAngle->beta)) != 0)
// 	{
// 		EulerAngle->alpha = atan2( Rot[1][0]/temp, Rot[0][0]/temp);
// 		EulerAngle->gamma = atan2( Rot[2][1]/temp, Rot[2][2]/temp);
// 	}
// 	else
// 	{
// 		EulerAngle->alpha = 0;
// 		EulerAngle->beta = 90; // 또는 -90
// 		EulerAngle->gamma = atan2( Rot[0][1], Rot[1][1]); // 또는 -atan2( Rot[0][1], Rot[1][1])
// 		EulerAngle->gamma = R2D(EulerAngle->gamma);
// 		return -1;
// 	}
// 
// 	EulerAngle->alpha = R2D(EulerAngle->alpha);
// 	EulerAngle->beta = R2D(EulerAngle->beta);
// 	EulerAngle->gamma = R2D(EulerAngle->gamma);

	return 1;
}

void CEquivalentAngleAxis::EqvAngleSetFromVectorAToVectorB(CVector3D vA, CVector3D vB)
{
   /* 벡터A와 벡터B사이의 등가의 회전축과 회전각 구하기 */

   //vA와 vB의 사이각을 구한다.
   float fRadian = acosf(vA*vB / (vA.Scalar() * vB.Scalar()));
	   
   //완전히 정반대벡터이면 외적 축을 구할수 없기때문에 보정한다.
   CVector3D vA2(vA);
   if(PI - fRadian < 0.0005f) 
	   vA2 = vA2 + 0.0005f;

   //vA와 vB에 직교하는 벡터를 구한다.(cross)
   CVector3D vCross;
   vCross=vA2^vB;
   vCross.Normalize();

   m_K = vCross;
   m_theta.SetRadian(fRadian);
}

int CEquivalentAngleAxis::RotationMatrix_To_EulerParameter( double (*Rot)[3], CAngle3Axis &EulerAngle)
{
	// rotation행렬로부터 alpha, beta,gamma 알아내기 
	double temp; // 라디안
	EulerAngle.beta.SetRadian(atan2( -Rot[2][0], sqrt( SQUARE(Rot[0][0])+SQUARE(Rot[1][0]) )));
	if( (temp = cos(EulerAngle.beta.Radian())) != 0)
	{
		EulerAngle.gamma.SetRadian(atan2( Rot[1][0]/temp, Rot[0][0]/temp));
		EulerAngle.alpha.SetRadian(atan2( Rot[2][1]/temp, Rot[2][2]/temp));
	} else {
		EulerAngle.gamma = 0;
		EulerAngle.beta = 90; // 또는 -90
		EulerAngle.alpha.SetRadian(atan2( Rot[0][1], Rot[1][1])); // 또는 -atan2( Rot[0][1], Rot[1][1])
		return -1;
	}

	return 1;
}