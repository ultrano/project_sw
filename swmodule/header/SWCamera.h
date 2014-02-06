#ifndef SWCamera_h__
#define SWCamera_h__

#include "SWComponent.h"
#include "TMatrix4x4.h"

class SWCamera : public SWComponent
{
	SW_RTTI( SWCamera, SWComponent );
	
	void onAwake();
	void onRemove();

	void onUpdate();
	
	TMatrix4x4 m_viewMatrix;
	TMatrix4x4 m_projMatrix;

public:

	static SWHardRef<SWCamera> mainCamera;

	void orthoMode( float width, float height, float near, float far );
	void perspectiveMode( float fov, float aspect, float near, float far );

	const TMatrix4x4& getProjMatrix() const;
	const TMatrix4x4& getViewMatrix();
};

#endif // SWCamera_h__