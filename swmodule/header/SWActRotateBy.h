#ifndef SWActRotateBy_h__
#define SWActRotateBy_h__

#include "SWAct.h"

class SWActRotateBy : public SWAct
{
	SW_RTTI( SWActRotateBy, SWAct );

	float m_spendTime;
	float m_duration;
	tvec3 m_value;

public:

	SWActRotateBy( float duration, const tvec3& euler );
	~SWActRotateBy();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate( float delta );
};

#endif // SWActRotateBy_h__