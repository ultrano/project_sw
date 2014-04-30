#ifndef SWActSendMsg_h__
#define SWActSendMsg_h__

#include "SWAct.h"

class SWActSendMsg : public SWAct
{
	SW_RTTI( SWActSendMsg, SWAct );

public:

	SWActSendMsg( const tstring& eventName );
	~SWActSendMsg();

protected:
	
	virtual bool isDone();
	virtual void onStart();
	virtual void onUpdate();

private:

	tstring m_eventName;

};

#endif // SWActSendMsg_h__