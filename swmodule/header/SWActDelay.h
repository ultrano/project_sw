#ifndef SWActDelay_h__
#define SWActDelay_h__

#include "SWAct.h"

class SWActDelay : public SWAct
{
	SW_RTTI( SWActDelay, SWAct );

	float m_accum;
	float m_delay;

public:

	SWActDelay( float delay = 0 );
	~SWActDelay();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate();
};

#endif // SWActDelay_h__