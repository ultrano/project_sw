#include "SWBoolean.h"

SWBoolean::SWBoolean()
{

}

SWBoolean::SWBoolean( const Value& value )
	: m_value( value )
{

}

SWBoolean::~SWBoolean()
{

}

const tboolean& SWBoolean::getValue()
{
	return m_value;
}

void SWBoolean::setValue( const tboolean& value )
{
	m_value = value;
}