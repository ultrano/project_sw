
#include "TVector3f.h"
#include "SWMath.h"
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

TVector3f TVector3f::normal() const
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

TVector3f	TVector3f::scale( const TVector3f& v ) const
{
	return TVector3f( x*v.x, y*v.y, z*v.z );
}

void        TVector3f::rotateX( float radian )
{
	float cosR = SWMath.cos( radian );
	float sinR = SWMath.sin( radian );
	float ay = (y * cosR) - (z * sinR);
	float az = (y * sinR) + (z * cosR);
	y = ay;
	z = az;
}

void        TVector3f::rotateY( float radian )
{
	float cosR = SWMath.cos( radian );
	float sinR = SWMath.sin( radian );
	float ax = (z * sinR) + (x * cosR);
	float az = (z * cosR) - (x * sinR);
	x = ax;
	z = az;
}

void        TVector3f::rotateZ( float radian )
{
	float cosR = SWMath.cos( radian );
	float sinR = SWMath.sin( radian );
	float ax = (x * cosR) - (y * sinR);
	float ay = (x * sinR) + (y * cosR);
	x = ax;
	y = ay;
}