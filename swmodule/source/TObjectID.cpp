#include "TObjectID.h"

unsigned& uniqueObjectID()
{
	static unsigned unique_id = 0;
	return unique_id;
}

TObjectID::TObjectID()
	: m_id(++uniqueObjectID())
{

}

TObjectID::~TObjectID()
{

}

