#include "SWArray.h"
#include <algorithm>
SWObject* SWArray::get( unsigned int index ) const
{
	if ( index >= count() ) return NULL;
	return m_array[ index ]();
}

void SWArray::set( unsigned int index, const SWObject* object )
{
	if ( index >= count() ) return;
	m_array[ index ] = object;
}

void SWArray::add( const SWObject* object )
{
	m_array.push_back( object );
}

void SWArray::remove( unsigned int index )
{
	if ( index >= count() ) return;
	std::remove( m_array.begin(), m_array.end(), m_array[ index ] );
}

unsigned int SWArray::count() const
{
	return m_array.size();
}