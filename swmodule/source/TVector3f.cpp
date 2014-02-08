
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

TVector3f TVector3f::cross( const TVector3f& v ) const
{
	TVector3f out;
	out.x = (y * v.z) - (y * v.z);
	out.y = (z * v.x) - (x * v.z);
	out.z = (x * v.y) - (y * v.x);
	return out;
}

TVector3f TVector3f::normalize() const
{
    TVector3f out( 0, 0, 0 );
    float len = length();
    if ( len != 0.0f )
    {
        out.x = x / len;
        out.y = y / len;
		out.z = z / len;
    }
    else
    {
        //! 길이가 0인 벡터에 대한 단위화는 보장하지 않는다. (사전에 체크)
       
        //SWLog( "zero length vector" );
    }
	return out;
}
