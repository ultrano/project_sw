
#include "SWVector3f.h"
#include <math.h>

const SWVector3f SWVector3f::axisX(1,0,0);
const SWVector3f SWVector3f::axisY(0,1,0);
const SWVector3f SWVector3f::axisZ(0,0,1);
const SWVector3f SWVector3f::zero(0,0,0);
const SWVector3f SWVector3f::one(1,1,1);

float SWVector3f::length() const
{
	return sqrtf( (x * x) + (y * y) + (z * z) );
}

float SWVector3f::dot( const SWVector3f& v ) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

SWVector3f& SWVector3f::normalize()
{
	*this /= length();
	return *this;
}