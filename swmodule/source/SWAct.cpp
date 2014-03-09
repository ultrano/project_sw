#include "SWAct.h"
#include "SWAction.h"


SWAct::SWAct()
{

}

SWAct::~SWAct()
{

}

void SWAct::setAction( SWAction* action )
{
	m_action = action;
}

SWAction* SWAct::getAction()
{
	return m_action();
}