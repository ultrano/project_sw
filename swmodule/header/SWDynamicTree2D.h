#ifndef SWDynamicTree2D_h__
#define SWDynamicTree2D_h__

#include "SWRefCounter.h"
#include "SWType.h"

class SWDynamicTree2D : public SWMemory
{
public:

	tuint createProxy( const taabb2d& aabb, void* userData );
	void  updateProxy( tuint proxyID, const taabb2d& aabb );
	void  destroyProxy(tuint proxyID);

	template<typename T>
	void  query( T* callBack, const taabb2d& aabb );

private:

	tuint allocNode();
	void  freeNode(tuint nodeID);

	void insertLeaf(tuint nodeID);
	void removeLeaf(tuint nodeID);

private:

	struct TreeNode : public SWMemory
	{
		taabb2d aabb;
		void* userData;
		tuint childID[2];
		union
		{
			tuint parentID; //! using in dynamic tree
			tuint nextID;   //! using in free list;
		};
		bool isLeaf() { return (childID[1] == nullID); }
	};
	tarray<TreeNode> m_nodes;

	static tuint nullID;
	tuint m_rootID;
	tuint m_freeID;
};

template<typename T>
void SWDynamicTree2D::query( T* callBack, const taabb2d& aabb )
{
	tlist suspects;
	suspects.push_back( m_rootID );

	while ( suspects.size() > 0 )
	{
		tuint nodeID = suspects.front();
		suspects.pop_front();

		if ( nodeID == nullID ) continue;

		TreeNode& node = m_nodes[ nodeID ];
		if ( node.aabb.collide( aabb ) )
		{
			if ( node.isLeaf() )
			{
				callBack->queryCallback( nodeID );
			}
			else
			{
				suspects.push_back( node.childID[0] );
				suspects.push_back( node.childID[1] );
			}
		}
	}
}

#endif // SWDynamicTree2D_h__