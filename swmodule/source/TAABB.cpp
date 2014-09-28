#include "TAABB.h"
#include "SWMath.h"

TAABB2D::TAABB2D()
	: lower( TVector2f::zero )
	, upper( TVector2f::zero )
{

}

TAABB2D::TAABB2D( const TVector2f& kLower, const TVector2f& kUpper ) 
	: lower( kLower )
	, upper( kUpper )
{
	set( lower, upper );
}

TAABB2D::TAABB2D( const TAABB2D& aabb1, const TAABB2D& aabb2 )
	: lower( TVector2f::zero )
	, upper( TVector2f::zero )
{
	combine( aabb1, aabb2 );
}

void TAABB2D::set( const TVector2f& point1, const TVector2f& point2 )
{
	lower.x = (point1.x < point2.x)? point1.x : point2.x;
	lower.y = (point1.y < point2.y)? point1.y : point2.y;

	upper.x = (point1.x > point2.x)? point1.x : point2.x;
	upper.y = (point1.y > point2.y)? point1.y : point2.y;
}

bool TAABB2D::contains( const TAABB2D& aabb ) const
{
	bool ret = true;

	ret = ret && (lower.x <= aabb.lower.x);
	ret = ret && (lower.y <= aabb.lower.y);
	ret = ret && (aabb.upper.x <= upper.x);
	ret = ret && (aabb.upper.y <= upper.y);

	return ret;
}

bool TAABB2D::collide( const TAABB2D& aabb ) const
{
	tvec2 dist1 = aabb.lower - upper;
	tvec2 dist2 = lower - aabb.upper;

	bool ret1 = ((dist1.x > 0) || (dist1.y > 0));
	bool ret2 = ((dist2.x > 0) || (dist2.y > 0));

	return (!ret1 && !ret2);
}

void TAABB2D::getCenter( TVector2f& center ) const
{
	center = (lower + upper) * 0.5f;
}

TAABB2D& TAABB2D::combine( const TAABB2D& aabb1, const TAABB2D& aabb2 )
{
	lower.x = SWMath.min( aabb1.lower.x, aabb2.lower.x );
	lower.y = SWMath.min( aabb1.lower.y, aabb2.lower.y );

	upper.x = SWMath.max( aabb1.upper.x, aabb2.upper.x );
	upper.y = SWMath.max( aabb1.upper.y, aabb2.upper.y );

	return *this;
}

TAABB2D& TAABB2D::combine( const TAABB2D& aabb )
{
	return combine( *this, aabb );
}

float TAABB2D::getPerimeter() const
{
	float w = upper.x - lower.x;
	float h = upper.y - lower.y;
	return (w + h) * 2.0f;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



TAABB3D::TAABB3D()
	: lower( TVector3f::zero )
	, upper( TVector3f::zero )
{

}

TAABB3D::TAABB3D( const TVector3f& kLower, const TVector3f& kUpper ) 
	: lower( kLower )
	, upper( kUpper )
{
	set( lower, upper );
}

TAABB3D::TAABB3D( const TAABB3D& aabb1, const TAABB3D& aabb2 )
	: lower( TVector3f::zero )
	, upper( TVector3f::zero )
{
	combine( aabb1, aabb2 );
}

void TAABB3D::set( const TVector3f& point1, const TVector3f& point2 )
{
	lower.x = (point1.x < point2.x)? point1.x : point2.x;
	lower.y = (point1.y < point2.y)? point1.y : point2.y;
	lower.z = (point1.z < point2.z)? point1.z : point2.z;

	upper.x = (point1.x > point2.x)? point1.x : point2.x;
	upper.y = (point1.y > point2.y)? point1.y : point2.y;
	upper.z = (point1.z > point2.z)? point1.z : point2.z;
}

bool TAABB3D::contains( const TAABB3D& aabb ) const
{
	bool ret = true;

	ret = ret && (lower.x <= aabb.lower.x);
	ret = ret && (lower.y <= aabb.lower.y);
	ret = ret && (lower.z <= aabb.lower.z);
	ret = ret && (aabb.upper.x <= upper.x);
	ret = ret && (aabb.upper.y <= upper.y);
	ret = ret && (aabb.upper.z <= upper.z);

	return ret;
}

bool TAABB3D::collide( const TAABB3D& aabb ) const
{
	tvec3 dist1 = aabb.lower - upper;
	tvec3 dist2 = lower - aabb.upper;

	if (dist1.x > 0) return false;
	if (dist1.y > 0) return false;
	if (dist1.z > 0) return false;

	if (dist2.x > 0) return false;
	if (dist2.y > 0) return false;
	if (dist2.z > 0) return false;

	return true;
}

void TAABB3D::getCenter( TVector3f& center ) const
{
	center = (lower + upper) * 0.5f;
}

TAABB3D& TAABB3D::combine( const TAABB3D& aabb1, const TAABB3D& aabb2 )
{
	lower.x = SWMath.min( aabb1.lower.x, aabb2.lower.x );
	lower.y = SWMath.min( aabb1.lower.y, aabb2.lower.y );
	lower.z = SWMath.min( aabb1.lower.z, aabb2.lower.z );

	upper.x = SWMath.max( aabb1.upper.x, aabb2.upper.x );
	upper.y = SWMath.max( aabb1.upper.y, aabb2.upper.y );
	upper.z = SWMath.max( aabb1.upper.z, aabb2.upper.z );

	return *this;
}

TAABB3D& TAABB3D::combine( const TAABB3D& aabb )
{
	return combine( *this, aabb );
}

float TAABB3D::getPerimeter() const
{
	float w = upper.x - lower.x;
	float h = upper.y - lower.y;
	float d = upper.z - lower.z;
	return (w + h + d) * 2.0f;
}