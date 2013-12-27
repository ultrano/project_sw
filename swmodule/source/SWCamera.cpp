#include "SWCamera.h"
#include "SWGameObject.h"

SWHardRef<SWCamera> SWCamera::mainCamera = NULL;

void SWCamera::onAwake()
{
	__super::onAwake();
	gameObject()->addUpdateDelegate( GetDelegate( onUpdate ) );
}

void SWCamera::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate( onUpdate ) );
	__super::onRemove();
}

void SWCamera::onUpdate()
{
}