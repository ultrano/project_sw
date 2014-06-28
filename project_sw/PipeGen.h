#ifndef PipeGen_h__
#define PipeGen_h__

#include "SWHeaders.h"

class PipeGen : public SWBehavior
{
	SW_RTTI( PipeGen, SWBehavior );

public:

	PipeGen( factory_constructor ) {};
protected:

	void onAwake();

	void newPipe();

private:
};

#endif //! PipeGen_h__