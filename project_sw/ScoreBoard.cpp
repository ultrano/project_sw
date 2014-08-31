#include "ScoreBoard.h"
#include "Rider.h"

ScoreBoard::ScoreBoard()
{

}

ScoreBoard::ScoreBoard( factory_constructor )
{

}

ScoreBoard::~ScoreBoard()
{

}

void ScoreBoard::onAwake()
{
	__super::onAwake();

	gameObject()->setLayerName( "UI" );

	SWGameObject* go = SW_GC.getScene()->findGO( "Rider" );
	m_rider = (Rider*)go->getComponent( "Rider" );

	SWHardRef<SWSpriteAtlas> atlas = SWAssets.loadSpriteAtlas( "flappy_bird.png" );
	SWSpriteRenderer* renderer = gameObject()->addComponent<SWSpriteRenderer>();
	renderer->setSprite( atlas()->find( "bird_0" ) );

}

void ScoreBoard::onFixedRateUpdate()
{

}
