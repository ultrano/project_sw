#ifndef SWAction_h__
#define SWAction_h__

#include "SWComponent.h"

class SWAct;
class SWList;
class SWAction : public SWComponent
{
	SW_RTTI( SWAction, SWComponent );

	SWHardRef<SWList> m_actList;
	float m_spendTime;

public:

	SWAction();
	~SWAction();

	void runAct( SWAct* act );
	void stopAct( SWAct* act );

	void onStart();
	void onRemove();
	void onUpdate();
};

#endif // SWAction_h__