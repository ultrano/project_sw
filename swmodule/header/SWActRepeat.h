#ifndef SWActRepeat_h__
#define SWActRepeat_h__

#include "SWAct.h"

class SWActRepeat : public SWAct
{
	SW_RTTI( SWActRepeat, SWAct );

public:

	SWActRepeat( SWAct* act, tuint count = 0 );
	SWActRepeat( factory_constructor );
	~SWActRepeat();

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	SWHardRef<SWAct> m_act;
	tuint m_limitCount;
	tuint m_repeatCount;
};

#endif // SWActRepeat_h__