#include "SWTable.h"

SWTable::SWTable()
{

}

SWTable::~SWTable()
{

}

SWObject* SWTable::find( const tstring& key ) const
{
	Value::const_iterator itor = m_table.find( key );
	if ( itor == m_table.end() ) return NULL;
	return (itor->second)();
}

void SWTable::insert( const tstring& key, const SWObject* object )
{
	m_table.insert( std::make_pair( key, object ) );
}

void SWTable::remove( const tstring& key )
{
	m_table.erase( key );
}

SWTable::iterator SWTable::begin()
{
	return m_table.begin();
}

SWTable::const_iterator SWTable::begin() const
{
	return m_table.begin();
}

SWTable::iterator SWTable::end()
{
	return m_table.end();
}

SWTable::const_iterator SWTable::end() const
{
	return m_table.end();
}
