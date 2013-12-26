#ifndef SWCamera_h__
#define SWCamera_h__

#include "SWComponent.h"
#include "SWMatrix4x4.h"

class SWCamera : public SWComponent
{
	SW_RTTI( SWCamera, SWComponent );
	
	void onAwake();
	void onRemove();

	void onUpdate();

public:

	static SWHardRef<SWCamera> mainCamera;

	SWMatrix4x4 cameraMatrix;

};

#endif // SWCamera_h__