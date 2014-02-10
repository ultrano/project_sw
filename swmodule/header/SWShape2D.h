#ifndef SWShape2D_h__
#define SWShape2D_h__

#include "SWType.h"
#include "SWRefCounter.h"
#include "SWRtti.h"

class SWShape2D : public SWRefCountable
{
	SW_RTTI_ROOT( SWShape2D );
	SW_REFERENCE( SWShape2D );

public:

	virtual tvec2 getFarthest( const tvec2& direction ) = 0;

};

class SWCircle : public SWShape2D
{
	SW_RTTI( SWCircle, SWShape2D );
	SW_REFERENCE( SWCircle );
	
	tvec2 m_center;
	float m_radius;

	virtual tvec2 getFarthest( const tvec2& direction );
};


#endif //! SWShape2D_h__
