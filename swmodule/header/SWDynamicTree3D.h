#ifndef SWDynamicTree3D_h__
#define SWDynamicTree3D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWDynamicTree3D : public SWMemory
{
public:

	SWDynamicTree3D();
	~SWDynamicTree3D();

	tuint createProxy( const taabb3d& aabb, void* userData );
	bool  updateProxy( tuint proxyID, const taabb3d& aabb );
	void  destroyProxy(tuint proxyID);

	void* getUserData( tuint proxyID ) const;
	bool  getFatAABB( tuint proxyID, taabb3d& aabb ) const;

	void  query( tarray<tuint>& result, const taabb3d& aabb ) const;

private:

	tuint allocNode();
	void  freeNode(tuint nodeID);

	void insertLeaf(tuint nodeID);
	void removeLeaf(tuint nodeID);

public:

	static const tuint nullID;

private:

	class TreeNode;
	tarray<TreeNode> m_nodes;
	tuint m_rootID;
	tuint m_freeID;
};


#endif // SWDynamicTree3D_h__