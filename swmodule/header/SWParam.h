#ifndef __SWParam_h_
#define __SWParam_h_

#include "SWArray.h"

class SWParam : public SWObject
{
	SW_RTTI( SWParam, SWObject );

	SWHardRef<SWArray> m_params;

public:

	SWParam( SWArray* params );
	~SWParam();
	const SWObject* get( unsigned int index );
	unsigned int count();

};

#endif //!__SWParam_h_