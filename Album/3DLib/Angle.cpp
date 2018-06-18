#include "Angle.h"

float CAngle::Pie = 3.1415926535897932384626433832795f;

//////////////////////////////////////////////////////////
// Angle :: Constructor
CAngle::CAngle (void)
{
	Angle=0.0f;
}
CAngle::CAngle (float fa)
{
	Angle=fa;
	Normalize();
}

//////////////////////////////////////////////////////////
// Angle :: operator

// Assignment
CAngle& CAngle::operator =(const CAngle& a)
{
	Angle=a.Angle;
	Normalize();
	return *this;
}

// Negative Value
CAngle& CAngle::operator -(void)
{
    Angle=-Angle;
	Normalize();
    return *this;
}

// Add
CAngle& CAngle::operator +=(const CAngle& a)
{
    Angle+=a.Angle ;
	Normalize();
    return *this ;
}

// Substraction
CAngle& CAngle::operator -=(const CAngle& a)
{
    Angle-=a.Angle ;
	Normalize();
    return *this ;
}

// Scalar multiplication
CAngle& CAngle::operator *=(float r)
{
    Angle*=r ;
	Normalize();
    return *this ;
}

// Scalar division
CAngle& CAngle::operator /=(float r)
{
    Angle/=r ;
	Normalize();
    return *this ;
}

// Equal check
int CAngle::operator ==(CAngle a)
{
	if (Angle==a.Angle)
		return TRUE;
	else
		return FALSE;
}

// Not Equal check
int CAngle::operator !=(CAngle a)
{
	if (Angle!=a.Angle)
		return TRUE;
	else
		return FALSE;
}

// Compare
int CAngle::operator <=(CAngle a)
{
	if (Angle<=a.Angle)
		return TRUE;
	else
		return FALSE;
}
int CAngle::operator >=(CAngle a)
{
	if (Angle>=a.Angle)
		return TRUE;
	else
		return FALSE;
}
int CAngle::operator <(CAngle a)
{
	if (Angle<a.Angle)
		return TRUE;
	else
		return FALSE;
}
int CAngle::operator >(CAngle a)
{
	if (Angle>a.Angle)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////
// Angle :: operator(friends)

// Addition
CAngle operator +(const CAngle& a1, const CAngle& a2)
{
    CAngle a(a1.Angle+a2.Angle) ;
    return a ;
}

// Substraction
CAngle operator -(const CAngle& a1, const CAngle& a2)
{
    CAngle a(a1.Angle-a2.Angle) ;
    return a ;
}

// Scalar product
CAngle operator *(float scalar, const CAngle& a1)
{
    CAngle a(scalar*a1.Angle) ;
    return a ;
}
CAngle operator *(const CAngle& a1, float scalar)
{
    CAngle a(scalar*a1.Angle) ;
    return a ;
}
  
//////////////////////////////////////////////////////////
// Angle :: functions

// Make Normalize
void CAngle::Normalize(void)
{
	if(Angle<0.0f)
		Angle+=((float)(((int)((-Angle)/360.0f))+1))*360.0f;
	if(Angle>360.0f)
		Angle-=((float)((int)(Angle/360.0f)))*360.0f;
}

// 360 Base Angle
float CAngle::Degree(void)
{
    return Angle ;
}

// Radian Angle
float CAngle::Radian(void)
{
    return Angle*Pie/180.0f ;
}

// Set 360 Base Angle
void CAngle::SetDegree(float a)
{
	Angle = a;
	Normalize();
}

// Set Radian Angle
void CAngle::SetRadian(float a)
{
	Angle = a*180.0f/Pie;
	Normalize();
}
