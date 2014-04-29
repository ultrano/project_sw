#ifndef SWActAlphaTo_h__
#define SWActAlphaTo_h__

#include "SWAct.h"

class SWActAlphaTo : public SWAct
{
	SW_RTTI( SWActAlphaTo, SWAct );

public:

	SWActAlphaTo( float duration, float alpha );
	~SWActAlphaTo();

private:

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();

private:

	float m_srcAlpha;
	float m_dstAlpha;
	float m_duration;
	float m_spendTIme;

};
#endif // SWActAlphaTo_h__