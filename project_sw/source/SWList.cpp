#include "SWList.h"

void SWList::add( const SWObject* object )
{
	m_value.push_back( object );
}

void SWList::remove( const SWObject* object )
{
	m_value.remove( object );
}

void SWList::clear()
{
	m_value.clear();
}

void SWList::copy( Value& val )
{
	val = m_value;
}

void SWList::copy( SWList* val )
{
	if ( swrtti_cast<SWList>( val ) == NULL) return;
	val->m_value = m_value;
}

unsigned int SWList::count() const
{
	return m_value.size();
}

SWList::iterator SWList::begin()
{
	return m_value.begin();
}

SWList::const_iterator SWList::begin() const
{
	return m_value.begin();
}

SWList::iterator SWList::end()
{
	return m_value.end();
}

SWList::const_iterator SWList::end() const
{
	return m_value.end();
}
