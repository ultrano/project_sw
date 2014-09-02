#include "SWFontRenderer.h"
#include "SWGameObject.h"
#include "SWTransform.h"
#include "SWMaterial.h"
#include "SWShader.h"
#include "SWCamera.h"
#include "SWTexture.h"
//#include "SWSprite.h"
#include "SWObjectStream.h"
#include "SWAssets.h"
#include "SWMesh.h"
#include "SWFontInfo.h"

SWFontRenderer::SWFontRenderer( factory_constructor )
	: m_mesh( new SWMesh() )
	, m_color( 1,1,1,1 )
	, m_textChanged( false )
{
	SWHardRef<SWShader> shader = SWAssets.loadShader( "system/font.shader" );
	m_material = new SWMaterial( shader() );
}

SWFontRenderer::~SWFontRenderer()
{
}

void SWFontRenderer::setText( const tstring& text )
{
	m_text = text;
	m_textChanged = true;
}

const tstring& SWFontRenderer::getText() const
{
	return m_text;
}

void SWFontRenderer::setFontTexture( SWTexture* texture )
{
	m_texture = texture;
}

SWTexture* SWFontRenderer::getFontTexture() const
{
	return m_texture();
}

void SWFontRenderer::setFontInfo( SWFontInfo* fontInfo )
{
	m_fontInfo = fontInfo;
}

SWFontInfo* SWFontRenderer::getFontInfo() const
{
	return m_fontInfo();
}

void SWFontRenderer::setColor( const tcolor& color )
{
	m_color = color;
}

const tcolor& SWFontRenderer::getColor() const
{
	return m_color;
}

void SWFontRenderer::onAwake()
{
	__super::onAwake();
}

void SWFontRenderer::render( SWCamera* camera )
{
	if ( !m_mesh.isValid() ) return;
	if ( !m_material.isValid() ) return;
	if ( !m_texture.isValid() ) return;
	if ( m_text.size() == 0 ) return;
	if ( m_textChanged )
	{
		m_textChanged = false;
		updateMesh();
	}

	SWTransform* transform = gameObject()->getComponent<SWTransform>();
	const TMatrix4x4& model = transform->getWorldMatrix();
	const TMatrix4x4& VPMat = camera->getVPMatrix();

	tquat color( m_color.r, m_color.g, m_color.b, m_color.a );

	m_material()->setTexture( "TEXTURE_0", m_texture() );
	m_material()->setMatrix4x4( "MATRIX_MVP", ( model * VPMat ) );
	m_material()->setVector4( "COLOR", color );
	m_material()->apply();
	m_mesh()->draw();
}

void SWFontRenderer::updateMesh()
{
	if ( !m_mesh.isValid() ) return;
	if ( !m_fontInfo.isValid() ) return;

	SWMesh* mesh = m_mesh();
	mesh->resizeVertexStream( m_text.size() * 4 );
	mesh->resizeTexCoordStream( m_text.size() * 4 );
	mesh->resizeTriangleStream( m_text.size() * 2 );

	tuint32 lastID = 0;
	tvec2 cursor(0,0);
	tvec2 scale( m_fontInfo()->getScaleW(), m_fontInfo()->getScaleH() );
	for ( tuint i = 0 ; i < m_text.size() ; ++i )
	{
		tuint32 id = m_text[i];
		SWFontInfo::Char*    fontChar = m_fontInfo()->getChar( id );
		if ( fontChar == NULL ) continue;

		SWFontInfo::Kerning* kerning  = m_fontInfo()->getKerning( lastID, id );
		cursor.x += (kerning != NULL)? kerning->amount : 0;
		
		lastID = id;
		tuint base = i*4;

		float left   = cursor.x + fontChar->xoffset;
		float top    = cursor.y - fontChar->yoffset;
		float right  = left + fontChar->width;
		float bottom = top - fontChar->height;
		cursor.x += fontChar->xadvence;
		
		mesh->setTriangle( (i*2)+0, tindex3( base+0, base+1, base+2 ) );
		mesh->setTriangle( (i*2)+1, tindex3( base+3, base+2, base+1 ) );

		mesh->setVertex( base+0, tvec3(  left, top, 0 ) );
		mesh->setVertex( base+1, tvec3( right, top, 0 ) );
		mesh->setVertex( base+2, tvec3(  left, bottom, 0 ) );
		mesh->setVertex( base+3, tvec3( right, bottom, 0 ) );
		
		mesh->setTexCoord( base+0, tvec2(                     fontChar->x/scale.x,                      fontChar->y/scale.y ) );
		mesh->setTexCoord( base+1, tvec2( (fontChar->x + fontChar->width)/scale.x,                      fontChar->y/scale.y ) );
		mesh->setTexCoord( base+2, tvec2(                     fontChar->x/scale.x, (fontChar->y + fontChar->height)/scale.y ) );
		mesh->setTexCoord( base+3, tvec2( (fontChar->x + fontChar->width)/scale.x, (fontChar->y + fontChar->height)/scale.y ) );
	}
}

void SWFontRenderer::serialize( SWObjectWriter* ow )
{
}

void SWFontRenderer::deserialize( SWObjectReader* or )
{
}