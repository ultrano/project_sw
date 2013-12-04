#include "SWMeshFilter.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMesh.h"
#include <memory>

void SWMeshFilter::draw()
{
	if ( m_mesh.isValid() ) m_mesh()->draw();
}

void SWMeshFilter::onStart()
{
	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->setMeshFilter( this );
}

void SWMeshFilter::onRemove()
{
	__super::onRemove();
}

void SWMeshFilter::setMesh( SWMesh* mesh )
{
	m_mesh = mesh;
}

SWMesh* SWMeshFilter::getMesh() const
{
	return m_mesh();
}