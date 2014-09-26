#include "SWDynamicTree2D.h"

#define SW_NullNodeID ((tuint)-1)
#define SW_ExpandCount (256)
#define SW_AddedExtension (0.2f)

struct SWTreeNode2D : public SWMemory
{
	taabb2d aabb;
	void* userData;
	tuint childID[2];

	union
	{
		tuint parentID; //! using in dynamic tree
		tuint nextID;   //! using in free list;
	};

	bool isLeaf() { return (childID[1] == SW_NullNodeID); }
};

tuint SWDynamicTree2D::createProxy( const taabb2d& aabb, void* userData )
{
	tuint proxyID = allocNode();
	tvec2 extension( SW_AddedExtension, SW_AddedExtension );

	SWTreeNode2D& node = m_nodes[proxyID];
	node.aabb = aabb;
	node.aabb.lower -= extension;
	node.aabb.upper += extension;
	node.userData = userData;
	
	insertLeaf( proxyID );

	return proxyID;
}

void SWDynamicTree2D::destroyProxy( tuint proxyID )
{
	if ( proxyID >= m_nodes.size() ) return;

	removeLeaf( proxyID );
	freeNode( proxyID );
}

tuint SWDynamicTree2D::allocNode()
{
	if ( m_freeNodeID == SW_NullNodeID )
	{
		tuint index  = m_nodes.size();
		tuint resize = (index + SW_ExpandCount);
		m_nodes.resize( resize );

		m_freeNodeID = index;
		for ( ; index < resize ;++index )
		{
			SWTreeNode2D& node = m_nodes[index];
			node.nextID = (index + 1);
			node.aabb = taabb2d();
			node.userData = NULL;
			node.childID[0] = SW_NullNodeID;
			node.childID[1] = SW_NullNodeID;
		}
		m_nodes[resize-1].nextID = SW_NullNodeID;
	}

	//! finding free node
	{
		tuint proxyID = m_freeNodeID;
		SWTreeNode2D& node = m_nodes[ proxyID ];
		m_freeNodeID = node.nextID;

		return proxyID;
	}

	return SW_NullNodeID;
}

void SWDynamicTree2D::freeNode( tuint nodeID )
{
	if ( nodeID >= m_nodes.size() ) return;

	SWTreeNode2D& node = m_nodes[ nodeID ];
	node.nextID = m_freeNodeID;
	node.aabb = taabb2d();
	node.userData = NULL;
	node.childID[0] = SW_NullNodeID;
	node.childID[1] = SW_NullNodeID;
	m_freeNodeID = nodeID;
}

void SWDynamicTree2D::insertLeaf( tuint nodeID )
{
	tuint targetID = m_rootNodeID;
	if ( targetID == SW_NullNodeID )
	{
		m_rootNodeID = nodeID;
		return;
	}

	SWTreeNode2D& leafNode = m_nodes[nodeID];
	if ( !leafNode.isLeaf() ) return;

	//! finding fittest other leaf node
	bool isLeaf = false;
	do
	{
		SWTreeNode2D& targetNode = m_nodes[targetID];
		if ( isLeaf = targetNode.isLeaf() )
		{
			tuint newParentID = allocNode();
			SWTreeNode2D& newParentNode = m_nodes[newParentID];

			newParentNode.childID[0] = targetID;
			newParentNode.childID[1] = nodeID;

			targetNode.parentID = newParentID;
			leafNode.parentID   = newParentID;

			targetID = newParentID;
		}
		else
		{
			SWTreeNode2D& childNode1 = m_nodes[ targetNode.childID[0] ];
			SWTreeNode2D& childNode2 = m_nodes[ targetNode.childID[1] ];

			float cost1 = taabb2d(childNode1.aabb, leafNode.aabb).getPerimeter() - leafNode.aabb.getPerimeter();
			float cost2 = taabb2d(childNode2.aabb, leafNode.aabb).getPerimeter() - leafNode.aabb.getPerimeter();

			targetID = (cost1 < cost2)? targetNode.childID[0] : targetNode.childID[1];
		}
	} while (isLeaf == false);

	//! update AABBs
	while (targetID != SW_NullNodeID)
	{
		SWTreeNode2D& targetNode = m_nodes[targetID];

		SWTreeNode2D& childNode1 = m_nodes[ targetNode.childID[0] ];
		SWTreeNode2D& childNode2 = m_nodes[ targetNode.childID[1] ];

		targetNode.aabb.combine(childNode1.aabb, childNode2.aabb);

		targetID = targetNode.parentID;
	}
}

void SWDynamicTree2D::removeLeaf( tuint nodeID )
{
	SWTreeNode2D& leafNode = m_nodes[nodeID];
	if ( !leafNode.isLeaf() ) return;

	if ( nodeID == m_rootNodeID )
	{
		m_rootNodeID = SW_NullNodeID;
		return;
	}

	SWTreeNode2D& parent = m_nodes[leafNode.parentID];
	tuint grandParentID = parent.parentID;

	tuint anotherLeafID = (parent.childID[0] == nodeID)? parent.childID[1] : parent.childID[0];
	SWTreeNode2D& anotherLeafNode = m_nodes[ anotherLeafID ];

	freeNode( anotherLeafNode.parentID );
	if ( grandParentID == SW_NullNodeID )
	{
		anotherLeafNode.parentID = SW_NullNodeID;
		m_rootNodeID = anotherLeafID;
		return;
	}

	SWTreeNode2D& grandParentNode = m_nodes[ grandParentID ];

	anotherLeafNode.parentID = grandParentID;
	if ( grandParentNode.childID[0] == anotherLeafNode.parentID )
	{
		grandParentNode.childID[0] = anotherLeafID;
	}
	else
	{
		grandParentNode.childID[1] = anotherLeafID;
	}

	//! update AABBs
	tuint targetID = grandParentID;
	while (targetID != SW_NullNodeID)
	{
		SWTreeNode2D& targetNode = m_nodes[targetID];

		SWTreeNode2D& childNode1 = m_nodes[ targetNode.childID[0] ];
		SWTreeNode2D& childNode2 = m_nodes[ targetNode.childID[1] ];

		targetNode.aabb.combine(childNode1.aabb, childNode2.aabb);

		targetID = targetNode.parentID;
	}
}
