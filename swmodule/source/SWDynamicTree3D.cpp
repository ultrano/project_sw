#include "SWDynamicTree3D.h"

#define SW_ExpandCount (256)
#define SW_AddedExtension (1.0f)

struct SWDynamicTree3D::TreeNode : public SWMemory
{
	taabb3d aabb;
	void* userData;
	tuint childID[2];
	union
	{
		tuint parentID; //! using in dynamic tree
		tuint nextID;   //! using in free list;
	};
	bool isLeaf() const { return (childID[1] == nullID); }
};

const tuint SWDynamicTree3D::nullID = ((tuint)-1);

SWDynamicTree3D::SWDynamicTree3D()
	: m_rootID( nullID )
	, m_freeID( nullID )
{

}

SWDynamicTree3D::~SWDynamicTree3D()
{
	m_nodes.clear();
}

tuint SWDynamicTree3D::createProxy( const taabb3d& aabb, void* userData )
{
	tuint proxyID = allocNode();
	tvec3 extension( SW_AddedExtension, SW_AddedExtension, SW_AddedExtension );

	TreeNode& node = m_nodes[proxyID];
	node.aabb = aabb;
	node.aabb.lower -= extension;
	node.aabb.upper += extension;
	node.userData = userData;

	insertLeaf( proxyID );

	return proxyID;
}

bool SWDynamicTree3D::updateProxy( tuint proxyID, const taabb3d& aabb )
{
	if ( proxyID >= m_nodes.size() ) return false;

	TreeNode& node = m_nodes[proxyID];
	if ( node.aabb.contains( aabb ) ) return false;

	removeLeaf( proxyID );

	tvec3 extension( SW_AddedExtension, SW_AddedExtension, SW_AddedExtension );
	node.aabb = aabb;
	node.aabb.lower -= extension;
	node.aabb.upper += extension;

	insertLeaf( proxyID );

	return true;
}

void SWDynamicTree3D::destroyProxy( tuint proxyID )
{
	if ( proxyID >= m_nodes.size() ) return;

	removeLeaf( proxyID );
	freeNode( proxyID );
}

tuint SWDynamicTree3D::allocNode()
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
			node.aabb = taabb3d();
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
		node.parentID = nullID;
		return proxyID;
	}

	return nullID;
}

void SWDynamicTree3D::freeNode( tuint nodeID )
{
	if ( nodeID >= m_nodes.size() ) return;

	TreeNode& node = m_nodes[ nodeID ];
	node.nextID = m_freeID;
	node.aabb = taabb3d();
	node.userData = NULL;
	node.childID[0] = nullID;
	node.childID[1] = nullID;
	m_freeID = nodeID;
}

void SWDynamicTree3D::insertLeaf( tuint nodeID )
{
	if ( nodeID >= m_nodes.size() ) return;

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

			if ( targetNode.parentID != nullID )
			{
				TreeNode& oldParentNode = m_nodes[targetNode.parentID];
				if (oldParentNode.childID[0] == targetID)
				{
					oldParentNode.childID[0] = newParentID;
				}
				else
				{
					oldParentNode.childID[1] = newParentID;
				}
			}

			newParentNode.childID[0] = targetID;
			newParentNode.childID[1] = nodeID;

			newParentNode.parentID = targetNode.parentID;
			targetNode.parentID = newParentID;
			leafNode.parentID   = newParentID;

			if ( targetID == m_rootID ) m_rootID = newParentID;
			targetID = newParentID;
			break;
		}
		else
		{
			TreeNode& targetNode = m_nodes[targetID];
			TreeNode& childNode1 = m_nodes[ targetNode.childID[0] ];
			TreeNode& childNode2 = m_nodes[ targetNode.childID[1] ];

			float cost1 = taabb3d(childNode1.aabb, leafNode.aabb).getPerimeter() - childNode1.aabb.getPerimeter();
			float cost2 = taabb3d(childNode2.aabb, leafNode.aabb).getPerimeter() - childNode2.aabb.getPerimeter();

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

void SWDynamicTree3D::removeLeaf( tuint nodeID )
{
	if ( nodeID >= m_nodes.size() ) return;

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
	if ( grandParentNode.childID[0] == anotherLeafNode.parentID )
	{
		grandParentNode.childID[0] = anotherLeafID;
	}
	else
	{
		grandParentNode.childID[1] = anotherLeafID;
	}
	anotherLeafNode.parentID = grandParentID;

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

void* SWDynamicTree3D::getUserData( tuint proxyID ) const
{
	if ( proxyID >= m_nodes.size() ) return NULL;

	const TreeNode& node = m_nodes[proxyID];
	if ( !node.isLeaf() ) return NULL;

	return node.userData;
}

bool SWDynamicTree3D::getFatAABB( tuint proxyID, taabb3d& aabb ) const
{
	if ( proxyID >= m_nodes.size() ) return false;

	const TreeNode& node = m_nodes[proxyID];
	if ( !node.isLeaf() ) return false;

	aabb = node.aabb;

	return true;
}

void SWDynamicTree3D::query( tarray<tuint>& result, const taabb3d& aabb ) const
{
	result.clear();

	tlist<tuint> suspects;
	suspects.push_back( m_rootID );

	while ( suspects.size() > 0 )
	{
		tuint nodeID = suspects.front();
		suspects.pop_front();

		if ( nodeID == nullID ) continue;

		const TreeNode& node = m_nodes[ nodeID ];
		if ( node.aabb.collide( aabb ) )
		{
			if ( node.isLeaf() )
			{
				result.push_back( nodeID );
			}
			else
			{
				suspects.push_back( node.childID[0] );
				suspects.push_back( node.childID[1] );
			}
		}
	}
}
