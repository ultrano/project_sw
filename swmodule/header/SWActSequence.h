#ifndef SWActSequence_h__
#define SWActSequence_h__

#include "SWAct.h"

class SWActSequence : public SWAct
{
	SW_RTTI( SWActSequence, SWAct );
	
	tuint m_cursor;
	SWWeakRef<SWAct> m_current;
	tarray< SWHardRef<SWAct> > m_acts;

public:

	SWActSequence();
	~SWActSequence();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );
};

#endif // SWActSequence_h__