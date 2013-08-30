#include "SWMesh.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshData.h"
#include <memory>

void SWMesh::render()
{
	if ( m_data.isValid() )
	{
		std::vector<float>& vertices  = m_data()->getVertexStream();
		std::vector<float>& texCoords = m_data()->getTexCoordStream();
		std::vector<unsigned short>& indeces = m_data()->getIndexStream();
		SW_GC.setVertexBuffer( &vertices[0] );
		SW_GC.setTexCoordBuffer( &texCoords[0] );
		SW_GC.indexedDraw( indeces.size(), &indeces[0] );
	}
}

void SWMesh::onAdded()
{
	getGameObject()->addComponent<SWMeshRenderer>();
}

void SWMesh::onRemoved()
{

}

void SWMesh::setData( SWMeshData* data )
{
	m_data = data;
}

