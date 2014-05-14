#ifndef SWActDelay_h__
#define SWActDelay_h__

#include "SWAct.h"

class SWActDelay : public SWAct
{
	SW_RTTI( SWActDelay, SWAct );

public:

	SWActDelay( float delay = 0 );
	~SWActDelay();

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	float m_accum;
	float m_delay;
};

#endif // SWActDelay_h__