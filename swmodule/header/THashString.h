#ifndef THashString_h__
#define THashString_h__

#include "SWRefCounter.h"
#include "SWPrimaryType.h"

class THashString : public SWMemory
{
	class HashData : public SWRefCountable
	{
		SW_REFERENCE( HashData );
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
	THashString();
	THashString(const tchar* str);
	THashString( const tstring& str );
	THashString( const THashString& copy );
	~THashString();

	const tstring& str() const;
	const tchar*   c_str() const;
	thash32        hash() const;

	THashString& operator = ( const tchar* str );
	THashString& operator = ( const tstring& str );
	THashString& operator = ( const THashString& copy );
	bool operator == ( const THashString& copy ) const;
	bool operator != ( const THashString& copy ) const;
	bool operator <  ( const THashString& copy ) const;

	static const THashString& empty();
};


#endif // THashString_h__
