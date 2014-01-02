#ifndef SWValue_h__
#define SWValue_h__

#include "SWObject.h"
#include "SWTable.h"
#include "SWArray.h"
#include "SWNumber.h"
#include "SWBoolean.h"
#include "SWString.h"

class SWValue : public SWObject
{
	SW_RTTI( SWValue, SWObject );

public:

	bool isNumber() const;
	bool isString() const;
	bool isBoolean() const;
	bool isTable() const;
	bool isArray() const;
	bool isValue() const;

	const tnumber&  asNumber() const;
	const tstring&  asString() const;
	const tboolean& asBoolean() const;

	SWValue* get( unsigned int index ) const;
	SWValue* find( const tstring& key ) const;

	unsigned int count() const;
};

#endif // SWValue_h__