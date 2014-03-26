#ifndef TVector3f_h__
#define TVector3f_h__

#include "SWPrimaryType.h"
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
	TVector3f( const TVector2f& fxy, float fz ) : x(fxy.x), y(fxy.y), z(fz) {};

	float		length() const;
	float		dot( const TVector3f& v ) const;
	TVector3f   cross( const TVector3f& v ) const;
	TVector3f	normal() const;
	TVector2f   xy() const { return TVector2f( x, y ); };

	void        rotateX( float radian );
	void        rotateY( float radian );
	void        rotateZ( float radian );

	TVector3f  operator - () const { return TVector3f(-x, -y, -z); }
	TVector3f  operator +( const TVector3f& pt ) const { return TVector3f( x + pt.x, y + pt.y, z + pt.z ); }
	TVector3f  operator -( const TVector3f& pt ) const {return TVector3f( x - pt.x, y - pt.y, z - pt.z ); }
	TVector3f  operator *( float scalar ) const { return TVector3f( x * scalar, y * scalar, z * scalar ); }
	TVector3f  operator /( float scalar ) const { return TVector3f( x / scalar, y / scalar, z / scalar ); }

	TVector3f& operator += ( const TVector3f& a ) { x += a.x; y += a.y; z += a.z; return *this; }
	TVector3f& operator -= ( const TVector3f& a ) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	TVector3f& operator *= ( float f ) { x *= f; y *= f; z *= f; return *this; }
	TVector3f& operator /= ( float f ) { x /= f; y /= f; z /= f; return *this; }

	bool	  operator == ( const TVector3f& a ) { return ((x==a.x)&&(y==a.y)&&(z==a.z)); };
	bool	  operator != ( const TVector3f& a ) { return !(*this == a);};

	TVector3f& operator = ( const TVector3f& a ) { x = a.x; y = a.y; z = a.z;  return *this; }

};

inline TVector3f operator * ( float f, const TVector3f& a ) { return TVector3f(a.x * f, a.y * f, a.z * f); }

#endif // TVector3f_h__
