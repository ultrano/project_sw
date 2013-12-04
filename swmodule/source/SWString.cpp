#include "SWString.h"


SWString::SWString()
{

}

SWString::SWString( const Value& value )
	: m_value( value )
{

}

SWString::~SWString()
{

}

const SWString::Value& SWString::getValue()
{
	return m_value;
}

void SWString::setValue( const SWString::Value& value )
{
	m_value = value;
}
