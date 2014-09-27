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

void TAABB2D::set( const TVector2f& kLower, const TVector2f& kUpper )
{
	lower.x = (kLower.x < kUpper.x)? kLower.x : kUpper.x;
	lower.y = (kLower.y < kUpper.y)? kLower.y : kUpper.y;

	upper.x = (kLower.x > kUpper.x)? kLower.x : kUpper.x;
	upper.y = (kLower.y > kUpper.y)? kLower.y : kUpper.y;
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
