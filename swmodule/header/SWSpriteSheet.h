#ifndef SWSpriteSheet_h__
#define SWSpriteSheet_h__

#include "SWResource.h"

class SWArray;
class SWSprite;
class SWSpriteSheet : public SWResource
{
	SW_RTTI( SWSpriteSheet, SWResource );

public:

	SWSpriteSheet();
	~SWSpriteSheet();
	
	void addSprite( const SWSprite* sprite );
	void setDelayPerUnit( float delay );

	SWSprite* getSpriteAt( tuint index ) const;
	SWHardRef<SWArray> getSpriteArray() const;
	float getDelayPerUnit() const;

	tuint count() const;

private:

	float m_delayPerUnit;
	SWHardRef<SWArray> m_sprites;

};

#endif // SWSpriteSheet_h__