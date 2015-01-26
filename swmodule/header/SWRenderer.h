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

	tuint getProxyID() const;
	bool computeAABB( taabb3d& aabb ) const;

	tuint getRenderOrder() const;
	void setRenderOrder(tuint order);

protected:

	override void onAwake();
	override void onRemove();

	void onLayerChanged();

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	SWHardRef<SWMesh>   m_mesh;
	SWHardRef<SWMaterial> m_material;
	tuint m_layer;
	tuint m_proxyID;
	tuint m_renderOrder;
};

#endif // SWRenderer