#include "SWRenderer.h"
#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWMesh.h"
#include "SWMaterial.h"
#include "SWObjectStream.h"
#include "SWTransform.h"

SWRenderer::SWRenderer()
	: m_mesh( new SWMesh() )
	, m_material( new SWMaterial() )
{

}

SWRenderer::SWRenderer( factory_constructor )
	: m_mesh( new SWMesh() )
	, m_material( new SWMaterial() )
{

}

SWRenderer::~SWRenderer()
{

}

void SWRenderer::onAwake()
{
	SWGameScene* scene = SW_GC.getScene();
	m_proxyID = scene->addRenderer( this );
}

void SWRenderer::onRemove()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->removeRenderer( this );
}

void SWRenderer::setMaterial( const SWMaterial* material )
{
	m_material = material;
}

SWMaterial* SWRenderer::getMaterial() const
{
	return m_material();
}

void SWRenderer::setMesh( const SWMesh* mesh )
{
	m_mesh = mesh;
}

SWMesh* SWRenderer::getMesh() const
{
	return m_mesh();
}

void SWRenderer::serialize( SWObjectWriter* writer )
{
	__super::serialize( writer );

	writer->writeObject( m_mesh() );
	writer->writeObject( m_material() );

}

void SWRenderer::deserialize( SWObjectReader* reader )
{
	__super::deserialize( reader );

	m_mesh   = swrtti_cast<SWMesh>( reader->readObject() );
	m_material = swrtti_cast<SWMaterial>( reader->readObject() );
}

tuint SWRenderer::getProxyID() const
{
	return m_proxyID;
}

bool SWRenderer::computeAABB( taabb3d& aabb ) const
{
	if ( !m_mesh.isValid() ) return false;

	SWTransform* trans = getComponent<SWTransform>();
	if ( trans == NULL ) return false;

	const taabb3d& meshAABB = m_mesh()->getAABB();
	const tmat44 worldMat = trans->getWorldMatrix();

	tvec3 point1 = meshAABB.lower * worldMat;
	tvec3 point2 = meshAABB.upper * worldMat;

	aabb.set( point1, point2 );

	return true;
}
