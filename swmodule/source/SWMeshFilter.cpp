#include "SWMeshFilter.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWObjectStream.h"
#include "SWMesh.h"
#include <memory>

SWMeshFilter::SWMeshFilter()
{

}

SWMeshFilter::SWMeshFilter( factory_constructor )
{

}

SWMeshFilter::~SWMeshFilter()
{

}

void SWMeshFilter::draw()
{
	if ( m_mesh.isValid() ) m_mesh()->draw();
}

void SWMeshFilter::onAwake()
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

void SWMeshFilter::serialize( SWObjectWriter* ow )
{
	ow->writeObject( m_mesh() );
}

void SWMeshFilter::deserialize( SWObjectReader* or )
{
	m_mesh = swrtti_cast<SWMesh>( or->readObject() );
}
