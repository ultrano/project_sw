#ifndef SWAct_h__
#define SWAct_h__

#include "SWObject.h"

class SWAction;

class SWAct : SWObject
{
	SW_RTTI( SWAct, SWObject );

	SWWeakRef<SWAction> m_action;
	bool m_isPaused;

public:

	void setAction( SWAction* action );
	SWAction* getAction();

	void pause() { m_isPaused = true; };
	void resume() { m_isPaused = false; };
	bool isPaused() const { return m_isPaused; };

	virtual float length() { return 0; };
	virtual bool isDone() = 0;
	virtual void onStart() = 0;
	virtual void onUpdate() = 0;

};

#endif // SWAct_h__