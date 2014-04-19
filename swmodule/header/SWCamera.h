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
	TMatrix4x4 m_vpMatrix;
	TMatrix4x4 m_invProjMatrix;

	float m_near;
	float m_far;

	tcolor m_clearColor;

public:

	static SWHardRef<SWCamera> mainCamera;

	SWCamera();
	~SWCamera();

	void orthoMode( float width, float height, float near, float far );
	void perspectiveMode( float fov, float aspect, float near, float far );

	tvec3 screenToWorld( const tvec3& screenPt ) const;
	tray  screenToRay( const tvec2& screenPt ) const;

	void setClearColor( const tcolor& color );
	const tcolor& getClearColor() const;

	const TMatrix4x4& getProjMatrix() const;
	const TMatrix4x4& getViewMatrix() const;
	const TMatrix4x4& getVPMatrix() const;
};

#endif // SWCamera_h__