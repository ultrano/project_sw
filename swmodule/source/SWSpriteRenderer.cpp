#include "SWSpriteRenderer.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"
#include "SWTexture.h"
#include "SWSprite.h"
#include "SWObjectStream.h"
#include "SWAssets.h"
#include "SWMesh.h"

SWSpriteRenderer::SWSpriteRenderer( factory_constructor )
	: m_mesh( new SWMesh() )
{
	SWHardRef<SWShader> shader = SWAssets.loadShader( "system/sprite.shader" );
	m_material = new SWMaterial( shader() );

	TVector2f texCoords[] = { TVector2f(0,0), TVector2f(1,0), TVector2f(0,1), TVector2f(1,1) };
	TIndex3 indices[] = { TIndex3(0,1,2), TIndex3(3,2,1) };

	SWMesh* mesh = m_mesh();
	mesh->resizeVertexStream( 4 );
	mesh->setTexCoordStream( 4, &texCoords[0]);
	mesh->setTriangleStream( 2, &indices[0] );
}

SWSpriteRenderer::~SWSpriteRenderer()
{

}

void SWSpriteRenderer::onAwake()
{
	__super::onAwake();
	m_color = tcolor(1,1,1,1);
}

void SWSpriteRenderer::render( SWCamera* camera )
{
	if ( m_material.isValid() )
	{
		SWTransform* transform = gameObject()->getComponent<SWTransform>();
		const TMatrix4x4& model = transform->getWorldMatrix();
		const TMatrix4x4& VPMat = camera->getVPMatrix();
		const tvec2& offset = m_sprite()->getScaledOffset();
		const tvec2& size   = m_sprite()->getScaledSize();

		tquat scaler( size.x, size.y, offset.x, offset.y );
		tquat color( m_color.r, m_color.g, m_color.b, m_color.a );

		m_material()->setTexture( "TEXTURE_0", m_sprite()->getTexture() );
		m_material()->setMatrix4x4( "MATRIX_MVP", ( model * VPMat ) );
		m_material()->setVector4( "COLOR", color );
		m_material()->setVector4( "SCALER", scaler );
		m_material()->apply();
	}

	if ( m_mesh.isValid() ) m_mesh()->draw();
}

void SWSpriteRenderer::setSprite( const SWSprite* sprite )
{
	tvec2 oldSize = m_sprite.isValid()? m_sprite()->getSize() : tvec2::zero;
	
	m_sprite = sprite;
	if ( !m_sprite.isValid() ) return;

	const tvec2& size = sprite->getSize();
	if ( oldSize == size ) return;

	float halfW = size.x/2.0f;
	float halfH = size.y/2.0f;

	SWMesh* mesh = m_mesh();
	mesh->setVertex( 0, tvec3( -halfW, +halfH, 0 ) );
	mesh->setVertex( 1, tvec3( +halfW, +halfH, 0 ) );
	mesh->setVertex( 2, tvec3( -halfW, -halfH, 0 ) );
	mesh->setVertex( 3, tvec3( +halfW, -halfH, 0 ) );
	mesh->updateMesh();
}

const SWSprite* SWSpriteRenderer::getSprite() const
{
	return m_sprite();
}

void SWSpriteRenderer::setColor( const tcolor& color )
{
	m_color = color;
}

const tcolor& SWSpriteRenderer::getColor() const
{
	return m_color;
}

void SWSpriteRenderer::serialize( SWObjectWriter* ow )
{
	ow->writeColor( m_color );
	ow->writeObject( m_mesh() );
	ow->writeObject( m_sprite() );
	ow->writeObject( m_material() );
}

void SWSpriteRenderer::deserialize( SWObjectReader* or )
{
	or->readColor( m_color );
	m_mesh   = swrtti_cast<SWMesh>( or->readObject() );
	m_sprite = swrtti_cast<SWSprite>( or->readObject() );
	m_material = swrtti_cast<SWMaterial>( or->readObject() );
}
