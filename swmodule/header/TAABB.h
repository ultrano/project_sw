#ifndef TAABB_h__
#define TAABB_h__

#include "TVector2f.h"
#include "TVector3f.h"

class TAABB2D : public SWMemory
{
public:

	TVector2f lower;
	TVector2f upper;

	TAABB2D();
	TAABB2D( const TVector2f& kLower, const TVector2f& kUpper );
	TAABB2D( const TAABB2D& aabb1, const TAABB2D& aabb2 );

	void set( const TVector2f& kLower, const TVector2f& kUpper );

	bool contains( const TAABB2D& aabb ) const;
	bool collide( const TAABB2D& aabb ) const;
	
	void getCenter( TVector2f& center ) const;
	float getPerimeter() const;

	TAABB2D& combine( const TAABB2D& aabb1, const TAABB2D& aabb2 );
	TAABB2D& combine( const TAABB2D& aabb );
};

class TAABB3D : public SWMemory
{
public:

	TVector3f lower;
	TVector3f upper;

	TAABB3D() {}
	TAABB3D( const TVector3f& kLower, const TVector3f& kUpper ) : lower( kLower ), upper( kUpper ) {}

	TVector3f center()
	{
		return TVector3f( (lower.x + upper.x)/2.0f, (lower.y + upper.y)/2.0f, (lower.z + upper.z)/2.0f );
	}
};

#endif //! TAABB_h__