#include "SWTable.h"

SWTable::SWTable()
{

}

SWTable::~SWTable()
{

}

SWObject* SWTable::find( const std::string& key ) const
{
	Value::const_iterator itor = m_table.find( key );
	if ( itor == m_table.end() ) return NULL;
	return (itor->second)();
}

void SWTable::insert( const std::string& key, SWObject* object )
{
	m_table.insert( std::make_pair( key, object ) );
}

void SWTable::remove( const std::string& key )
{
	m_table.erase( key );
}
