#ifndef TVector3f_h__
#define TVector3f_h__

#include "TVector2f.h"

class TVector3f
{
public:
	static const TVector3f axisX;
	static const TVector3f axisY;
	static const TVector3f axisZ;
	static const TVector3f zero;
	static const TVector3f one;
public:
	
	float x,y,z;

	TVector3f() {};
	TVector3f( const TVector3f& copy ): x(copy.x), y(copy.y), z(copy.z) {};
	TVector3f(float fx, float fy, float fz) : x(fx), y(fy), z(fz) {};

	TVector3f  operator - () const { return TVector3f(-x, -y, -z); }
	TVector3f& operator = ( const TVector3f& a ) { x = a.x; y = a.y; z = a.z;  return *this; }
	TVector3f& operator += ( const TVector3f& a ) { x += a.x; y += a.y; z += a.z; return *this; }
	TVector3f& operator -= ( const TVector3f& a ) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	TVector3f& operator *= ( float f ) { x *= f; y *= f; z *= f; return *this; }
	TVector3f& operator /= ( float f ) { x /= f; y /= f; z /= f; return *this; }
	bool	  operator == ( const TVector3f& a ) { return ((x==a.x)&&(y==a.y)&&(z==a.z)); };
	bool	  operator != ( const TVector3f& a ) { return !(*this == a);};

	float		length() const;
	float		dot(const TVector3f& v) const;
	TVector3f&	normalize();
};

inline TVector3f operator + ( const TVector3f& a, const TVector3f& b ) { return TVector3f(a.x + b.x, a.y + b.y, a.z + b.z); }
inline TVector3f operator - ( const TVector3f& a, const TVector3f& b ) { return TVector3f(a.x - b.x, a.y - b.y, a.z - b.z); }

inline TVector3f operator * ( const TVector3f& a, float f ) { return TVector3f(a.x * f, a.y * f, a.z * f); }
inline TVector3f operator * ( float f, const TVector3f& a ) { return TVector3f(a.x * f, a.y * f, a.z * f); }

inline TVector3f operator / ( const TVector3f& a, float f ) { return TVector3f(a.x / f, a.y / f, a.z / f); }

#endif // TVector3f_h__
