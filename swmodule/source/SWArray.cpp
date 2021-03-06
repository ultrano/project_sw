#include "SWArray.h"
#include <algorithm>

SWArray::SWArray()
{

}

SWArray::~SWArray()
{
	m_value.clear();
}

SWObject* SWArray::get( unsigned int index ) const
{
	if ( index >= count() ) return NULL;
	return m_value[ index ]();
}

void SWArray::set( unsigned int index, const SWObject* object )
{
	if ( index >= count() ) return;
	m_value[ index ] = object;
}

void SWArray::add( const SWObject* object )
{
	m_value.push_back( object );
}

void SWArray::remove( unsigned int index )
{
	if ( index >= count() ) return;
	Type::iterator last = std::remove( m_value.begin(), m_value.end(), m_value[ index ] );
	m_value.erase( last, m_value.end() );
}

void SWArray::clear()
{
	m_value.clear();
}

void SWArray::copyTo( Type& val )
{
	val = m_value;
}

void SWArray::copyTo( SWArray* val )
{
	if ( swrtti_cast<SWArray>( val ) == NULL) return;
	val->m_value = m_value;
}

unsigned int SWArray::count() const
{
	return m_value.size();
}

SWArray::iterator SWArray::begin()
{
	return m_value.begin();
}

SWArray::iterator SWArray::end()
{
	return m_value.end();
}

SWArray::const_iterator SWArray::begin() const
{
	return m_value.begin();
}

SWArray::const_iterator SWArray::end() const
{
	return m_value.end();
}