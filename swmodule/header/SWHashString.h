#ifndef SWHashString_h__
#define SWHashString_h__

#include "SWRefCounter.h"
#include "SWPrimaryType.h"

class SWHashString : public SWMemory
{
	class HashData : public SWRefCountable
	{
		SW_REFERNCE( HashData );
	public:
		thash32 hash;
		tstring str;
		HashData(thash32 h, const tstring& s);
		~HashData();
	};

	typedef ttable< thash32, HashData::WRef > CacheTable;
	static CacheTable& cacheTable();

	HashData::Ref m_data;
public:
	SWHashString();
	SWHashString(const tchar* str);
	SWHashString( const tstring& str );
	SWHashString( const SWHashString& copy );
	~SWHashString();

	const tstring& str() const;
	thash32        hash() const;

	SWHashString& operator = ( const tchar* str );
	SWHashString& operator = ( const tstring& str );
	SWHashString& operator = ( const SWHashString& copy );
	bool operator == ( const SWHashString& copy ) const;
	bool operator != ( const SWHashString& copy ) const;
	bool operator <  ( const SWHashString& copy ) const;
};


#endif // SWHashString_h__
