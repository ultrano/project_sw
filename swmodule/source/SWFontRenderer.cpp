#include "SWFontRenderer.h"
#include <stdarg.h>
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
#include "SWDefines.h"

SWFontRenderer::SWFontRenderer( factory_constructor )
	: m_mesh( new SWMesh() )
	, m_color( 1,1,1,1 )
	, m_textChanged( false )
	, m_alignH( SW_Align_Left )
	, m_alignV( SW_Align_Top )
{
	SWHardRef<SWShader> shader = SWAssets.loadShader( "system/font.shader" );
	m_material = new SWMaterial( shader() );
}

SWFontRenderer::~SWFontRenderer()
{
}

void SWFontRenderer::setText( const char* format, ... )
{
	const int size = 256;
    char stack[size];
	char* buffer = &stack[0];

    va_list args;
    va_start( args, format );
    int ret = vsprintf( buffer,format, args );
	if ( ret < 0 )
	{
		tstring heap;
		do
		{
			heap.resize( heap.size() + size );
			ret = vsprintf( &heap[0],format, args );
		} while ( ret < 0 );
		buffer = &heap[0];
	}
    va_end( args );
	
	if ( m_textChanged = (m_text != buffer) )
	{
		m_text = buffer;
	}
}

void SWFontRenderer::setText( const tstring& text )
{
	if ( m_textChanged = (m_text != text) )
	{
		m_text = text;
	}
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

void SWFontRenderer::setAlignH( int LCR )
{
	if ( LCR == SW_Align_Center || LCR == SW_Align_Left || LCR == SW_Align_Right )
	{
		m_alignH = LCR;
		m_textChanged = true;
	}
}

int  SWFontRenderer::getAlignH() const
{
	return m_alignH;
}

void SWFontRenderer::setAlignV( int TCB )
{
	if ( TCB == SW_Align_Center || TCB == SW_Align_Top || TCB == SW_Align_Bottom )
	{
		m_alignV = TCB;
		m_textChanged = true;
	}
}

int  SWFontRenderer::getAlignV() const
{
	return m_alignV;
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

	tuint32 lastID = '\r';
	tvec2 cursor( 0, 0 );
	tvec2 scale( m_fontInfo()->getScaleW(), m_fontInfo()->getScaleH() );

	float lineHeight = getLinesHeight( m_text );
	switch ( m_alignV )
	{
	case SW_Align_Top    : cursor.y = 0; break;
	case SW_Align_Bottom : cursor.y = lineHeight; break;
	case SW_Align_Center : cursor.y = lineHeight/2.0f; break;
	}

	for ( tuint i = 0 ; i < m_text.size() ; ++i )
	{
		tuint32 id = m_text[i];

		if ( lastID == (int)'\n' || lastID == (int)'\r' )
		{
			float lineWidth = (float)getLineWidth( m_text, i );
			switch ( m_alignH )
			{
			case SW_Align_Left   : cursor.x = 0; break;
			case SW_Align_Right  : cursor.x = -lineWidth; break;
			case SW_Align_Center : cursor.x = -lineWidth/2.0f; break;
			}
		}

		if ( id == (int)'\n' || id == (int)'\r' )
		{
			lastID = id;
			cursor.y -= m_fontInfo()->getLineHeight();
			continue;
		}

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

tuint SWFontRenderer::getLineWidth( const tstring& text, tuint offset ) const
{
	tuint length = 0;
	tint32 lastID = 0;
	for ( tuint i = offset ; i < text.size() ; ++i )
	{
		tint32 id = text[i];
		if ( id == (int)'\n' || id == (int)'\r' ) break;

		SWFontInfo::Char* fonstChar = m_fontInfo()->getChar( id );
		SWFontInfo::Kerning* kerning = m_fontInfo()->getKerning( lastID, id );

		length += fonstChar? fonstChar->xadvence : 0;
		length += kerning? kerning->amount : 0;
	}

	return length;
}

tuint SWFontRenderer::getLinesHeight( const tstring& text ) const
{
	tuint lineCount = (text.size() > 0);
	for ( tuint i = 0 ; i < text.size() ; ++i )
	{
		char id = text[i];
		if ( id == '\n' || id == '\r' ) ++lineCount;
	}
	return lineCount * m_fontInfo()->getLineHeight();
}

void SWFontRenderer::serialize( SWObjectWriter* ow )
{
}

void SWFontRenderer::deserialize( SWObjectReader* or )
{
}