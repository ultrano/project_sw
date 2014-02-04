#ifndef SWString_h__
#define SWString_h__

#include "SWObject.h"

class SWString : public SWObject
{
	SW_RTTI( SWString, SWObject );

private:

	thashstr m_value;

public:

	SWString();
	SWString( const thashstr& value );
	~SWString();

	const thashstr& getValue();
	void setValue( const thashstr& value );
};

#endif // SWString_h__