#include "SWRefCounter.h"
#include <stddef.h>

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
