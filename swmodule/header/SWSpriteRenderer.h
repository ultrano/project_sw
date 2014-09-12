#ifndef SWSpriteRenderer_h__
#define SWSpriteRenderer_h__

#include "SWRenderer.h"

class SWMesh;
class SWSprite;
class SWMaterial;
class SWSpriteRenderer : public SWRenderer
{
	SW_RTTI( SWSpriteRenderer, SWRenderer );

public:

	SWSpriteRenderer( factory_constructor );
	~SWSpriteRenderer();

	void setSprite( const SWSprite* sprite );
	const SWSprite* getSprite() const;

	void setColor( const tcolor& color );
	const tcolor& getColor() const;

	void setMesh( const SWMesh* mesh );
	const SWMesh* getMesh() const;

protected:

	virtual void onAwake();
	virtual void render( SWCamera* camera );
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	SWHardRef<SWMesh>   m_mesh;
	SWHardRef<SWSprite> m_sprite;
	SWHardRef<SWMaterial> m_material;
	tcolor m_color;
};

#endif // SWSpriteRenderer_h__