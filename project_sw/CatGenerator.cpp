#include "CatGenerator.h"
#include "SWGameContext.h"
#include "SWMath.h"
#include "Cat.h"
#include "SWGameObject.h"
#include "SWTime.h"
#include "SWTable.h"
#include "SWNumber.h"

void CatGenerator::onAwake()
{
	SWTable* config = swrtti_cast<SWTable>(SW_GC.findItem("config"));
	SWNumber* catGen = swrtti_cast<SWNumber>(config->find("catGen"));
	genTime = catGen->getValue();
	accum = 0;
}

void CatGenerator::onUpdate()
{
	accum -= SWTime.getDeltaTime();
	if ( accum < 0 )
	{
		accum = genTime;

		SWGameObject* go = new SWGameObject;
		go->defineProp( "spriteData" );
		go->setProp( "spriteData", gameObject()->getProp( "spriteData" ) );
		go->addComponent<Cat>();
		go->setName( "cat" );
	}
}
