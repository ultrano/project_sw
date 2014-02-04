#ifndef SWTable_h__
#define SWTable_h__

#include "SWObject.h"

class SWTable : public SWObject
{
	SW_RTTI( SWTable, SWObject );

public:

	typedef thashstr Key;
	typedef ttable< Key, SWObject::Ref > Value;
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