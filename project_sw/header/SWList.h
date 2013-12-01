#ifndef SWList_h__
#define SWList_h__

#include "SWObject.h"
#include <list>

class SWList : public SWObject
{
	SW_RTTI( SWList, SWObject );

public:

	typedef std::list< SWObject::Ref > Value;
	typedef Value::iterator iterator;
	typedef Value::const_iterator const_iterator;

private:

	Value m_value;

public:

	void add( const SWObject* object );
	void remove( const SWObject* object );
	void clear();
	void copy( Value& val );
	void copy( SWList* val );

	unsigned int count() const;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;
};

#endif // SWList_h__