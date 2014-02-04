#include "SWString.h"


SWString::SWString()
{

}

SWString::SWString( const thashstr& value )
	: m_value( value )
{

}

SWString::~SWString()
{

}

const thashstr& SWString::getValue()
{
	return m_value;
}

void SWString::setValue( const thashstr& value )
{
	m_value = value;
}
