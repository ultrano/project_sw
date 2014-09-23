#include "SWRenderer.h"
#include "SWGameObject.h"
#include "SWGameContext.h"
#include "SWGameScene.h"
#include "SWMesh.h"
#include "SWMaterial.h"
#include "SWObjectStream.h"

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
	scene->m_renderers.push_back( this );
}

void SWRenderer::onRemove()
{
	SWGameScene* scene = SW_GC.getScene();
	scene->m_renderers.remove( this );
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
