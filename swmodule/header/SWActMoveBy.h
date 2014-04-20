#ifndef SWActMoveBy_h__
#define SWActMoveBy_h__

#include "SWAct.h"

class SWActMoveBy : public SWAct
{
	SW_RTTI( SWActMoveBy, SWAct );

	float m_spendTime;
	float m_duration;
	tvec3 m_value;

public:

	SWActMoveBy( float duration, const tvec3& val );
	~SWActMoveBy();

	bool isDone();
	bool onStart();
	void onUpdate();
};

#endif // SWActMoveBy_h__