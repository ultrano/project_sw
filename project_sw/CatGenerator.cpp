#include "CatGenerator.h"
#include "SWGameContext.h"
#include "SWMath.h"
#include "Cat.h"
#include "SWGameObject.h"
#include "SWTime.h"

void CatGenerator::onAwake()
{
	accum = 0;
}

void CatGenerator::onUpdate()
{
	accum -= SWTime.getDeltaTime();
	if ( accum < 0 )
	{
		accum = SWMath.randomInt( 30, 80 )/10.0f;

		SWGameObject* go = new SWGameObject;
		go->addComponent<Cat>();
		go->setName( "cat" );
	}
}
