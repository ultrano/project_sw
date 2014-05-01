#ifndef SWSprite_h__
#define SWSprite_h__

#include "SWObject.h"

class SWTexture;
class SWSprite : public SWObject
{
	SW_RTTI( SWSprite, SWObject );

public:

	SWSprite();
	~SWSprite();

private:

	SWHardRef<SWTexture> m_texture;
	trect m_rect;

};
#endif // SWSprite_h__