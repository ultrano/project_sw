#include "WIText.h"
#include "SWMesh.h"
#include "SWVector3f.h"
#include "WIFontChar.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWGameContext.h"
#include <vector>

void WIText::onAwake()
{
	m_updateMesh = false;
	gameObject()->addComponent<SWMeshRenderer>();
	SWMeshFilter* filter = gameObject()->addComponent<SWMeshFilter>();
	filter->setMesh( new SWMesh );
	m_mesh = filter->getMesh();
	gameObject()->addUpdateDelegate( GetDelegate(updateMesh) );
}

void WIText::onRemove()
{
	gameObject()->removeUpdateDelegate( GetDelegate(updateMesh) );
}

void WIText::setFont( WIFontData* font )
{
	m_font = font;
	m_updateMesh = true;
}

void WIText::setText( const SWString::Value& text )
{
	m_text = text;
	m_updateMesh = true;
}

const SWString::Value& WIText::getText() const
{
	return m_text;
}

void WIText::updateMesh()
{
	if ( m_updateMesh == false ) return;
	m_updateMesh = false;

	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->setTexture( m_font()->getFontTexture() );

	std::vector<SWVector3f> m_pos;
	std::vector<SWVector2f> m_tex;
	std::vector<unsigned short> m_indices;

	m_pos.resize( m_text.size() * 4 );
	m_tex.resize( m_text.size() * 4 );
	m_indices.resize( m_text.size() * 3 * 2 );

	int width = 0;
	int height = 0;
	SW_GC.getTextureSize( m_font()->getFontTexture(), width, height );
	float startOffsetX = 0;
	for ( int i = 0 ; i < m_text.size() ; ++i )
	{
		WIFontChar* ch = m_font()->getChar( (int)m_text[i] );
		if ( ch == NULL ) break;

		m_indices[i*6+0] = 2+(i*4);
		m_indices[i*6+1] = 1+(i*4);
		m_indices[i*6+2] = 0+(i*4);
		
		m_indices[i*6+3] = 3+(i*4);
		m_indices[i*6+4] = 0+(i*4);
		m_indices[i*6+5] = 2+(i*4);

		m_pos[(i*4)+0] = SWVector3f( startOffsetX + ch->offsetX, ch->offsetY, 0 );
		m_pos[(i*4)+1] = SWVector3f( startOffsetX + ch->offsetX, ch->h+ch->offsetY, 0 );
		m_pos[(i*4)+2] = SWVector3f( startOffsetX + ch->w+ch->offsetX, ch->h+ch->offsetY, 0 );
		m_pos[(i*4)+3] = SWVector3f( startOffsetX + ch->w+ch->offsetX, ch->offsetY, 0 );
		startOffsetX = startOffsetX + ch->w+ch->offsetX;

		m_tex[(i*4)+0] = SWVector2f( ch->x/(float)width, ch->y/(float)height );
		m_tex[(i*4)+1] = SWVector2f( ch->x/(float)width, (ch->y + ch->h)/(float)height );
		m_tex[(i*4)+2] = SWVector2f( (ch->x + ch->w)/(float)width, (ch->y + ch->h)/(float)height );
		m_tex[(i*4)+3] = SWVector2f( (ch->x + ch->w)/(float)width, ch->y/(float)height );
	}

	m_mesh()->setVertexStream( m_pos.size(), &m_pos[0] );
	m_mesh()->setTexCoordStream( m_tex.size(), &m_tex[0] );
	m_mesh()->setIndexStream( m_indices.size(), &m_indices[0] );
	m_mesh()->updateMesh();
}