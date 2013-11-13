#ifndef SWAction_h__
#define SWAction_h__

#include "SWComponent.h"

class SWAct;
class SWAction : public SWComponent
{
	SW_RTTI( SWAction, SWComponent );

	SWHardRef<SWAct> m_act;
	float m_spendTime;

	void onStart();
	void onRemove();
	void onUpdate( SWObject* param );
};

#endif // SWAction_h__