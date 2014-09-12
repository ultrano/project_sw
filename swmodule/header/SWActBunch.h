#ifndef SWActBunch_h__
#define SWActBunch_h__

#include "SWAct.h"

class SWActBunch : public SWAct
{
	SW_RTTI( SWActBunch, SWAct );

public:

	SWActBunch( SWAct* act1 = NULL, SWAct* act2 = NULL, SWAct* act3 = NULL );
	SWActBunch( factory_constructor );
	~SWActBunch();

	void addAct( SWAct* act );

	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();

protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	tuint m_countOfDone;
	tarray< SWHardRef<SWAct> > m_acts;
};

#endif // SWActBunch_h__