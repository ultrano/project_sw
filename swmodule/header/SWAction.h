#ifndef SWAction_h__
#define SWAction_h__

#include "SWBehavior.h"

class SWAct;
class SWList;
class SWAction : public SWBehavior
{
	SW_RTTI( SWAction, SWBehavior );

public:

	SWAction();
	SWAction( factory_constructor );
	~SWAction();

	void setAct( const tstring& name, SWAct* act );
	SWAct* getAct( const tstring& name );
	void play( const tstring& name );
	void stop();
	bool isPlaying() const;

private:

	void onStart();
	void onRemove();
	void onUpdate();
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	typedef ttable< thashstr, SWHardRef<SWAct> > ActTable;

	ActTable m_actTable;
	SWWeakRef<SWAct> m_next;
	SWWeakRef<SWAct> m_act;
};

#endif // SWAction_h__