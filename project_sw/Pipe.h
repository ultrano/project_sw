#ifndef Pipe_h__
#define Pipe_h__

#include "SWHeaders.h"

class Pipe : public SWBehavior
{
	SW_RTTI( Pipe, SWBehavior );

public :

	Pipe( factory_constructor );

protected :

	void onAwake();

private :
};

#endif //! Pipe_h__