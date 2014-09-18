#include "FireWork.h"

FireWork::FireWork( factory_constructor )
{

}

FireWork::~FireWork()
{

}

FireWork* FireWork::createFireWork()
{
	SWGameObject* go = SW_GC.getScene()->findGO( "Pool/FireWork" );
	if ( go == NULL ) go = new SWGameObject;
	return go->addComponent<FireWork>();
}

void FireWork::onAwake()
{
	__super::onAwake();

	//! game object
	{
		gameObject()->setName( queryRtti()->name );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->setAct( "fire", new SWActSequence( new SWActDelay(5), new SWActDelegate(GetDelegator(goToPool)) ) );
	}

	//! boom cloud
	{
		SWGameObject* go = new SWGameObject;
		go->setName( "Boom" );

		SWSpriteRenderer* renderer = go->addComponent<SWSpriteRenderer>();
		tcolor rgba( SWMath.randomFloat(), SWMath.randomFloat(), SWMath.randomFloat(), 1 );
		renderer->setColor( rgba );
		rgba.a = 0;
		
		SWHardRef<SWSpriteAnimation> spriteAnim = SWAssets.loadSpriteAnimation( "animations/boom_anim.txt" );
		SWActAnimate* anim = new SWActAnimate( 1, spriteAnim()->getSequenceAt(0) );
		SWActBunch* bunch = new SWActBunch();
		bunch->addAct( anim );
		bunch->addAct( new SWActColorTo(0.5f,rgba) );
		bunch->addAct( new SWActScaleTo(0.5f,tvec3::one ) );

		SWAction* action = go->addComponent<SWAction>();
		action->setAct( "fire", bunch );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setParent( getComponent<SWTransform>() );
	}

	//! twinkle
	{
		SWGameObject* go = new SWGameObject;
		go->setName( "Twinkle" );

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
		action->setAct( "fire", bunch );

		SWTransform* trans = go->getComponent<SWTransform>();
		trans->setParent( getComponent<SWTransform>() );
		trans->setLocalScale( tvec3::zero );
	}

	goToPool();
}

void FireWork::onFixedRateUpdate()
{

}

void FireWork::fire()
{
	{
		SWGameObject* worksGO = SW_GC.getScene()->findGO( "FireWorks" );
		if ( worksGO == NULL ) worksGO = new SWGameObject( "FireWorks" );

		gameObject()->setActive( true );

		SWAction* action = gameObject()->addComponent<SWAction>();
		action->play( "fire" );

		SWTransform* trans = getComponent<SWTransform>();
		float radian = SWMath.angleToRadian( SWMath.randomInt(0,360) );
		trans->setLocalRotate( tvec3( 0, 0, radian ) );
		trans->setParent( worksGO->getComponent<SWTransform>() );
	}

	//! boom cloud
	{
		SWTransform* trans = getComponent<SWTransform>()->find( "Boom" );
		trans->setLocalScale( tvec3::zero );

		SWSpriteRenderer* renderer = trans->getComponent<SWSpriteRenderer>();
		tcolor color = renderer->getColor();
		color.a = 1;
		renderer->setColor( color );

		SWAction* action = trans->getComponent<SWAction>();
		action->play( "fire" );
	}

	//! twinkle
	{
		SWTransform* trans = getComponent<SWTransform>()->find( "Twinkle" );
		trans->setLocalScale( tvec3::zero );

		SWSpriteRenderer* renderer = trans->getComponent<SWSpriteRenderer>();
		tcolor color = renderer->getColor();
		color.a = 1;
		renderer->setColor( color );

		SWAction* action = trans->getComponent<SWAction>();
		action->play( "fire" );
	}
}

void FireWork::goToPool()
{
	SWGameObject* poolGO = SW_GC.getScene()->findGO( "Pool" );
	if ( poolGO == NULL )
	{
		poolGO = new SWGameObject( "Pool" );
		poolGO->setActive( false );
	}

	gameObject()->setActive( false );
	getComponent<SWTransform>()->setParent( poolGO->getComponent<SWTransform>() );
}
