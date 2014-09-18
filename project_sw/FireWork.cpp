#include "FireWork.h"

FireWork::FireWork( factory_constructor )
{

}

FireWork::~FireWork()
{

}

void FireWork::onStart()
{
	{
		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "destroy", new SWActSequence( new SWActDelay(5), new SWActDestroy ) );
		action->play( "destroy" );

		SWTransform* trans = getComponent<SWTransform>();
		float radian = SWMath.angleToRadian( SWMath.randomInt(0,360) );
		trans->setLocalRotate( tvec3( 0, 0, radian ) );
	}

	{
		SWGameObject* go = new SWGameObject;

		tcolor rgba( SWMath.randomFloat(), SWMath.randomFloat(), SWMath.randomFloat(), 1 );
		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setColor( rgba );
		rgba.a = 0;
		
		SWHardRef<SWSpriteAnimation> spriteAnim = SWAssets.loadSpriteAnimation( "animations/boom_anim.txt" );
		SWActAnimate* anim = new SWActAnimate( 1, spriteAnim()->getSequenceAt(0) );
		SWActBunch* bunch = new SWActBunch();
		bunch->addAct( anim );
		bunch->addAct( new SWActColorTo(0.5f,rgba) );
		bunch->addAct( new SWActScaleTo(0.5f,tvec3::one ) );

		SWAction* action = go->addComponent<SWAction>();
		action->setAct( "boom", bunch );
		action->play( "boom" );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setParent( getComponent<SWTransform>() );
		trans->setLocalScale( tvec3::zero );
	}

	{
		SWGameObject* go = new SWGameObject;

		tcolor rgba( SWMath.randomFloat(), SWMath.randomFloat(), SWMath.randomFloat(), 1 );
		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		renderer->setColor( rgba );
		rgba.a = 0;

		SWHardRef<SWSpriteAnimation> spriteAnim = SWAssets.loadSpriteAnimation( "animations/twinkle_anim.txt" );
		SWActAnimate* anim = new SWActAnimate( 1, spriteAnim()->getSequenceAt(0) );
		SWActBunch* bunch = new SWActBunch();
		bunch->addAct( anim );
		bunch->addAct( new SWActSequence( new SWActDelay(0.5f), new SWActColorTo(0.5f,rgba) ) );
		bunch->addAct( new SWActScaleTo(1,tvec3::one*0.6f) );

		SWAction* action = go->addComponent<SWAction>();
		action->setAct( "twinkle", bunch );
		action->play( "twinkle" );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setParent( getComponent<SWTransform>() );
		trans->setLocalScale( tvec3::zero );
	}
}

void FireWork::onFixedRateUpdate()
{

}