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