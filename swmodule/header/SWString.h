#ifndef SWString_h__
#define SWString_h__

#include "SWObject.h"
#include <string>

class SWString : public SWObject
{
	SW_RTTI( SWString, SWObject );

public:

	typedef std::string Value;

private:

	Value m_value;

public:

	SWString();
	SWString( const Value& value );
	~SWString();

	const Value& getValue();
	void setValue( const Value& value );
};

#endif // SWString_h__