#ifndef SWActBunch_h__
#define SWActBunch_h__

#include "SWAct.h"

class SWActBunch : public SWAct
{
	SW_RTTI( SWActBunch, SWAct );

public:

	SWActBunch();
	SWActBunch( factory_constructor );
	~SWActBunch();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();

protected:

	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	tuint m_countOfDone;
	tarray< SWHardRef<SWAct> > m_acts;
};

#endif // SWActBunch_h__