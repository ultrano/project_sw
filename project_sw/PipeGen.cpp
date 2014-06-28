#include "PipeGen.h"
#include "Pipe.h"

void PipeGen::onAwake()
{
	__super::onAwake();

	SWGameObject* go = gameObject();
	SWAction* action = go->addComponent<SWAction>();
	SWActDelay* act1 = new SWActDelay( 1.5f );
	SWActSendMsg* act2 = new SWActSendMsg( "NewPipe" );
	SWActSequence* act3 = new SWActSequence();

	act3->addAct( act1 );
	act3->addAct( act2 );

	action->setAct( "create", new SWActRepeat( act3 ) );
	action->play( "create" );

	setMessageDelegator( "NewPipe", GetDelegator( newPipe ) );
}

void PipeGen::newPipe()
{
	SWGameObject* go = new SWGameObject;
	go->addComponent<Pipe>();
}