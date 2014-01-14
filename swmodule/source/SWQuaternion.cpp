#include "SWQuaternion.h"

#include <math.h>

SWQuaternion& SWQuaternion::operator*=( const SWQuaternion& q )
{
	*this = *this * q; return *this;
}

SWQuaternion operator*( const SWQuaternion& a, const SWQuaternion& b )
{
	return SWQuaternion
		( a.x*b.w + a.w*b.x - a.z*b.y + a.y*b.z /* i */
		, a.y*b.w + a.w*b.y - a.x*b.z + a.z*b.x /* j */
		, a.z*b.w + a.w*b.z - a.y*b.x + a.x*b.y /* k */
		, a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z /* w */ );
}

SWVector3f operator*( const SWVector3f& a, const SWQuaternion& b )
{
	SWQuaternion ret(a,0);
	SWQuaternion inv;
	b.inverse(inv);
	return (b * SWQuaternion(a,0) * inv).vec();
}

void SWQuaternion::identify()
{
	x = 0; y = 0; z = 0; w = 1;
}

SWQuaternion& SWQuaternion::normalize()
{
	if ( float n = norm() ) *this /= n;
	return *this;
}

float SWQuaternion::norm() const
{
	return sqrtf( x*x + y*y + z*z + w*w );
}

SWVector3f  SWQuaternion::vec() const
{
	return (SWVector3f&)*this;
}

void SWQuaternion::conjugate( SWQuaternion& q ) const
{
	q.vec() = -vec();
	q.w   = w;
}

void SWQuaternion::inverse( SWQuaternion& q ) const
{
	q.w     = (x*x) + (y*y) + (z*z )+ (w*w);
	q.vec() = (-vec()) / q.w;
	q.w     = w / q.w;
}

SWQuaternion& SWQuaternion::rotate( const SWVector3f& axis, float radian )
{
	vec() = SWVector3f(axis).normalize() * sinf(radian/2.0f);
	w     = cosf(radian/2.0f);
	return *this;
}