#ifndef TestScene_h__
#define TestScene_h__

#include "SWHeaders.h"
#include "SWDynamicTree2D.h"

class TestScene : public SWGameScene
{
public:
	TestScene() {}
	~TestScene() {}

	override void onAwake()
	{
		SWDynamicTree2D dynamicTree;

		taabb2d aabb;
		aabb.set( tvec2(0,0), tvec2(10,10) );
		dynamicTree.createProxy( aabb, NULL );

		aabb.set( tvec2(11,11), tvec2(20,20) );
		dynamicTree.createProxy( aabb, NULL );

		aabb.set( tvec2(0,0), tvec2(-10,-10) );
		tuint proxyID = dynamicTree.createProxy( aabb, NULL );


		tarray<tuint> result;
		aabb.set( tvec2(-5,-5), tvec2(5,5) );
		dynamicTree.query(result, aabb);

	}

};

#endif // TestScene_h__