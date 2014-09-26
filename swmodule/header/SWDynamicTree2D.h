#ifndef SWDynamicTree2D_h__
#define SWDynamicTree2D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWTreeNode2D;
class SWDynamicTree2D : public SWMemory
{

public:

	tuint createProxy( const taabb2d& aabb, void* userData );
	void destroyProxy(tuint proxyID);

private:

	tuint allocNode();
	void  freeNode(tuint nodeID);

	void insertLeaf(tuint nodeID);
	void removeLeaf(tuint nodeID);

private:

	tarray<SWTreeNode2D> m_nodes;

	tuint m_rootNodeID;
	tuint m_freeNodeID;
};

#endif // SWDynamicTree2D_h__