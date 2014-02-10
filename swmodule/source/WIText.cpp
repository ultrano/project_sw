#include "WIText.h"
#include "SWMesh.h"
#include "TVector3f.h"
#include "TIndex3.h"
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
	if ( m_text.size() == 0 ) return;
	m_updateMesh = false;

	SWMeshRenderer* renderer = gameObject()->addComponent<SWMeshRenderer>();
	renderer->setTexture( m_font()->getFontTexture() );

	tarray<TVector3f, SWAllocator<TVector3f> > m_pos;
	tarray<TVector2f, SWAllocator<TVector2f> > m_tex;
	tarray<TIndex3, SWAllocator<TIndex3> > m_indices;

	m_pos.resize( m_text.size() * 4 );
	m_tex.resize( m_text.size() * 4 );
	m_indices.resize( m_text.size() * 2 );

	int defaultSize = 40;
	float sizeScale = (float)m_fontSize/(float)defaultSize;
	float width  = (float)m_font()->getScaleW();
	float height = (float)m_font()->getScaleH();
	int spaceWidth = 5;
	float lineHeight = m_font()->getLineHeight()*sizeScale;
	float startOffsetX = 0;
	float startOffsetY = 0;
	for ( int i = 0 ; i < m_text.size() ; ++i )
	{
		int charID = m_text[i];
		if ( charID < 256 )
		{
			if ( charID == (int)'\n' || charID == (int)'\r' )
			{
				startOffsetY -= lineHeight;
				startOffsetX = 0;
				continue;
			}
			else if ( isspace( charID ) != 0 )
			{
				startOffsetX += spaceWidth;
				continue;
			}
		}
		if ( charID == 0xfeff ) continue;
		WIFontChar* ch = m_font()->getChar( charID );
		if ( ch == NULL )
		{
			ch = m_font()->getChar( (int)'?' );
			if ( ch == NULL ) break;
		}

		m_indices[i*2+0].index1 = 0+(i*4);
		m_indices[i*2+0].index2 = 1+(i*4);
		m_indices[i*2+0].index3 = 2+(i*4);
		
		m_indices[i*2+1].index1 = 3+(i*4);
		m_indices[i*2+1].index2 = 2+(i*4);
		m_indices[i*2+1].index3 = 1+(i*4);

		float left   = startOffsetX + ch->offsetX;
		float top    = startOffsetY - ch->offsetY;
		float right  = left + ch->w;
		float bottom = top  - ch->h;
		m_pos[(i*4)+0] = sizeScale*TVector3f(  left, top, 0 );
		m_pos[(i*4)+1] = sizeScale*TVector3f( right, top, 0 );
		m_pos[(i*4)+2] = sizeScale*TVector3f(  left, bottom, 0 );
		m_pos[(i*4)+3] = sizeScale*TVector3f( right, bottom, 0 );
		startOffsetX = startOffsetX + ch->advanceX;

		m_tex[(i*4)+0] = TVector2f(           ch->x/width, (ch->y)/height );
		m_tex[(i*4)+1] = TVector2f( (ch->x + ch->w)/width, (ch->y)/height );
		m_tex[(i*4)+2] = TVector2f(           ch->x/width, (ch->y + ch->h)/height );
		m_tex[(i*4)+3] = TVector2f( (ch->x + ch->w)/width, (ch->y + ch->h)/height );
	}

	m_mesh()->setVertexStream( m_pos.size(), &m_pos[0] );
	m_mesh()->setTexCoordStream( m_tex.size(), &m_tex[0] );
	m_mesh()->setTriangleStream( m_indices.size(), &m_indices[0] );
}