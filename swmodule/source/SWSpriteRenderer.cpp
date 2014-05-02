#include "SWSpriteRenderer.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"
#include "SWTexture.h"
#include "SWSprite.h"
#include "SWObjectStream.h"

SWSpriteRenderer::SWSpriteRenderer( factory_constructor )
	: m_material( new SWMaterial() )
{

}

SWSpriteRenderer::~SWSpriteRenderer()
{

}

void SWSpriteRenderer::onAwake()
{
	m_material()->setVector4( "COLOR", tquat(1,1,1,1) );
}

void SWSpriteRenderer::render( SWCamera* camera )
{
	if ( m_material.isValid() )
	{
		SWTransform* transform = gameObject()->getComponent<SWTransform>();
		const TMatrix4x4& model = transform->getWorldMatrix();
		const TMatrix4x4& vpMat = camera->getVPMatrix();
		const tvec2& offset = m_sprite()->getScaledOffset();
		const tvec2& size   = m_sprite()->getScaledSize();

		tquat scaler( size.x, size.y, offset.x, offset.y );
		tquat color( m_color.r, m_color.g, m_color.b, m_color.a );

		m_material()->setMatrix4x4( "MATRIX_MVP", ( model * vpMat ) );
		m_material()->setVector4( "COLOR", color );
		m_material()->setVector4( "SCALER", scaler );
		m_material()->apply();
	}
}
