#ifndef SWQuaternion_h__
#define SWQuaternion_h__

#include "SWVector3f.h"

class SWQuaternion
{
public:

	float x, y, z;
	float w;

	SWQuaternion() : x(0), y(0), z(0), w(1) {};
	SWQuaternion( const SWQuaternion& copy ): x(copy.x), y(copy.y), z(copy.z), w(copy.w) {};
	SWQuaternion( const SWVector3f& v, float fw) : x(v.x), y(v.y), z(v.z), w(fw) {};
	SWQuaternion(float fx, float fy, float fz, float fw) : x(fx), y(fy), z(fz), w(fw) {};

	SWQuaternion& operator  = ( const SWQuaternion& q ) { vec()  = q.vec(); w  = q.w; return *this; }
	SWQuaternion& operator += ( const SWQuaternion& q ) { vec() += q.vec(); w += q.w; return *this; }
	SWQuaternion& operator -= ( const SWQuaternion& q ) { vec() -= q.vec(); w -= q.w; return *this; }
	SWQuaternion& operator /= ( float f ) { vec() /= f; w /= f; return *this; }
	SWQuaternion& operator *= ( float f ) { vec() *= f; w *= f; return *this; }
	SWQuaternion& operator *= ( const SWQuaternion& q );

	void		identify();
	SWQuaternion&	normalize();
	float		norm() const;
	SWVector3f  vec() const;
	void		conjugate(SWQuaternion& q) const;
	void		inverse(SWQuaternion& q) const;
	SWQuaternion& rotate(const SWVector3f& axis, float radian);
};

inline SWQuaternion operator + ( const SWQuaternion& a, const SWQuaternion& b ) { return SWQuaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline SWQuaternion operator - ( const SWQuaternion& a, const SWQuaternion& b ) { return SWQuaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
SWQuaternion operator * ( const SWQuaternion& a, const SWQuaternion& b );
SWVector3f   operator * ( const SWVector3f& a, const SWQuaternion& b );

#endif // SWQuaternion_h__