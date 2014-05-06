#ifndef SWSpriteSheet_h__
#define SWSpriteSheet_h__

#include "SWResource.h"

class SWArray;
class SWSprite;
class SWSpriteSequence : public SWResource
{
	SW_RTTI( SWSpriteSequence, SWResource );

public:

	SWSpriteSequence();
	~SWSpriteSequence();

	void setName( const tstring& name );
	const tstring& getName() const;

	void setDelayPerUnit( float delay );
	float getDelayPerUnit() const;

	void addSprite( const SWSprite* sprite );
	SWSprite* getSpriteAt( tuint index ) const;

	tuint count() const;

private:

	tstring m_name;
	float m_delayPerUnit;
	SWHardRef<SWArray> m_sprites;
	SWObject
};

#endif // SWSpriteSheet_h__