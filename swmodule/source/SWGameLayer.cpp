#include "SWGameLayer.h"
#include "SWCamera.h"
#include "SWRenderer.h"
#include "SWTransform.h"
#include "SWGameObject.h"
#include "SWLog.h"

#include <algorithm>

struct RendererSorter
{
	tvec3 cameraPos;
	tvec3 lookDir;
	RendererSorter( const tvec3& pos, const tvec3& look ) : cameraPos( pos ), lookDir( look ) {}
	bool operator()( SWRenderer* left, SWRenderer* right )
	{
		SWTransform* lTrans = left->getComponent<SWTransform>();
		SWTransform* rTrans = right->getComponent<SWTransform>();
		tvec3 lPos = lTrans->getPosition();
		tvec3 rPos = rTrans->getPosition();
		float lDepth = lookDir.dot(lPos - cameraPos);
		float rDepth = lookDir.dot(rPos - cameraPos);
		return lDepth < rDepth;
	}
};

SWGameLayer::SWGameLayer()
{

}

SWGameLayer::~SWGameLayer()
{

}

tuint SWGameLayer::addRenderer( SWRenderer* renderer )
{
	if ( renderer == NULL ) return m_rendererTree.nullID;

	tuint proxyID = m_rendererTree.createProxy( taabb3d(), renderer );
	if ( proxyID != m_rendererTree.nullID ) m_renderers.push_back( renderer );
	else SWLog( "failed to create renderer proxy aabb into dynamic tree" );

	update();

	return proxyID;
}

void SWGameLayer::removeRenderer( SWRenderer* renderer )
{
	if ( renderer == NULL ) return;

	tuint proxyID = renderer->getProxyID();
	if ( proxyID == m_rendererTree.nullID ) return;

	m_rendererTree.destroyProxy( proxyID );
	m_renderers.remove( renderer );

	SortedTable::iterator itor = m_sortedTable.begin();
	for ( ; itor != m_sortedTable.end() ; ++itor )
	{
		RendererArray& renderers = itor->second;
		RendererArray::iterator last = std::remove( renderers.begin(), renderers.end(), renderer );
		renderers.erase( last, renderers.end() );
	}
}

void SWGameLayer::addCamera( SWCamera* camera )
{
	if ( camera == NULL ) return;

	tuint proxyID = m_cameraTree.createProxy( taabb3d(), camera );
	if ( proxyID != m_cameraTree.nullID ) m_cameras.push_back( camera );
	else SWLog( "failed to create camera proxy aabb into dynamic tree" );

	m_proxyIDTable[camera] = proxyID;
	update();
}

void SWGameLayer::removeCamera( SWCamera* camera )
{
	if ( camera == NULL ) return;

	ProxyIDTable::iterator itor = m_proxyIDTable.find( camera );
	if ( itor == m_proxyIDTable.end() ) return;

	m_cameraTree.destroyProxy( itor->second );
	m_cameras.remove( camera );
	m_sortedTable.erase( camera );
	m_proxyIDTable.erase( camera );
}

void SWGameLayer::update()
{
	//! are renderers updated
	bool rendererUpdated = false;

	//! updated camera array;
	CameraArray updatedCameras;

	//! check renderer AABB update
	for ( SWObject::List::iterator itor = m_renderers.begin()
		; itor != m_renderers.end() 
		; ++itor )
	{
		SWRenderer* renderer = swrtti_cast<SWRenderer>((*itor)());
		if ( renderer == NULL ) continue;
		if ( !renderer->gameObject()->isActiveInScene() ) continue;

		taabb3d aabb;
		renderer->computeAABB( aabb );
		tuint proxyID = renderer->getProxyID();
		if ( m_rendererTree.updateProxy( proxyID, aabb ) ) rendererUpdated = true;
	}

	//! check camera AABB update
	for ( SWObject::List::iterator itor = m_cameras.begin()
		; itor != m_cameras.end() 
		; ++itor )
	{
		SWCamera* camera = swrtti_cast<SWCamera>((*itor)());
		if ( camera == NULL ) continue;
		if ( !camera->gameObject()->isActiveInScene() ) continue;

		taabb3d aabb;
		camera->computeFrustrumAABB( aabb );
		ProxyIDTable::iterator proxyIDpair = m_proxyIDTable.find( camera );
		if ( m_cameraTree.updateProxy( proxyIDpair->second, aabb ) )
		{
			updatedCameras.push_back( camera );
		}
	}

	//! getting cameras that need to be re-sorted;
	if ( rendererUpdated )
	{
		updatedCameras.clear();
		SWObject::List::iterator itor = m_cameras.begin();
		for ( ; itor != m_cameras.end() ; ++itor )
		{
			SWCamera* camera = swrtti_cast<SWCamera>((*itor)());
			if ( camera == NULL ) continue;
			if ( !camera->gameObject()->isActiveInScene() ) continue;

			updatedCameras.push_back( camera );
		}
	}

	if ( updatedCameras.size() <= 0 ) return;

	SWLog( "layer rendering updated" );
	//! re-sorting renderer by camera and AABB Culling
	tuint count = updatedCameras.size();
	while ( count-- )
	{
		SWCamera* camera = updatedCameras[count];
		SWTransform* trans = camera->getComponent<SWTransform>();

		RendererArray& sortedArray = m_sortedTable[camera];
		sortedArray.clear();

		ProxyIDTable::iterator proxyIDpair = m_proxyIDTable.find( camera );
		tuint proxyID = proxyIDpair->second;
		taabb3d aabb;
		m_cameraTree.getFatAABB( proxyID, aabb );
		m_rendererTree.query( m_aabbResult, aabb );
		for ( tuint i = 0 ; i < m_aabbResult.size() ; ++i)
		{
			SWRenderer* renderer = (SWRenderer*)m_rendererTree.getUserData( m_aabbResult[i] );
			sortedArray.push_back( renderer );
		}

		RendererSorter sorter( trans->getPosition(), camera->getLookDir() );
		std::sort( sortedArray.begin(), sortedArray.end(), sorter );
	}
}

void SWGameLayer::draw( SWCamera* camera )
{
	if ( camera == NULL ) return;
	if ( !camera->gameObject()->isActiveInScene() ) return;

	SortedTable::iterator itor = m_sortedTable.find( camera );
	if ( itor == m_sortedTable.end() ) return;

	RendererArray& sortedArray = itor->second;
	for ( tuint i = 0 ; i < sortedArray.size() ; ++i )
	{
		sortedArray[i]->render( camera );
	}
}
