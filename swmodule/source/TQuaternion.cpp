#include "TQuaternion.h"

#include <math.h>

TQuaternion& TQuaternion::operator*=( const TQuaternion& q )
{
	*this = *this * q; return *this;
}

TQuaternion operator*( const TQuaternion& a, const TQuaternion& b )
{
	return TQuaternion
		( a.x*b.w + a.w*b.x - a.z*b.y + a.y*b.z /* i */
		, a.y*b.w + a.w*b.y - a.x*b.z + a.z*b.x /* j */
		, a.z*b.w + a.w*b.z - a.y*b.x + a.x*b.y /* k */
		, a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z /* w */ );
}

TVector3f operator*( const TVector3f& a, const TQuaternion& b )
{
	TQuaternion ret(a,0);
	TQuaternion inv;
	b.inverse(inv);
	return (b * TQuaternion(a,0) * inv).vec();
}

void TQuaternion::identify()
{
	x = 0; y = 0; z = 0; w = 1;
}

TQuaternion TQuaternion::normal()
{
	TQuaternion ret( *this );
	if ( float n = norm() ) ret /= n;
	return ret;
}

float TQuaternion::norm() const
{
	return sqrtf( x*x + y*y + z*z + w*w );
}

TVector3f  TQuaternion::vec() const
{
	return (TVector3f&)*this;
}

void TQuaternion::conjugate( TQuaternion& q ) const
{
	q.vec() = -vec();
	q.w   = w;
}

void TQuaternion::inverse( TQuaternion& q ) const
{
	q.w     = (x*x) + (y*y) + (z*z )+ (w*w);
	q.vec() = (-vec()) / q.w;
	q.w     = w / q.w;
}

TQuaternion& TQuaternion::rotate( const TVector3f& axis, float radian )
{
	vec() = axis.normal() * sinf(radian/2.0f);
	w     = cosf(radian/2.0f);
	return *this;
}