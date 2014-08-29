#ifndef BackGround_h__
#define BackGround_h__

#include "SWHeaders.h"
#include "GameValues.h"

class BackGround : public SWBehavior
{
	SW_RTTI( BackGround, SWBehavior );

public:

	BackGround();
	BackGround( factory_constructor );
	~BackGround();

protected:

	virtual void onAwake();
	virtual void onUpdate();

private:

	SWHardRef<SWGameObject> m_camera;
	SWHardRef<SWGameObject> m_images[2];
};

#endif // BackGround_h__