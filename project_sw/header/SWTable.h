#ifndef SWTable_h__
#define SWTable_h__

#include "SWObject.h"
#include <map>

class SWTable : public SWObject
{
	SW_RTTI( SWTable, SWObject );

	typedef std::map< std::string, SWHardRef<SWObject> > Value;

	Value m_table;

public:

	SWTable();
	~SWTable();

	SWObject* find( const std::string& key ) const;
	void insert( const std::string& key, SWObject* object );
	void remove( const std::string& key );

};


#endif // SWTable_h__