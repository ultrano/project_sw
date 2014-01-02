#include "SWString.h"


SWString::SWString()
{

}

SWString::SWString( const tstring& value )
	: m_value( value )
{

}

SWString::~SWString()
{

}

const tstring& SWString::getValue()
{
	return m_value;
}

void SWString::setValue( const tstring& value )
{
	m_value = value;
}
