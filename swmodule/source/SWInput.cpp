#include "SWInput.h"


__SWInput& __SWInput::getInstance()
{
	static __SWInput instance;
	return instance;
}

__SWInput::__SWInput()
	: m_lastKey( 0 )
	, m_keyDownCount( 0 )
{

}

__SWInput::~__SWInput()
{

}

int __SWInput::getTouchState() const
{
	return m_touchState;
}

const tvec2& __SWInput::getTouchXY() const
{
	return m_touchXY;
}

int __SWInput::getDeltaX() const
{
	return m_deltaX;
}

int __SWInput::getDeltaY() const
{
	return m_deltaY;
}

bool __SWInput::getKey( tuint code ) const
{
	if ( code < eKeyCount ) return m_keyFlags[code];
	return false;
}

tuint __SWInput::getLastKey() const
{
	return m_lastKey;
}

void __SWInput::addInputDelegate( SWDelegator* del )
{
	m_listeners.push_back( del );
}

void __SWInput::removeInputDelegate( SWDelegator* del )
{
	m_listeners.remove( del );
}