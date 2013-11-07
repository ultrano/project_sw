#include "SWMeshFilter.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMesh.h"
#include <memory>

void SWMeshFilter::draw()
{
	if ( m_mesh.isValid() )
	{
		std::vector<float>& vertices  = m_mesh()->getVertexStream();
		std::vector<float>& texCoords = m_mesh()->getTexCoordStream();
		std::vector<unsigned short>& indeces = m_mesh()->getIndexStream();
		SW_GC.setVertexBuffer( &vertices[0] );
		SW_GC.setTexCoordBuffer( &texCoords[0] );
		SW_GC.indexedDraw( indeces.size(), &indeces[0] );
	}
}

void SWMeshFilter::onAdded()
{
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->setMeshFilter( this );
}

void SWMeshFilter::onRemoved()
{

}

void SWMeshFilter::setMesh( SWMesh* mesh )
{
	m_mesh = mesh;
}

SWMesh* SWMeshFilter::getMesh() const
{
	return m_mesh();
}