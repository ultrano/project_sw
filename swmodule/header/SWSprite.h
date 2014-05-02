#ifndef SWSprite_h__
#define SWSprite_h__

#include "SWObject.h"

class SWTexture;
class SWSprite : public SWObject
{
	SW_RTTI( SWSprite, SWObject );

public:

	SWSprite( SWTexture* texture, float x, float y, float width, float height );
	~SWSprite();

	SWTexture* getTexture() const;
	const tvec2& getOffset() const;
	const tvec2& getSize() const;
	const tvec2& getScaledOffset() const;
	const tvec2& getScaledSize() const;

private:

	SWHardRef<SWTexture> m_texture;
	tvec2 m_offset;
	tvec2 m_size;
	tvec2 m_scaledOffset;
	tvec2 m_scaledSize;

};
#endif // SWSprite_h__