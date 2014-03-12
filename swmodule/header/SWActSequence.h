#ifndef SWActSequence_h__
#define SWActSequence_h__

#include "SWAct.h"

class SWActSequence : public SWAct
{
	SW_RTTI( SWActSequence, SWAct );
	
	tuint m_cursor;
	float m_startTime;
	float m_accumTime;
	SWWeakRef<SWAct> m_current;
	tarray< SWHardRef<SWAct> > m_acts;

public:

	SWActSequence();
	~SWActSequence();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate( float delta );
};

#endif // SWActSequence_h__