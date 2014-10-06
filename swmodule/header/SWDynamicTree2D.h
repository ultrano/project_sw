#ifndef SWDynamicTree2D_h__
#define SWDynamicTree2D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWDynamicTree2D : public SWMemory
{
public:

	SWDynamicTree2D();
	~SWDynamicTree2D();

	tuint createProxy( const taabb2d& aabb, void* userData );
	bool  updateProxy( tuint proxyID, const taabb2d& aabb );
	void  destroyProxy(tuint proxyID);

	void* getUserData( tuint proxyID ) const;
	bool  getFatAABB( tuint proxyID, taabb2d& aabb ) const;

	void  query( tarray<tuint>& result, const taabb2d& aabb ) const;

private:

	tuint allocNode();
	void  freeNode(tuint nodeID);

	void insertLeaf(tuint nodeID);
	void removeLeaf(tuint nodeID);

private:

	class TreeNode;
	tarray<TreeNode> m_nodes;
	tuint m_rootID;
	tuint m_freeID;
	static tuint nullID;
};

#endif // SWDynamicTree2D_h__