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

	void set( const TVector2f& point1, const TVector2f& point2 );

	bool contains( const TAABB2D& aabb ) const;
	bool collide( const TAABB2D& aabb ) const;

	void getCenter( TVector2f& center ) const;
	float getPerimeter() const;

	TAABB2D& combine( const TAABB2D& aabb1, const TAABB2D& aabb2 );
	TAABB2D& combine( const TAABB2D& aabb );

	inline void min( const TVector2f& vec )
	{
		lower.x = (lower.x > vec.x)? vec.x:lower.x;
		lower.y = (lower.y > vec.y)? vec.y:lower.y;
	}
	inline void max( const TVector2f& vec )
	{
		upper.x = (upper.x > vec.x)? upper.x:vec.x;
		upper.y = (upper.y > vec.y)? upper.y:vec.y;
	}
};

class TAABB3D : public SWMemory
{
public:

	TVector3f lower;
	TVector3f upper;

	TAABB3D();
	TAABB3D( const TVector3f& kLower, const TVector3f& kUpper );
	TAABB3D( const TAABB3D& aabb1, const TAABB3D& aabb2 );

	void set( const TVector3f& point1, const TVector3f& point2 );

	bool contains( const TAABB3D& aabb ) const;
	bool collide( const TAABB3D& aabb ) const;

	void getCenter( TVector3f& center ) const;
	float getPerimeter() const;

	TAABB3D& combine( const TAABB3D& aabb1, const TAABB3D& aabb2 );
	TAABB3D& combine( const TAABB3D& aabb );
};

#endif //! TAABB_h__