#include "SWContact2D.h"
#include "SWFixture2D.h"
#include "SWCollider2D.h"

SWContact2D::SWContact2D()
{
	state.clear();
}

SWContact2D::~SWContact2D()
{

}

bool SWContact2D::evaluate()
{
	return testFixture2D( fixture1(), fixture2() );
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
