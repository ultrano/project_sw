#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWShape2D.h"

class TestScene : public SWGameScene
{
	SW_RTTI(TestScene,SWGameScene);
public:
	TestScene() {}
	~TestScene() {}

	override void onAwake()
	{
		SWShape2D::Transform transform1;
		transform1.scale = tvec2(1,1);
		transform1.rotate = SWMath.angleToRadian( 0 );
		transform1.move = tvec2(0,0);

		SWShape2D::Transform transform2;
		transform2.scale = tvec2(1,1);
		transform2.rotate = SWMath.angleToRadian( 45 );
		transform2.move = tvec2(3,3);

		SWHardRef<SWCircle> circle = new SWCircle;
		circle()->set( tvec2::zero, 10 );

		SWHardRef<SWPolygonShape2D> polygon = new SWPolygonShape2D;
		polygon()->setBox(1, 1);
		
		testCollide( polygon(), transform1, polygon(), transform2 );
	}

	override void onFixedRateUpdate()
	{
	}

};

#endif // TestScene_h__