#ifndef SWAction_h__
#define SWAction_h__

#include "SWComponent.h"

class SWAct;
class SWAction : public SWComponent
{
	SW_RTTI( SWAction, SWComponent );

	SWHardRef<SWAct> m_act;
	float m_spendTime;

public:

	void setAct( SWAct* act );
	SWAct* getAct() const;

	void onStart();
	void onRemove();
	void onUpdate();
};

#endif // SWAction_h__