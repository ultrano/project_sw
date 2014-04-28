#ifndef SWCamera_h__
#define SWCamera_h__

#include "SWComponent.h"
#include "TMatrix4x4.h"

class SWCamera : public SWComponent
{
	SW_RTTI( SWCamera, SWComponent );

public:

	SWCamera();
	SWCamera( factory_constructor );
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

	const thashstr& getTargetLayerName() const;
	void setTargetLayerName( const thashstr& name );

	int getDepth() const { return m_depth; }
	void setDepth( int depth ) { m_depth = depth; }

	const tvec3& getLookDir() const { return m_lookDir; }
	const tvec3& getUpDir() const { return m_upDir; }
	const tvec3& getRightDir() const { return m_rightDir; }

private:

	void onAwake();
	void onRemove();
	void onUpdate();

private:

	TMatrix4x4 m_viewMatrix;
	TMatrix4x4 m_projMatrix;
	TMatrix4x4 m_vpMatrix;
	TMatrix4x4 m_invProjMatrix;

	float m_near;
	float m_far;

	tcolor m_clearColor;

	thashstr m_layerName;
	int m_depth;

	tvec3 m_lookDir;
	tvec3 m_upDir;
	tvec3 m_rightDir;
};

#endif // SWCamera_h__