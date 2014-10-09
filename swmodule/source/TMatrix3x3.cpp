#include "TMatrix3x3.h"
#include "SWMath.h"

float matDet33( unsigned square, float* m )
{
	if ( square == 1 ) return *m;
	float ret = 0;
	for ( unsigned i = 0 ; i < square ; ++i )
	{
		int k = 0;
		float sub[9] = {0};
		for ( unsigned r = 1 ; r < square ; ++r )
		{
			for ( unsigned c = 0 ; c < square ; ++c )
			{
				if ( i == c ) continue;
				sub[k++] = (m[(r*square) + c]);
			}
		}
		float cof = ((i%2)? -1:+1) * matDet33(square-1, &sub[0]);
		ret += m[i] * cof;
	}
	return ret;
}

TMatrix3x3::TMatrix3x3()
{
	identity();
}

void TMatrix3x3::set( const TVector2f& scale, float rotate, const TVector2f& move )
{
	float cost = SWMath.cos( rotate );
	float sint = SWMath.sin( rotate );
	
	m11 = +cost * scale.x;
	m12 = +sint * scale.x;
	m21 = -sint * scale.y;
	m22 = +cost * scale.y;

	m31 = move.x; m32 = move.y;
	m13 = 0; m23 = 0; m33 = 1;
}

void TMatrix3x3::identity()
{
	m11=(1); m12=(0); m13=(0);
	m21=(0); m22=(1); m23=(0);
	m31=(0); m32=(0); m33=(1);
}

float TMatrix3x3::determinant() const
{
	return matDet33(3, (float*)this);
}

float TMatrix3x3::minorDet( unsigned char row, unsigned char col ) const
{
	int i = 0;
	float minor[2 * 2] = {0};
	for ( int r = 0 ; r < 3 ; ++r )
	{
		if ( r == row ) continue;
		for ( int c = 0 ; c < 3 ; ++c )
		{
			if ( c == col ) continue;
			minor[i++] = ((float*)this)[(r*3) + c];
		}
	}
	return matDet33(2,minor);
}

void TMatrix3x3::inverse( TMatrix3x3& m ) const
{
	adjoint(m);
	float det = determinant();
	if ( det != 0.0f ) m *= 1.0f/det;
}

void TMatrix3x3::adjoint( TMatrix3x3& m ) const
{
	for ( int r = 0 ; r < 3 ; ++r )
	{
		for ( int c = 0 ; c < 3 ; ++c )
		{
			m.m[c][r] = (((r+c)%2)? -1:+1) * minorDet( r, c );
		}
	}
}

void TMatrix3x3::transpose( TMatrix3x3& t ) const
{
	for ( int r = 0 ; r < 3 ; ++r )
	{
		for ( int c = 0 ; c < r ; ++c )
		{
			if( c == r ) continue;

			float f = m[r][c];
			t.m[r][c] = m[c][r];
			t.m[c][r] = f;
		}
	}
}

TVector3f TMatrix3x3::row( unsigned char r ) const
{
	if ( r < 3 ) return TVector3f( m[r][0], m[r][1], m[r][2] );
	return TVector3f::zero;
}

TVector3f TMatrix3x3::col( unsigned char c ) const
{
	if ( c < 3 ) return TVector3f( m[0][c], m[1][c], m[2][c] );
	return TVector3f::zero;
}

TMatrix3x3& TMatrix3x3::operator=( const TMatrix3x3& mat )
{
	memcpy( this, &mat, sizeof(TMatrix3x3) );
	return *this;
}

TMatrix3x3& TMatrix3x3::operator*=( float f )
{
	for ( int i = 0 ; i < 9 ; ++i ) ((float*)this)[i] *= f;
	return *this;
}

TMatrix3x3& TMatrix3x3::operator*=( const TMatrix3x3& mat )
{
	float t1,t2,t3;
	for ( int r = 0 ; r < 4 ; ++r )
	{
		t1 = m[r][0]; t2 = m[r][1]; t3 = m[r][2];
		m[r][0] = (t1*mat.m11) + (t2*mat.m21) + (t3*mat.m31);
		m[r][1] = (t1*mat.m12) + (t2*mat.m22) + (t3*mat.m32);
		m[r][2] = (t1*mat.m13) + (t2*mat.m23) + (t3*mat.m33);
	}
	return *this;
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