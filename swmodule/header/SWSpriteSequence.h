#ifndef SWSpriteSequence_h__
#define SWSpriteSequence_h__

#include "SWObject.h"

class SWArray;
class SWSprite;
class SWSpriteSequence : public SWObject
{
	SW_RTTI( SWSpriteSequence, SWObject );

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

protected:
	
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	tstring m_name;
	float m_delayPerUnit;
	SWHardRef<SWArray> m_sprites;
};

#endif // SWSpriteSequence_h__