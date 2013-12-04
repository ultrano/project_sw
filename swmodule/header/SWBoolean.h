#ifndef SWBoolean_h__
#define SWBoolean_h__

#include "SWObject.h"

class SWBoolean : public SWObject
{
	SW_RTTI( SWBoolean, SWObject );
public:
	typedef bool Value;
private:
	Value m_value;
public:

	SWBoolean();
	SWBoolean( const Value& value );
	~SWBoolean();

	const Value& getValue();;
	void setValue( const Value& value );;

};

#endif // SWBoolean_h__