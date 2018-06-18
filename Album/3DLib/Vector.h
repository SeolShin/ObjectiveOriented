#ifndef VECTOR_H
#define VECTOR_H

#include <windows.h>
#include <math.h>
#include <iostream>

using namespace std;

class CVector3D;

class CVector2D{
  private :
  public :
	union {
		float	vec[1];
		float	x;
	};
	float	y;

	// 2D Vector :: Constructors
	CVector2D (void);
	CVector2D (float a, float b);
	CVector2D (const CVector2D& v);

	// 2D Vector :: operator
	CVector2D& operator =(const CVector2D& v);
	CVector2D& operator -(void);
	CVector2D& operator +=(const CVector2D& v);
	CVector2D& operator -=(const CVector2D& v);
	CVector2D& operator *=(float r);
	CVector2D& operator /=(float r);
	int operator ==(CVector2D v2);

	// 2D Vector :: operator(friends)
	friend CVector2D operator +(const CVector2D& v1, const CVector2D& v2);
	friend CVector2D operator -(const CVector2D& v1, const CVector2D& v2);
	friend CVector2D operator *(float scalar, const CVector2D& v1);
	friend CVector2D operator *(const CVector2D& v1, float scalar);
	friend float    operator *(const CVector2D& v1, const CVector2D& v2);
	friend CVector3D operator ^(const CVector2D& v1, const CVector2D& v2);
	friend istream& operator>> (istream& is, CVector2D& v);
	friend ostream& operator<< (ostream& os, const CVector2D& v);

	// 2D Vector :: functions
	void Normalize(void);
	float NormSq(void);
	float Scalar(void);
	void cout(void);
};


class CVector3D {
  private :
  public :
	union {
		float	vec[1];
		float	x;
	};
	float	y, z;

	// 3D Vector :: Constructors
	CVector3D (void);
	CVector3D (float a, float b, float c);
	CVector3D (const CVector3D& v);

	// 3D Vector :: operator
	CVector3D& operator =(const CVector3D& v);
	CVector3D& operator -(void);
	CVector3D& operator +=(const CVector3D& v);
	CVector3D& operator -=(const CVector3D& v);
	CVector3D& operator *=(float r);
	CVector3D& operator /=(float r);
	int operator ==(CVector3D v3);

	// 3D Vector :: operator(friends)
	friend CVector3D operator +(const CVector3D& v1, const CVector3D& v2);
	friend CVector3D operator +(const CVector3D& v1,float scalar);
	friend CVector3D operator -(const CVector3D& v1, const CVector3D& v2);
	friend CVector3D operator *(float scalar, const CVector3D& v1);
	friend CVector3D operator *(const CVector3D& v1,float scalar);
	friend float operator *(const CVector3D& v1, const CVector3D& v2);
	friend CVector3D operator ^(const CVector3D& v1, const CVector3D& v2);
	friend istream& operator>> (istream& is, CVector3D& v);
	friend ostream& operator<< (ostream& os, const CVector3D& v3);

	// 3D Vector :: functions
	void Normalize(void);
	float NormSq(void);
	float Scalar(void);
	void cout(void);
};

#endif
