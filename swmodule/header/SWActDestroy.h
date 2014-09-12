#ifndef SWActDestroy_h__
#define SWActDestroy_h__

#include "SWAct.h"

class SWActDestroy : public SWAct
{
	SW_RTTI( SWActDestroy, SWAct );

public:

	SWActDestroy( float delay = 0 );
	~SWActDestroy();

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	float m_accum;
	float m_delay;
	bool m_destroied;
};

#endif // SWActDestroy_h__