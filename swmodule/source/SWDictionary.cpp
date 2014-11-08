#include "SWDictionary.h"
#include "SWUtil.h"
#include "SWString.h"
#include "SWNumber.h"
#include "SWBoolean.h"

SWDictionary::SWDictionary()
{

}

SWDictionary::~SWDictionary()
{

}

thash32 queryHash( const SWObject* obj )
{
	const SWRtti* rtti = obj->queryRtti();
	thash32 hash = obj->getID();

	if ( rtti == SWString::getRtti() ) hash = ((SWString*)obj)->getValue().hash();
	else if ( rtti == SWNumber::getRtti() ) hash = ((SWNumber*)obj)->getValue();
	else if ( rtti == SWBoolean::getRtti() ) hash = ((SWBoolean*)obj)->getValue();

	return hash;
}

SWObject* SWDictionary::findObject( const SWObject* key ) const
{
	const_iterator itor = find( key );
	if ( itor == end() ) return NULL;
	return itor->second();
}

SWObject* SWDictionary::findObject( const tstring& key ) const
{
	const_iterator itor = find( new SWString( key ) );
	if ( itor == end() ) return NULL;
	return itor->second();
}


SWDictionary::iterator SWDictionary::find( const SWObject* key )
{
	if ( key == NULL ) return m_dic.end();

	const SWRtti* rtti = key->queryRtti();
	thash32 hash = queryHash( key );

	iterator itor = m_dic.begin();
	for ( ; itor != m_dic.end() ; ++itor )
	{
		SWObject* first = itor->first();
		if ( rtti == first->queryRtti() && hash == queryHash( first ) ) break;
	}
	return itor;
}

SWDictionary::iterator SWDictionary::find( const tstring& key )
{
	thash32 hash = SWUtil.stringHash( key );

	iterator itor = m_dic.begin();
	for ( ; itor != m_dic.end() ; ++itor )
	{
		SWObject* first = itor->first();
		if ( SWString::getRtti() == first->queryRtti() && hash == queryHash( first ) ) break;
	}
	return itor;
}

SWDictionary::const_iterator SWDictionary::find( const SWObject* key ) const
{
	SWHardRef<SWObject> holder( key );
	if ( key == NULL ) return m_dic.end();

	const SWRtti* rtti = key->queryRtti();
	thash32 hash = queryHash( key );

	const_iterator itor = m_dic.begin();
	for ( ; itor != m_dic.end() ; ++itor )
	{
		SWObject* first = itor->first();
		if ( rtti == first->queryRtti() && hash == queryHash( first ) ) break;
	}
	return itor;
}

SWDictionary::const_iterator SWDictionary::find( const tstring& key ) const
{
	thash32 hash = SWUtil.stringHash( key );

	const_iterator itor = m_dic.begin();
	for ( ; itor != m_dic.end() ; ++itor )
	{
		SWObject* first = itor->first();
		if ( SWString::getRtti() == first->queryRtti() && hash == queryHash( first ) ) break;
	}
	return itor;
}


void SWDictionary::insert( const SWObject* key, const SWObject* obj )
{
	iterator itor = find( key );
	if ( itor != end() ) itor->second = obj;
	else m_dic.insert( std::make_pair( key, obj ) );
}

void SWDictionary::insert( const tstring& key, const SWObject* obj )
{
	iterator itor = find( key );
	if ( itor != end() ) itor->second = obj;
	else m_dic.insert( std::make_pair( new SWString(key), obj ) );
}


void SWDictionary::remove( const SWObject* key )
{
	iterator itor = find( key );
	if ( itor != end() ) m_dic.erase( itor );
}

void SWDictionary::remove( const tstring& key )
{
	iterator itor = find( key );
	if ( itor != end() ) m_dic.erase( itor );
}


SWDictionary::iterator SWDictionary::begin()
{
	return m_dic.begin();
}

SWDictionary::const_iterator SWDictionary::begin() const
{
	return m_dic.begin();
}

SWDictionary::iterator SWDictionary::end()
{
	return m_dic.end();
}

SWDictionary::const_iterator SWDictionary::end() const
{
	return m_dic.end();
}
