/*

uint RSHash( const string& str )
{
	uint b    = 378551;
	uint a    = 63689;
	uint hash = 0;

	for(std::size_t i = 0; i < str.length(); i++)
	{
		hash = hash * a + str[i];
		a    = a * b;
	}

	return (hash & 0x7FFFFFFF);
}

HashString::HashString()
{

}

HashString::HashString( const char* str )
{
	*this = str;
}

HashString::HashString( const string& str )
{
	*this = str;
}

HashString::HashString( const HashString& copy )
{
	*this = copy;
}

#pragma optimize("fuck",off)
HashString::~HashString()
{
	m_data = NULL;
}
#pragma optimize("fuck",on)

HashString& HashString::operator=( const char* str )
{
	*this = string(str);
	return *this;
}

HashString& HashString::operator=( const HashString& copy )
{
	m_data = copy.m_data;
	return *this;
}

HashString& HashString::operator=( const string& str )
{
	uint hash = RSHash(str);
	map<uint,WeakRef<StringData>>::iterator itor = cacheTable().find(hash);
	if ( itor != cacheTable().end() && itor->second.isValid() ) m_data = itor->second();
	else m_data = new StringData( hash, str );

	return *this;
}

bool HashString::operator==( const HashString& copy ) const
{
	return (m_data()&&copy.m_data())? m_data()->hash == copy.m_data()->hash : false;
}

bool HashString::operator!=( const HashString& copy ) const
{
	return !( *this == copy );
}

const string& HashString::str() const
{
	static string empty;
	if (m_data()) return m_data()->str;
	return empty;
}

uint HashString::hash() const
{
	if (m_data()) return m_data()->hash; return 0;
}

HashString::CacheTable& HashString::cacheTable()
{
	static map<uint,WeakRef<StringData>> gStringTable;
	return gStringTable;
}

bool HashString::operator<( const HashString& copy ) const
{
	return hash() < copy.hash();
}

HashString::StringData::StringData( uint h, const string& s ) : hash(h), str(s)
{
	CacheTable& ct = cacheTable();
	ct.insert( make_pair( hash, this ) );
}

HashString::StringData::~StringData()
{
	CacheTable& ct = cacheTable();
	ct.erase(hash);
}



*/