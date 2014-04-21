#ifndef SWActScaleTo_h__
#define SWActScaleTo_h__

#include "SWAct.h"
class SWActScaleTo : public SWAct
{
	SW_RTTI( SWActScaleTo, SWAct );
public:
	
	SWActScaleTo( float duration, const tvec3& scaleTo );
	~SWActScaleTo();

private:
	bool isDone();
	bool onStart();
	void onUpdate();
private:
	float m_duration;
	float m_spendTime;
	tvec3 m_scaleTo;
	tvec3 m_scaleFrom;
};

#endif // SWActScaleTo_h__