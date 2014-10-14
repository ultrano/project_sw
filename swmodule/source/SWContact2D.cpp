#include "SWContact2D.h"
#include "SWFixture2D.h"
#include "SWCollider2D.h"
#include "SWShape2D.h"
#include "SWRefNode.h"
#include "SWLog.h"

SWContact2D::SWContact2D()
{
	state.clear();
}

SWContact2D::~SWContact2D()
{

}

bool SWContact2D::evaluate()
{
	SWManifold mf;
	bool ret = testFixture2D( mf, fixture1(), fixture2() );
	if ( ret )
	{
		manifold = mf;

		tmat33 mat;
		SWCollider2D* collider1 = fixture1()->getCollide();
		collider1->computeMatrix2D( mat );
		const SWMassData& md1 = collider1->getMassData();
		cm1 = md1.center * mat;

		SWCollider2D* collider2 = fixture2()->getCollide();
		collider2->computeMatrix2D( mat );
		const SWMassData& md2 = collider2->getMassData();
		cm2 = md2.center * mat;

		//SWLog( "cm1 %f, %f", cm1.x, cm1.y );
		//SWLog( "cm2 %f, %f", cm2.x, cm2.y );
	}
	return ret;
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
