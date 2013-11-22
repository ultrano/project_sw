#ifndef __SWArray_h_
#define __SWArray_h_

#include "SWObject.h"
#include <vector>

class SWArray : public SWObject
{
	SW_RTTI( SWArray, SWObject );

	std::vector< SWHardRef<SWObject> > m_array;

public:

	SWObject* get( unsigned int index );
	void set( unsigned int index, const SWObject* object );
	
	void add( const SWObject* object );
	void remove( unsigned int index );

	 unsigned int count();

};

#endif //! __SWArray_h_