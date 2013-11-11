#include "SWAct.h"
#include "SWAction.h"

void SWAct::setAction( SWAction* action )
{
	m_action = action;
}

SWAction* SWAct::getAction()
{
	return m_action();
}
