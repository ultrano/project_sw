#ifndef SWActDelegate_h__
#define SWActDelegate_h__

#include "SWAct.h"

class SWActDelegate : public SWAct
{
	SW_RTTI( SWActDelegate, SWAct );

	bool m_called;
	SWHardRef<SWDelegator> m_del;

public:

	SWActDelegate( SWDelegator* del );
	~SWActDelegate();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate();
};

#endif // SWActDelegate_h__