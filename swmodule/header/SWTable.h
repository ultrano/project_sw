#ifndef SWTable_h__
#define SWTable_h__

#include "SWObject.h"
#include <map>

class SWTable : public SWObject
{
	SW_RTTI( SWTable, SWObject );

public:

	typedef tstring Key;
	typedef ttable< Key, SWHardRef<SWObject> > Value;
	typedef Value::iterator iterator;
	typedef Value::const_iterator const_iterator;

private:

	Value m_table;

public:

	SWTable();
	~SWTable();

	SWObject* find( const tstring& key ) const;
	void insert( const tstring& key, const SWObject* object );
	void remove( const tstring& key );

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;
};


#endif // SWTable_h__