#ifndef SWList_h__
#define SWList_h__

#include "SWObject.h"

class SWList : public SWObject
{
	SW_RTTI( SWList, SWObject );

public:

	typedef SWHardRef<SWList> Ref;
	typedef SWWeakRef<SWList> WRef;

	typedef tlist< SWObject::Ref > Type;
	typedef Type::iterator iterator;
	typedef Type::const_iterator const_iterator;

private:

	Type m_value;

public:

	SWList();
	~SWList();

	void add( const SWObject* object );
	void remove( const SWObject* object );
	void clear();
	void copy( Type& val );
	void copy( SWList* val );

	unsigned int count() const;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;
};

#endif // SWList_h__