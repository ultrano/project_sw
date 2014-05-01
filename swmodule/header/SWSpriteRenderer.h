#ifndef SWSpriteRenderer_h__
#define SWSpriteRenderer_h__

#include "SWRenderer.h"

class SWSprite;
class SWMaterial;
class SWSpriteRenderer : public SWRenderer
{
	SW_RTTI( SWSpriteRenderer, SWRenderer );

public:

	SWSpriteRenderer( factory_constructor );
	~SWSpriteRenderer();

protected:

	virtual void onAwake();
	virtual void render( SWCamera* camera );
	void serialize( SWObjectWriter* ow );
	void deserialize( SWObjectReader* or );

private:

	SWHardRef<SWSprite> m_sprite;
	SWHardRef<SWMaterial> m_material;
	tcolor m_color;
};

#endif // SWSpriteRenderer_h__