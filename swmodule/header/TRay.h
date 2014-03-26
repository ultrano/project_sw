#ifndef TRay_h__
#define TRay_h__

#include "TVector3f.h"

class TRay : public SWMemory
{
public:
	
	TVector3f direction;
	TVector3f origin;

	TRay() : direction( TVector3f::zero ), origin( TVector3f::zero ) {};
	TRay( const TVector3f& _origin, const TVector3f& _direction )
		: direction( _direction )
		, origin( _origin )
	{};
};

#endif // TRay_h__