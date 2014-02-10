#ifndef SWShape2D_h__
#define SWShape2D_h__

#include "SWType.h"
#include "SWRefCounter.h"

class SWShape2D : public SWRefCountable
{
	SW_RTTI_ROOT( SWShape );
	SW_REFERENCE( SWObject );

public:

	virtual const tvec2& getFarthest( const tvec2& direction ) = 0;

};

#endif //! SWShape2D_h__
