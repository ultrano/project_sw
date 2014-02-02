#ifndef SWActSequence_h__
#define SWActSequence_h__

#include "SWAct.h"

class SWActSequence : public SWAct
{
	SW_RTTI( SWActSequence, SWAct );

	int m_cursor;
	SWAct* m_current;
	SWObject::List m_acts;

	SWActSequence();
	~SWActSequence();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate();
};

#endif // SWActSequence_h__