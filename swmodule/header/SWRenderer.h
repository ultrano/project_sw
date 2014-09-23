#ifndef SWRenderer_h__
#define SWRenderer_h__

#include "SWComponent.h"
#include "SWList.h"

class SWMesh;
class SWCamera;
class SWMaterial;

class SWRenderer : public SWComponent
{
	SW_RTTI( SWRenderer, SWComponent );

public:

	SWRenderer();
	SWRenderer( factory_constructor );
	~SWRenderer();

	void preRender();
	virtual void render( SWCamera* ) = 0;

	void setMaterial( const SWMaterial* material );
	SWMaterial* getMaterial() const;

	void setMesh( const SWMesh* mesh );
	SWMesh* getMesh() const;

protected:

	void onAwake();
	void onRemove();

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	SWHardRef<SWMesh>   m_mesh;
	SWHardRef<SWMaterial> m_material;
};

#endif // SWRenderer