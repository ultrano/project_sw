#ifndef SWActSequence_h__
#define SWActSequence_h__

#include "SWAct.h"

class SWActSequence : public SWAct
{
	SW_RTTI( SWActSequence, SWAct );

public:

	SWActSequence( SWAct* act1 = NULL, SWAct* act2 = NULL, SWAct* act3 = NULL );
	SWActSequence( factory_constructor );
	~SWActSequence();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:
	
	tuint m_cursor;
	SWWeakRef<SWAct> m_current;
	tarray< SWHardRef<SWAct> > m_acts;
};

#endif // SWActSequence_h__