#include "Vector.h"

//////////////////////////////////////////////////////////
// 2D Vector :: Constructors
CVector2D::CVector2D (void)
{
	x=0 ; y=0;
}
CVector2D::CVector2D (float a, float b)
{
	x=a; y=b;
}
CVector2D::CVector2D (const CVector2D& v)
{
	x=v.x; y=v.y;
}

//////////////////////////////////////////////////////////
// 2D Vector :: operator

// Assignment
CVector2D& CVector2D::operator =(const CVector2D& v)
{
	x=v.x; y=v.y;
	return *this;
}

// Negative Value
CVector2D& CVector2D::operator -(void)
{
    x=-x; y=-y; 
    return *this;
}

// Add
CVector2D& CVector2D::operator +=(const CVector2D& v)
{
    x+=v.x ;  y+=v.y ;
    return *this ;
}

// Substraction
CVector2D& CVector2D::operator -=(const CVector2D& v)
{
    x-=v.x ;  y-=v.y ;
    return *this ;
}

// Scalar multiplication
CVector2D& CVector2D::operator *=(float r)
{
    x*=r ;  y*=r ;
    return *this ;
}

// Scalar division
CVector2D& CVector2D::operator /=(float r)
{
    x/=r ;  y/=r ;
    return *this ;
}

// Equal check
int CVector2D::operator ==(CVector2D v2)
{
	if (x==v2.x && y==v2.y)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////
// 2D Vector :: operator(friends)

// Addition
CVector2D operator +(const CVector2D& v1, const CVector2D& v2)
{
    CVector2D v(v1.x+v2.x, v1.y+v2.y) ;
    return v ;
}

// Substraction
CVector2D operator -(const CVector2D& v1, const CVector2D& v2)
{
    CVector2D v(v1.x-v2.x, v1.y-v2.y) ;
    return v ;
}

// Scalar product
CVector2D operator *(float scalar, const CVector2D& v1)
{
    CVector2D v(scalar*v1.x, scalar*v1.y) ;
    return v ;
}
CVector2D operator *(const CVector2D& v1, float scalar)
{
    CVector2D v(scalar*v1.x, scalar*v1.y) ;
    return v ;
}
  
// Dot product
float operator *(const CVector2D& v1, const CVector2D& v2)
{
    float dot  = v1.x*v2.x + v1.y*v2.y ;
    return dot ;
}

// Cross product
CVector3D operator ^(const CVector2D& v1, const CVector2D& v2)
{
	CVector3D	v (0, 0, v1.x*v2.y - v1.y*v2.x) ;
    return v ;
}

// Input
istream& operator>> (istream& is, CVector2D& v)
{
    is >> v.x >> v.y ;
    return is ;
}

// Output
ostream& operator<< (ostream& os, const CVector2D& v)
{
    os << v.x << "\t" << v.y<< endl ;
    return os ;
}

//////////////////////////////////////////////////////////
// 2D Vector :: functions

// Make Normalize
void CVector2D::Normalize(void)
{
    float sq = x*x+y*y ;
    sq = sqrt(sq) ;
    x/=sq ; y/=sq ;
}

// Norm Square
float CVector2D::NormSq(void)
{
    float sq = x*x+y*y ;
    return sq ;
}

// Scalar Value
float CVector2D::Scalar(void)
{
    float sq = sqrt(x*x+y*y) ;
    return sq ;
}

// Standard Out
void CVector2D::cout(void)
{
	std::cout <<x<<'\t'<<y<<endl;
}

//////////////////////////////////////////////////////////
// 3D Vector :: Constructors
CVector3D::CVector3D (void)
{
	x=0 ; y=0; z=0 ;
}
CVector3D::CVector3D (float a, float b, float c)
{
	x=a; y=b; z=c ;
}
CVector3D::CVector3D (const CVector3D& v)
{
	x=v.x; y=v.y; z=v.z ;
}

//////////////////////////////////////////////////////////
// 3D Vector :: operator

// Assignment
CVector3D& CVector3D::operator =(const CVector3D& v)
{
	x=v.x; y=v.y; z=v.z;
	return *this;
}

// Negative Value
CVector3D& CVector3D::operator -(void)
{
    x=-x; y=-y; z=-z ; 
    return *this;
}

// Add
CVector3D& CVector3D::operator +=(const CVector3D& v)
{
    x+=v.x ;  y+=v.y ;  z+=v.z ;
    return *this ;
}

// Substraction
CVector3D& CVector3D::operator -=(const CVector3D& v)
{
    x-=v.x ;  y-=v.y ;  z-=v.z ;
    return *this ;
}

// Scalar multiplication
CVector3D& CVector3D::operator *=(float r)
{
    x*=r ;  y*=r ;  z*=r ;  
    return *this ;
}

// Scalar division
CVector3D& CVector3D::operator /=(float r)
{
    x/=r ;  y/=r ;  z/=r ;  
    return *this ;
}

// Equal check
int CVector3D::operator ==(CVector3D v3)
{
	if (x==v3.x && y==v3.y && z==v3.z)
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////
// 3D Vector :: operator(friends)

// Addition
CVector3D operator +(const CVector3D& v1, const CVector3D& v2)
{
    CVector3D v(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z) ;
    return v ;
}

CVector3D operator +(const CVector3D& v1,float scalar)
{
	CVector3D v(v1.x+scalar, v1.y+scalar, v1.z+scalar) ;
    return v ;
}

// Substraction
CVector3D operator -(const CVector3D& v1, const CVector3D& v2)
{
    CVector3D v(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z) ;
    return v ;
}

// Scalar product
CVector3D operator *(float scalar, const CVector3D& v1)
{
    CVector3D v(scalar*v1.x, scalar*v1.y, scalar*v1.z) ;
    return v ;
}
CVector3D operator *(const CVector3D& v1,float scalar)
{
    CVector3D v(scalar*v1.x, scalar*v1.y, scalar*v1.z) ;
    return v ;
}
  
// Dot product
float operator *(const CVector3D& v1, const CVector3D& v2)
{
    float dot  = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z ;
    return dot ;
}

// Cross product
CVector3D operator ^(const CVector3D& v1, const CVector3D& v2)
{
    CVector3D v(v1.y*v2.z-v1.z*v2.y,
	       v1.z*v2.x-v1.x*v2.z,
	       v1.x*v2.y-v1.y*v2.x) ;
    return v ;
}

// Input
istream& operator>> (istream& is, CVector3D& v)
{
    is >> v.x >> v.y >> v.z ;
    return is ;
}

// Output
ostream& operator<< (ostream& os, const CVector3D& v3)
{
    os <<v3.x<<"\t"<<v3.y<<"\t"<<v3.z<<endl ;
    return os ;
}

//////////////////////////////////////////////////////////
// 3D Vector :: functions

// Make Normalize
void CVector3D::Normalize(void)
{
    float sq = x*x+y*y+z*z ;
    sq = sqrt(sq) ;
    x/=sq ; y/=sq ; z/=sq ;
}

// Norm Square
float CVector3D::NormSq(void)
{
    float sq = x*x+y*y+z*z ;
    return sq ;
}

// Scalar Value
float CVector3D::Scalar(void)
{
    float sq = sqrt(x*x+y*y+z*z) ;
    return sq ;
}

// Standard Out
void CVector3D::cout(void)
{
	std::cout <<x<<'\t'<<y<<'\t'<<z<<endl;
}









