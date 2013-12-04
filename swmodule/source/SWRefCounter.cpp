#include "SWRefCounter.h"
#include <stddef.h>

SWRefObject::SWRefObject( SWRefCountable* obj ) : m_obj(obj), m_hard(0), m_weak(0)
{

}

SWRefObject::~SWRefObject()
{

}

void SWRefObject::incHard()
{
	if ( !m_obj ) return;
	++m_hard;
}

void SWRefObject::decHard()
{
	if ( !m_obj ) return;
	--m_hard;
	if (m_hard == 0) 
	{
		if ( m_obj )
		{
			m_obj->_make_zero_ref();
			m_obj = NULL;
		}
	}
	else if ( m_hard < 0 ) m_hard = 0;
}

void SWRefObject::incWeak()
{
	++m_weak;
}

void SWRefObject::decWeak()
{
	if (--m_weak == 0) delete this;
}

SWRefCountable* SWRefObject::getObject() const
{
	return m_obj;
}

int SWRefObject::getHard() const
{
	return m_hard;
}

int SWRefObject::getWeak() const
{
	return m_weak;
}

bool SWRefObject::isUndead() const
{
	return ( getHard() <= 0 && getObject() );
}

SWRefCountable::SWRefCountable()
{
	m_ref = new SWRefObject(this);
	m_ref->incWeak();
}

SWRefCountable::~SWRefCountable()
{
	_make_zero_ref();
}

void SWRefCountable::_on_zero_ref()
{
	delete this;
}

void SWRefCountable::_make_zero_ref()
{
	if ( m_ref )
	{
		SWRefObject* ref = m_ref;
		m_ref = NULL;
		ref->m_hard = 0;
		ref->decWeak();
		_on_zero_ref();
		ref->m_obj  = NULL;
	}
}
