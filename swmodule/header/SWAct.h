#ifndef SWAct_h__
#define SWAct_h__

#include "SWObject.h"

class SWAction;

class SWAct : public SWObject
{
	SW_RTTI( SWAct, SWObject );

	SWWeakRef<SWAction> m_action;

public:

	SWAct();
	~SWAct();

	void setAction( SWAction* action );
	SWAction* getAction();


	virtual bool isDone() = 0;
	virtual bool onStart() = 0;
	virtual void onUpdate( float delta ) = 0;

};

#endif // SWAct_h__