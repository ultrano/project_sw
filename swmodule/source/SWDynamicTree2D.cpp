#include "SWDynamicTree2D.h"

#define SW_ExpandCount (256)
#define SW_AddedExtension (0.2f)

tuint SWDynamicTree2D::nullID = ((tuint)-1);

tuint SWDynamicTree2D::createProxy( const taabb2d& aabb, void* userData )
{
	tuint proxyID = allocNode();
	tvec2 extension( SW_AddedExtension, SW_AddedExtension );

	TreeNode& node = m_nodes[proxyID];
	node.aabb = aabb;
	node.aabb.lower -= extension;
	node.aabb.upper += extension;
	node.userData = userData;
	
	insertLeaf( proxyID );

	return proxyID;
}

void SWDynamicTree2D::updateProxy( tuint proxyID, const taabb2d& aabb )
{
	if ( proxyID >= m_nodes.size() ) return;

	TreeNode& node = m_nodes[proxyID];
	if ( node.aabb.contains( aabb ) ) return;

	removeLeaf( proxyID );

	tvec2 extention( SW_AddedExtension, SW_AddedExtension );
	node.aabb = aabb;
	node.aabb.lower -= extention;
	node.aabb.upper += extention;

	insertLeaf( proxyID );
}

void SWDynamicTree2D::destroyProxy( tuint proxyID )
{
	if ( proxyID >= m_nodes.size() ) return;

	removeLeaf( proxyID );
	freeNode( proxyID );
}

tuint SWDynamicTree2D::allocNode()
{
	if ( m_freeID == nullID )
	{
		tuint index  = m_nodes.size();
		tuint resize = (index + SW_ExpandCount);
		m_nodes.resize( resize );

		m_freeID = index;
		for ( ; index < resize ;++index )
		{
			TreeNode& node = m_nodes[index];
			node.nextID = (index + 1);
			node.aabb = taabb2d();
			node.userData = NULL;
			node.childID[0] = nullID;
			node.childID[1] = nullID;
		}
		m_nodes[resize-1].nextID = nullID;
	}

	//! finding free node
	{
		tuint proxyID = m_freeID;
		TreeNode& node = m_nodes[ proxyID ];
		m_freeID = node.nextID;

		return proxyID;
	}

	return nullID;
}

void SWDynamicTree2D::freeNode( tuint nodeID )
{
	if ( nodeID >= m_nodes.size() ) return;

	TreeNode& node = m_nodes[ nodeID ];
	node.nextID = m_freeID;
	node.aabb = taabb2d();
	node.userData = NULL;
	node.childID[0] = nullID;
	node.childID[1] = nullID;
	m_freeID = nodeID;
}

void SWDynamicTree2D::insertLeaf( tuint nodeID )
{
	tuint targetID = m_rootID;
	if ( targetID == nullID )
	{
		m_rootID = nodeID;
		return;
	}

	TreeNode& leafNode = m_nodes[nodeID];
	if ( !leafNode.isLeaf() ) return;

	//! finding fittest other leaf node
	do
	{
		if ( m_nodes[targetID].isLeaf() )
		{
			tuint newParentID = allocNode();
			TreeNode& targetNode = m_nodes[targetID];
			TreeNode& newParentNode = m_nodes[newParentID];

			newParentNode.childID[0] = targetID;
			newParentNode.childID[1] = nodeID;

			targetNode.parentID = newParentID;
			leafNode.parentID   = newParentID;

			targetID = newParentID;
			break;
		}
		else
		{
			TreeNode& targetNode = m_nodes[targetID];
			TreeNode& childNode1 = m_nodes[ targetNode.childID[0] ];
			TreeNode& childNode2 = m_nodes[ targetNode.childID[1] ];

			float cost1 = taabb2d(childNode1.aabb, leafNode.aabb).getPerimeter() - leafNode.aabb.getPerimeter();
			float cost2 = taabb2d(childNode2.aabb, leafNode.aabb).getPerimeter() - leafNode.aabb.getPerimeter();

			targetID = (cost1 < cost2)? targetNode.childID[0] : targetNode.childID[1];
		}
	} while ( true );

	//! update AABBs
	while (targetID != nullID)
	{
		TreeNode& targetNode = m_nodes[targetID];

		TreeNode& childNode1 = m_nodes[ targetNode.childID[0] ];
		TreeNode& childNode2 = m_nodes[ targetNode.childID[1] ];

		targetNode.aabb.combine(childNode1.aabb, childNode2.aabb);

		targetID = targetNode.parentID;
	}
}

void SWDynamicTree2D::removeLeaf( tuint nodeID )
{
	TreeNode& leafNode = m_nodes[nodeID];
	if ( !leafNode.isLeaf() ) return;

	if ( nodeID == m_rootID )
	{
		m_rootID = nullID;
		return;
	}

	TreeNode& parent = m_nodes[leafNode.parentID];
	tuint grandParentID = parent.parentID;

	tuint anotherLeafID = (parent.childID[0] == nodeID)? parent.childID[1] : parent.childID[0];
	TreeNode& anotherLeafNode = m_nodes[ anotherLeafID ];

	freeNode( anotherLeafNode.parentID );
	if ( grandParentID == nullID )
	{
		anotherLeafNode.parentID = nullID;
		m_rootID = anotherLeafID;
		return;
	}

	TreeNode& grandParentNode = m_nodes[ grandParentID ];

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
	while (targetID != nullID)
	{
		TreeNode& targetNode = m_nodes[targetID];

		TreeNode& childNode1 = m_nodes[ targetNode.childID[0] ];
		TreeNode& childNode2 = m_nodes[ targetNode.childID[1] ];

		targetNode.aabb.combine(childNode1.aabb, childNode2.aabb);

		targetID = targetNode.parentID;
	}
}
