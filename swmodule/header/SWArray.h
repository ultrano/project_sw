#ifndef __SWArray_h_
#define __SWArray_h_

#include "SWObject.h"
#include "SWType.h"

class SWArray : public SWObject
{
	SW_RTTI( SWArray, SWObject );

public:

	typedef SWHardRef<SWArray> Ref;
	typedef SWWeakRef<SWArray> WRef;

	typedef tarray< SWObject::Ref, SWAllocator<SWObject::Ref> > Type;
	typedef Type::iterator iterator;
	typedef Type::const_iterator const_iterator;

private:

	Type m_value;

public:

	SWArray();
	~SWArray();

	SWObject* get( unsigned int index ) const;
	void set( unsigned int index, const SWObject* object );

	void add( const SWObject* object );
	void remove( unsigned int index );
	void clear();
	void copy( Type& val );
	void copy( SWArray* val );

	unsigned int count() const;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;
};

#endif //! __SWArray_h_