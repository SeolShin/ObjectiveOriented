#ifndef ANGLE_H
#define ANGLE_H

#include <windows.h>
#include <math.h>

class CAngle{
  private :
  public :
	static float Pie;
	float Angle;

	// Angle :: Constructor
	CAngle (void);
	CAngle (float fa);

	// Angle :: operator
	CAngle& operator =(const CAngle& a);
	CAngle& operator -(void);
	CAngle& operator +=(const CAngle& a);
	CAngle& operator -=(const CAngle& a);
	CAngle& operator *=(float r);
	CAngle& operator /=(float r);
	int operator ==(CAngle a);
	int operator !=(CAngle a);
	int operator <=(CAngle a);
	int operator >=(CAngle a);
	int operator <(CAngle a);
	int operator >(CAngle a);

	// Angle :: operator(friends)
	friend CAngle operator +(const CAngle& a1, const CAngle& a2);
	friend CAngle operator -(const CAngle& a1, const CAngle& a2);
	friend CAngle operator *(float scalar, const CAngle& a1);
	friend CAngle operator *(const CAngle& a1, float scalar);

	// Angle :: functions
	void Normalize(void);
	float Degree(void);
	float Radian(void);
	void SetDegree(float a);
	void SetRadian(float a);
};

#endif
