#ifndef __SWNumber_h_
#define __SWNumber_h_

#include "SWObject.h"

class SWNumber : public SWObject
{
	SW_RTTI( SWNumber, SWObject );

	double m_value;

public:

	typedef double Value;

	SWNumber();
	SWNumber( const Value& value );
	~SWNumber();

	const double& getValue() { return m_value; };
	void setValue( const double& value ) { m_value = value; };

};


#endif //! __SWNumber_h_