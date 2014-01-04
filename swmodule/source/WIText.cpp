#include "WIText.h"
#include "SWMesh.h"
#include "SWVector3f.h"
#include "WIFontChar.h"
#include "SWGameObject.h"
#include "SWMeshRenderer.h"
#include "SWMeshFilter.h"
#include "SWGameContext.h"

void WIText::onAwake()
{
	m_fontSize = 40;
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

void WIText::setFontSize( size_t fontSize )
{
	m_fontSize = fontSize;
	m_updateMesh = true;
}

size_t WIText::getFontSize() const
{
	return m_fontSize;
}

void WIText::setText( const twstring& text )
{
	m_text = text;
	m_updateMesh = true;
}

const twstring& WIText::getText() const
{
	return m_text;
}

void WIText::updateMesh()
{
	if ( m_updateMesh == false ) return;
	m_updateMesh = false;

	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->setTexture( m_font()->getFontTexture() );

	tarray<SWVector3f> m_pos;
	tarray<SWVector2f> m_tex;
	tarray<unsigned short> m_indices;

	m_pos.resize( m_text.size() * 4 );
	m_tex.resize( m_text.size() * 4 );
	m_indices.resize( m_text.size() * 3 * 2 );

	int defaultSize = 40;
	float sizeScale = (float)m_fontSize/(float)defaultSize;
	int width = 0;
	int height = 0;
	int spaceWidth = 5;
	SW_GC.getTextureSize( m_font()->getFontTexture(), width, height );
	float startOffsetX = 0;
	for ( int i = 0 ; i < m_text.size() ; ++i )
	{
		int charID = m_text[i];
		if ( charID < 256 && isspace( charID ) != 0 )
		{
			startOffsetX += spaceWidth;
			continue;
		}
		if ( charID == 0xfeff ) continue;
		WIFontChar* ch = m_font()->getChar( charID );
		if ( ch == NULL )
		{
			ch = m_font()->getChar( (int)'?' );
			if ( ch == NULL ) break;
		}

		m_indices[i*6+0] = 0+(i*4);
		m_indices[i*6+1] = 1+(i*4);
		m_indices[i*6+2] = 2+(i*4);
		
		m_indices[i*6+3] = 3+(i*4);
		m_indices[i*6+4] = 2+(i*4);
		m_indices[i*6+5] = 1+(i*4);

		m_pos[(i*4)+0] = sizeScale*SWVector3f( startOffsetX + ch->offsetX, ch->h+ch->offsetY, 0 );
		m_pos[(i*4)+1] = sizeScale*SWVector3f( startOffsetX + ch->w+ch->offsetX, ch->h+ch->offsetY, 0 );
		m_pos[(i*4)+2] = sizeScale*SWVector3f( startOffsetX + ch->offsetX, ch->offsetY, 0 );
		m_pos[(i*4)+3] = sizeScale*SWVector3f( startOffsetX + ch->w+ch->offsetX, ch->offsetY, 0 );
		startOffsetX = startOffsetX + ch->w+ch->offsetX;

		m_tex[(i*4)+0] = SWVector2f( ch->x/(float)width, (ch->y)/(float)height );
		m_tex[(i*4)+1] = SWVector2f( (ch->x + ch->w)/(float)width, (ch->y)/(float)height );
		m_tex[(i*4)+2] = SWVector2f( ch->x/(float)width, (ch->y + ch->h)/(float)height );
		m_tex[(i*4)+3] = SWVector2f( (ch->x + ch->w)/(float)width, (ch->y + ch->h)/(float)height );
	}

	m_mesh()->setVertexStream( m_pos.size(), &m_pos[0] );
	m_mesh()->setTexCoordStream( m_tex.size(), &m_tex[0] );
	m_mesh()->setIndexStream( m_indices.size(), &m_indices[0] );
	m_mesh()->updateMesh();
}