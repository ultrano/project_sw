#include "SWSprite.h"
#include "SWTexture.h"

SWSprite::SWSprite()
{

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