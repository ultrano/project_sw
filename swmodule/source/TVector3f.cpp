
#include "TVector3f.h"
#include <math.h>

const TVector3f TVector3f::axisX(1,0,0);
const TVector3f TVector3f::axisY(0,1,0);
const TVector3f TVector3f::axisZ(0,0,1);
const TVector3f TVector3f::zero(0,0,0);
const TVector3f TVector3f::one(1,1,1);

float TVector3f::length() const
{
	return sqrtf( (x * x) + (y * y) + (z * z) );
}

float TVector3f::dot( const TVector3f& v ) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

TVector3f& TVector3f::normalize()
{
	*this /= length();
	return *this;
}