#ifndef TTriangle_h__
#define TTriangle_h__

#include "TVector2f.h"

class TTriangle : public SWMemory
{
public:

	TVector2f pos1;
	TVector2f pos2;
	TVector2f pos3;

	bool testPoint( const TVector2f& point ) const;

};

#endif //! TTriangle_h__
