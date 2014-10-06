#include "SWBroadPhase2D.h"
#include "SWMath.h"
#include <algorithm>

SWBroadPhase2D::SWBroadPhase2D()
{

}

SWBroadPhase2D::~SWBroadPhase2D()
{

}

tuint SWBroadPhase2D::createProxy( const taabb2d& aabb, void* userData )
{
	tuint proxyID = m_tree.createProxy( aabb, userData );
	m_updated.insert( proxyID );
	return proxyID;
}

bool SWBroadPhase2D::updateProxy( tuint proxyID, const taabb2d& aabb )
{
	bool ret = m_tree.updateProxy( proxyID, aabb );
	if ( ret ) m_updated.insert( proxyID );
	return ret;
}

void SWBroadPhase2D::destroyProxy( tuint proxyID )
{
	m_updated.erase( proxyID );
	m_tree.destroyProxy( proxyID );
}

void* SWBroadPhase2D::getUserData( tuint proxyID ) const
{
	return m_tree.getUserData( proxyID );
}

bool SWBroadPhase2D::getFatAABB( tuint proxyID, taabb2d& aabb ) const
{
	return m_tree.getFatAABB( proxyID, aabb );
}

bool SWBroadPhase2D::testOverlap( tuint proxyID1, tuint proxyID2 ) const
{
	taabb2d aabb1, aabb2;
	if ( !m_tree.getFatAABB( proxyID1, aabb1 ) ) return false;
	if ( !m_tree.getFatAABB( proxyID2, aabb2 ) ) return false;
	return aabb1.collide( aabb2 );
}

void SWBroadPhase2D::query( tarray<tuint>& result, const taabb2d& aabb ) const
{
	m_tree.query( result, aabb );
}

void SWBroadPhase2D::queryNewOverlap( OverlapResult& result )
{
	ttable<tuint64,tpair<tuint,tuint>> table;
	
	//! filtering duplicated pairs
	{
		tarray<tuint> temp;
		temp.reserve(16);

		tset<tuint>::iterator itor = m_updated.begin();
		for ( ; itor != m_updated.end() ; ++itor )
		{
			taabb2d fatAABB;
			tuint proxyID = (*itor);
			if ( !getFatAABB( proxyID, fatAABB ) ) continue;

			query( temp, fatAABB );

			for ( tuint i = 0 ; i < temp.size() ; ++i )
			{
				tuint minID = SWMath.min( proxyID, temp[i] );
				tuint maxID = SWMath.max( proxyID, temp[i] );
				if ( minID == maxID ) continue;
				tuint64 key = maxID;
				key = minID | (key << 32);

				if ( table.find( key ) != table.end() ) continue;
				table[key] = std::make_pair( minID, maxID );
			}
		}
		m_updated.clear();
	}

	//! copy filtered pairs to result.
	result.reserve( table.size() );
	ttable<tuint64,tpair<tuint,tuint>>::iterator itor = table.begin();
	for ( ; itor != table.end() ; ++itor )
	{
		result.push_back( itor->second );
	}
}
