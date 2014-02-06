#include "THashString.h"
#include "SWUtil.h"

THashString::THashString()
{

}

THashString::THashString( const char* str )
{
	*this = str;
}

THashString::THashString( const tstring& str )
{
	*this = str;
}

THashString::THashString( const THashString& copy )
{
	*this = copy;
}

THashString::~THashString()
{
	m_data = NULL;
}

THashString& THashString::operator=( const char* str )
{
	*this = tstring(str);
	return *this;
}

THashString& THashString::operator=( const THashString& copy )
{
	m_data = copy.m_data;
	return *this;
}

THashString& THashString::operator=( const tstring& str )
{
	thash32 hash = SWUtil.stringHash(str);
	CacheTable::iterator itor = cacheTable().find(hash);
	if ( itor != cacheTable().end() && itor->second.isValid() ) m_data = itor->second();
	else m_data = new HashData( hash, str );

	return *this;
}

bool THashString::operator==( const THashString& copy ) const
{
	return (m_data()&&copy.m_data())? m_data()->hash == copy.m_data()->hash : false;
}

bool THashString::operator!=( const THashString& copy ) const
{
	return !( *this == copy );
}

const tstring& THashString::str() const
{
	static tstring empty;
	if (m_data()) return m_data()->str;
	return empty;
}

const tchar* THashString::c_str() const
{
	if (m_data()) return str().c_str();
	return NULL;
}

thash32 THashString::hash() const
{
	if (m_data()) return m_data()->hash; return 0;
}

THashString::CacheTable& THashString::cacheTable()
{
	static CacheTable gStringTable;
	return gStringTable;
}

bool THashString::operator<( const THashString& copy ) const
{
	return hash() < copy.hash();
}

const THashString& THashString::empty()
{
	static THashString static_empty( "" );
	return static_empty;
}

THashString::HashData::HashData( thash32 h, const tstring& s ) : hash(h), str(s)
{
	THashString::CacheTable& ct = cacheTable();
	ct.insert( std::make_pair( hash, this ) );
}

THashString::HashData::~HashData()
{
	CacheTable& ct = cacheTable();
	ct.erase(hash);
}
