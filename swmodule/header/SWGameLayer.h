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

	void  addCamera( SWCamera* camera );
	void  removeCamera( SWCamera* camera );

	void  update();
	void  draw( SWCamera* camera );

private:

	struct CameraState
	{
		CameraState(){};
		tvec3 lookDir;
	};
	typedef tarray<SWCamera*> CameraArray;
	typedef tarray<SWRenderer*> RendererArray;
	typedef ttable<SWCamera*,RendererArray> SortedTable;
	typedef ttable<SWCamera*,tuint> ProxyIDTable;
	typedef ttable<SWCamera*,CameraState> CameraStateTable;

	SWObject::Array m_renderers;
	SWDynamicTree3D m_rendererTree;

	SWObject::Array m_cameras;
	SWDynamicTree3D m_cameraTree;
	CameraStateTable m_cameraStateTable;

	SortedTable m_sortedTable;
	tarray<tuint> m_aabbResult;

	ProxyIDTable m_proxyIDTable;
};

#endif // SWGameLayer_h__