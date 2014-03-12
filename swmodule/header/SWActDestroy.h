#ifndef SWActDestroy_h__
#define SWActDestroy_h__

#include "SWAct.h"

class SWActDestroy : public SWAct
{
	SW_RTTI( SWActDestroy, SWAct );

	float m_accum;
	float m_delay;
	bool m_destroied;

public:

	SWActDestroy( float delay = 0 );
	~SWActDestroy();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate( float delta );
};

#endif // SWActDestroy_h__