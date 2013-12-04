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

const SWBoolean::Value& SWBoolean::getValue()
{
	return m_value;
}

void SWBoolean::setValue( const SWBoolean::Value& value )
{
	m_value = value;
}