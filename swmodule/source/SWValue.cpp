#include "SWValue.h"
#include "SWUtil.h"

bool SWValue::isNumber() const
{
	return ( swrtti_cast<SWNumber>(this) != NULL );
}

bool SWValue::isString() const
{
	return ( swrtti_cast<SWString>(this) != NULL );
}

bool SWValue::isBoolean() const
{
	return ( swrtti_cast<SWBoolean>(this) != NULL );
}

bool SWValue::isTable() const
{
	return ( swrtti_cast<SWTable>(this) != NULL );
}

bool SWValue::isArray() const
{
	return ( swrtti_cast<SWArray>(this) != NULL );
}

bool SWValue::isValue() const
{
	return ( isNumber() || isString() || isBoolean() || isTable() || isArray() );
}

const tdouble& SWValue::asNumber() const
{
	static tdouble static_value = 0;
	static_value = 0;
	{
		SWNumber* object = swrtti_cast<SWNumber>(this);
		if ( object != NULL ) return object->getValue();
	}

	{
		SWString* object = swrtti_cast<SWString>(this);
		if ( object != NULL )
		{
			static_value =  SWUtil.strToNum( object->getValue().str() );
		}
	}
	
	return static_value;
}

const tstring& SWValue::asString() const
{
	SWString* object = swrtti_cast<SWString>(this);
	if ( object != NULL ) return object->getValue().str();
	static tstring static_empty = "";
	return static_empty;
}

const tboolean& SWValue::asBoolean() const
{
	SWBoolean* object = swrtti_cast<SWBoolean>(this);
	if ( object != NULL ) return object->getValue();
	static tboolean static_value = false;
	return static_value;
}

SWValue* SWValue::get( unsigned int index ) const
{
	SWArray* object = swrtti_cast<SWArray>(this);
	if ( object != NULL ) return (SWValue*)object->get( index );
	return NULL;
}

SWValue* SWValue::find( const tstring& key ) const
{
	SWTable* object = swrtti_cast<SWTable>(this);
	if ( object != NULL ) return (SWValue*)object->find( key );
	return NULL;
}

unsigned int SWValue::count() const
{
	SWArray* object = swrtti_cast<SWArray>(this);
	if ( object != NULL ) return object->count();
	return 0;
}
