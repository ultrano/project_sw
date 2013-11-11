#include "SWAction.h"
#include "SWGameObject.h"


void SWAction::onStart()
{
	__super::onStart();
	gameObject()->addUpdateDelegate( GetDelegate( onUpdate ) );
}

void SWAction::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( onUpdate ) );
	__super::onRemove();
}

void SWAction::onUpdate()
{

}
