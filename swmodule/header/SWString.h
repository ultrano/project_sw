#ifndef SWString_h__
#define SWString_h__

#include "SWObject.h"

class SWString : public SWObject
{
	SW_RTTI( SWString, SWObject );

private:

	tstring m_value;

public:

	SWString();
	SWString( const tstring& value );
	~SWString();

	const tstring& getValue();
	void setValue( const tstring& value );
};

#endif // SWString_h__