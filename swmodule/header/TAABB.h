#ifndef TAABB_h__
#define TAABB_h__

#include "TVector2f.h"
#include "TVector3f.h"

class TAABB2D : public SWMemory
{
public:

	TVector2f lower;
	TVector2f upper;

	TAABB2D() {}
	TAABB2D( const TVector2f& kLower, const TVector2f& kUpper ) : lower( kLower ), upper( kUpper ) {}

	TVector2f center()
	{
		return TVector2f( (lower.x + upper.x)/2.0f, (lower.y + upper.y)/2.0f );
	}
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