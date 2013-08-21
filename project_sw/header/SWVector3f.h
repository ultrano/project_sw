#ifndef SWVector3f_h__
#define SWVector3f_h__

class SWVector3f
{
public:
	static const SWVector3f axisX;
	static const SWVector3f axisY;
	static const SWVector3f axisZ;
	static const SWVector3f zero;
	static const SWVector3f one;
public:
	union
	{
		struct { float x,y,z; };
		struct { float v1,v2,v3; };
	};

	SWVector3f() {};
	SWVector3f( const SWVector3f& copy ): x(copy.x), y(copy.y), z(copy.z) {};
	SWVector3f(float fx, float fy, float fz) : x(fx), y(fy), z(fz) {};

	SWVector3f  operator - () const { return SWVector3f(-x, -y, -z); }
	SWVector3f& operator = ( const SWVector3f& a ) { x = a.x; y = a.y; z = a.z;  return *this; }
	SWVector3f& operator += ( const SWVector3f& a ) { x += a.x; y += a.y; z += a.z; return *this; }
	SWVector3f& operator -= ( const SWVector3f& a ) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	SWVector3f& operator *= ( float f ) { x *= f; y *= f; z *= f; return *this; }
	SWVector3f& operator /= ( float f ) { x /= f; y /= f; z /= f; return *this; }
	bool	  operator == ( const SWVector3f& a ) { return ((x==a.x)&&(y==a.y)&&(z==a.z)); };
	bool	  operator != ( const SWVector3f& a ) { return !(*this == a);};

	float		length() const;
	float		dot(const SWVector3f& v) const;
	SWVector3f&	normalize();
};

inline SWVector3f operator + ( const SWVector3f& a, const SWVector3f& b ) { return SWVector3f(a.x + b.x, a.y + b.y, a.z + b.z); }
inline SWVector3f operator - ( const SWVector3f& a, const SWVector3f& b ) { return SWVector3f(a.x - b.x, a.y - b.y, a.z - b.z); }

inline SWVector3f operator * ( const SWVector3f& a, float f ) { return SWVector3f(a.x * f, a.y * f, a.z * f); }
inline SWVector3f operator * ( float f, const SWVector3f& a ) { return SWVector3f(a.x * f, a.y * f, a.z * f); }

inline SWVector3f operator / ( const SWVector3f& a, float f ) { return SWVector3f(a.x / f, a.y / f, a.z / f); }

#endif // SWVector3f_h__