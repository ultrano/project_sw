#ifndef SWActContinue_h__
#define SWActContinue_h__

#include "SWAct.h"

class SWActContinue : public SWAct
{
	SW_RTTI( SWActContinue, SWAct );

	SWHardRef<SWAct> m_act;

public:

	SWActContinue( SWAct* act );
	~SWActContinue();

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();


};

#endif // SWActContinue_h__