#include "SWHashString.h"
#include "SWUtil.h"

SWHashString::SWHashString()
{

}

SWHashString::SWHashString( const char* str )
{
	*this = str;
}

SWHashString::SWHashString( const tstring& str )
{
	*this = str;
}

SWHashString::SWHashString( const SWHashString& copy )
{
	*this = copy;
}

SWHashString::~SWHashString()
{
	m_data = NULL;
}

SWHashString& SWHashString::operator=( const char* str )
{
	*this = tstring(str);
	return *this;
}

SWHashString& SWHashString::operator=( const SWHashString& copy )
{
	m_data = copy.m_data;
	return *this;
}

SWHashString& SWHashString::operator=( const tstring& str )
{
	thash32 hash = SWUtil.stringHash(str);
	CacheTable::iterator itor = cacheTable().find(hash);
	if ( itor != cacheTable().end() && itor->second.isValid() ) m_data = itor->second();
	else m_data = new HashData( hash, str );

	return *this;
}

bool SWHashString::operator==( const SWHashString& copy ) const
{
	return (m_data()&&copy.m_data())? m_data()->hash == copy.m_data()->hash : false;
}

bool SWHashString::operator!=( const SWHashString& copy ) const
{
	return !( *this == copy );
}

const tstring& SWHashString::str() const
{
	static tstring empty;
	if (m_data()) return m_data()->str;
	return empty;
}

const tchar* SWHashString::c_str() const
{
	if (m_data()) return str().c_str();
	return NULL;
}

thash32 SWHashString::hash() const
{
	if (m_data()) return m_data()->hash; return 0;
}

SWHashString::CacheTable& SWHashString::cacheTable()
{
	static CacheTable gStringTable;
	return gStringTable;
}

bool SWHashString::operator<( const SWHashString& copy ) const
{
	return hash() < copy.hash();
}

const SWHashString& SWHashString::empty()
{
	static SWHashString static_empty( "" );
	return static_empty;
}

SWHashString::HashData::HashData( thash32 h, const tstring& s ) : hash(h), str(s)
{
	SWHashString::CacheTable& ct = cacheTable();
	ct.insert( std::make_pair( hash, this ) );
}

SWHashString::HashData::~HashData()
{
	CacheTable& ct = cacheTable();
	ct.erase(hash);
}
