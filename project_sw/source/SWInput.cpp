#include "SWInput.h"


__SWInput& __SWInput::getInstance()
{
	static __SWInput instance;
	return instance;
}

__SWInput::__SWInput()
{

}

__SWInput::~__SWInput()
{

}

int __SWInput::getTouchState() const
{
	return m_touchState;
}

int __SWInput::getTouchX() const
{
	return m_touchX;
}

int __SWInput::getTouchY() const
{
	return m_touchY;
}

void __SWInput::addInputDelegate( SWDelegate* del )
{
	m_listeners.push_back( del );
}

void __SWInput::removeInputDelegate( SWDelegate* del )
{
	m_listeners.remove( del );
}

void __SWInput::onHandleEvent( int type, int param1, int param2 )
{
	m_touchState = type;
	m_touchX = param1;
	m_touchY = param2;

	SWObjectList::iterator itor = m_listeners.begin();
	for ( ; itor != m_listeners.end() ; ++itor )
	{
		SWDelegate* del = swrtti_cast<SWDelegate>( (*itor)() );
		del->call(NULL);
	}
}