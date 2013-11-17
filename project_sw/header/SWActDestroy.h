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
	virtual void onStart();
	virtual void onUpdate();
};

#endif // SWActDestroy_h__