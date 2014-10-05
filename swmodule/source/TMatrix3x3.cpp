#include "TMatrix3x3.h"
#include "SWMath.h"

void TMatrix3x3::set( const TVector2f& scale, float rotate, const TVector2f& move )
{
	float cost = SWMath.cos( rotate );
	float sint = SWMath.sin( rotate );
	m11 = +cost * scale.x;
	m21 = +sint * scale.y;
	m31 = move.x;
	m12 = -sint * scale.x;
	m22 = +cost * scale.y;
	m32 = move.y;
	m13 = 0; m23 = 0; m33 = 1;
}

const TMatrix3x3 operator*( const TMatrix3x3& a, const TMatrix3x3& b )
{
	TMatrix3x3 ret;
	float t1,t2,t3;
	for ( int r = 0 ; r < 3 ; ++r )
	{
		t1 = a.m[r][0]; t2 = a.m[r][1]; t3 = a.m[r][2];
		ret.m[r][0] = (t1*b.m11) + (t2*b.m21) + (t3*b.m31);
		ret.m[r][1] = (t1*b.m12) + (t2*b.m22) + (t3*b.m32);
		ret.m[r][2] = (t1*b.m13) + (t2*b.m23) + (t3*b.m33);
	}
	return ret;
}

const TVector2f operator*( const TVector2f& v, const TMatrix3x3& m )
{
	float w = (v.x*m.m13 + v.y*m.m23) + m.m33;
	if ( w == 0 ) w = FLT_EPSILON;
	return TVector3f
		( (v.x*m.m11 + v.y*m.m21 + m.m31)/w
		, (v.x*m.m12 + v.y*m.m22 + m.m32)/w
		, (v.x*m.m13 + v.y*m.m23 + m.m33)/w );
}

const TVector3f operator*( const TVector3f& v, const TMatrix3x3& m )
{
	return TVector3f
		( (v.x*m.m11 + v.y*m.m21 + v.z*m.m31)
		, (v.x*m.m12 + v.y*m.m22 + v.z*m.m32)
		, (v.x*m.m13 + v.y*m.m23 + v.z*m.m33) );
}