#include "SWContact2D.h"
#include "SWFixture2D.h"
#include "SWCollider2D.h"
#include "SWShape2D.h"
#include "SWRefNode.h"

SWContact2D::SWContact2D()
{
	state.clear();
}

SWContact2D::~SWContact2D()
{

}

bool SWContact2D::evaluate()
{
	SWManifold manifold;
	if ( testFixture2D( manifold, fixture1(), fixture2() ) )
	{
		normal = manifold.normal;
		depth  = manifold.depth;
		return true;
	}
	return false;
}

void SWContact2D::update()
{
	bool wasTouching = state.get( eTouching );
	bool isTouching  = evaluate();

	state.set( eTouching, isTouching );
	state.set( eEntering, ( !wasTouching && isTouching) );
	state.set( eStaying, ( wasTouching && isTouching) );
	state.set( eLeaving, ( wasTouching && !isTouching ) );
}

SWContactEdge2D::SWContactEdge2D()
{

}

SWContactEdge2D::~SWContactEdge2D()
{

}
