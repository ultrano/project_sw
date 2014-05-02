#ifndef SWSpriteSheet_h__
#define SWSpriteSheet_h__

#include "SWObject.h"

class SWArray;
class SWSprite;
class SWSpriteSheet : public SWObject
{
	SW_RTTI( SWSpriteSheet, SWObject );

public:

	SWSpriteSheet( factory_constructor );
	~SWSpriteSheet();
	
	void addSprite( const SWSprite* sprite );
	void setDelayPerUnit( float delay );

	SWHardRef<SWArray> getSprites() const;
	float getDelayPerUnit() const;

private:

	float m_delayPerUnit;
	SWHardRef<SWArray> m_sprites;

};

#endif // SWSpriteSheet_h__