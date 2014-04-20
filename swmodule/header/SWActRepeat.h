#ifndef SWActRepeat_h__
#define SWActRepeat_h__

#include "SWAct.h"

class SWActRepeat : public SWAct
{
	SW_RTTI( SWActRepeat, SWAct );

	SWHardRef<SWAct> m_act;
	tuint m_limitCount;
	tuint m_repeatCount;

public:

	SWActRepeat( SWAct* act, tuint count = 0 );
	~SWActRepeat();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate();


};

#endif // SWActRepeat_h__