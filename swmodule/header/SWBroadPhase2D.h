#ifndef SWBroadPhase2D_h__
#define SWBroadPhase2D_h__

#include "SWRefCounter.h"
#include "SWDynamicTree2D.h"

class SWBroadPhase2D : public SWRefCountable
{
public:
	typedef tpair<tuint,tuint>  OverlapPair;
	typedef tarray<OverlapPair> OverlapResult;
public:

	SWBroadPhase2D();
	~SWBroadPhase2D();

	tuint createProxy( const taabb2d& aabb, void* userData );
	bool  updateProxy( tuint proxyID, const taabb2d& aabb );
	void  destroyProxy(tuint proxyID);

	void* getUserData( tuint proxyID ) const;
	bool  getFatAABB( tuint proxyID, taabb2d& aabb ) const;
	bool  testOverlap( tuint proxyID1, tuint proxyID2 ) const;

	void  query( tarray<tuint>& result, const taabb2d& aabb ) const;
	void  queryNewOverlap( OverlapResult& result );

private:
	SWDynamicTree2D m_tree;
	tset<tuint> m_updated;
};

#endif // SWBroadPhase2D_h__