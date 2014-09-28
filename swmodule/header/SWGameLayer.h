#ifndef SWGameLayer_h__
#define SWGameLayer_h__

#include "SWRefCounter.h"
#include "SWDynamicTree3D.h"
#include "SWObject.h"

class SWRenderer;
class SWCamera;
class SWGameLayer : public SWRefCountable
{
public:

	SWGameLayer();
	~SWGameLayer();

	tuint addRenderer( SWRenderer* renderer );
	void  removeRenderer( SWRenderer* renderer );

	tuint addCamera( SWCamera* camera );
	void  removeCamera( SWCamera* camera );

	void  update();
	void  draw();

private:

	typedef tarray<SWCamera*> CameraArray;
	typedef tarray<SWRenderer*> RendererArray;
	typedef ttable<SWCamera*,RendererArray> SortedTable;

	thashstr m_name;

	SWObject::List m_renderers;
	SWDynamicTree3D m_rendererTree;

	SWObject::List m_cameras;
	SWDynamicTree3D m_cameraTree;

	SortedTable m_sortedTable;
	tarray<tuint> m_aabbResult;
};

#endif // SWGameLayer_h__