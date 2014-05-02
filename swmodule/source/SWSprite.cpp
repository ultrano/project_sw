#include "SWSprite.h"
#include "SWTexture.h"

SWSprite::SWSprite( SWTexture* texture, float x, float y, float width, float height )
	: m_texture( texture )
	, m_offset( x, y )
	, m_size( width, height )
{
	float texW = (float)m_texture()->getWidth();
	float texH = (float)m_texture()->getHeight();
	m_scaledOffset.x = x/texW;
	m_scaledOffset.y = y/texH;
	m_scaledSize.x = width/texW;
	m_scaledSize.y = height/texH;
}

SWSprite::~SWSprite()
{

}

SWTexture* SWSprite::getTexture() const
{
	return m_texture();
}

const tvec2& SWSprite::getOffset() const
{
	return m_offset;
}

const tvec2& SWSprite::getSize() const
{
	return m_size;
}

const tvec2& SWSprite::getScaledOffset() const
{
	return m_scaledOffset;
}

const tvec2& SWSprite::getScaledSize() const
{
	return m_scaledSize;
}