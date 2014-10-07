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

	bool computeFrustrumAABB( taabb3d& aabb ) const;

	tvec3 screenToWorld( const tvec3& screenPt ) const;
	tray  screenToRay( const tvec2& screenPt ) const;

	void setClearColor( const tcolor& color );
	const tcolor& getClearColor() const;

	void setClearDepth( float depth );
	float getClearDepth() const;

	const TMatrix4x4& getProjMatrix() const;
	const TMatrix4x4& getViewMatrix() const;
	const TMatrix4x4& getVPMatrix() const;

	tflag32 getCullingMask() const;
	void setCullingMask( tflag32 mask );

	int getDepth() const { return m_depth; }
	void setDepth( int depth ) { m_depth = depth; }

	int getClearFlags() const { return m_clearFlags; }
	void setClearFlags( int flags ) { m_clearFlags = flags; }

	const tvec3& getLookDir() const { return m_lookDir; }
	const tvec3& getUpDir() const { return m_upDir; }
	const tvec3& getRightDir() const { return m_rightDir; }

protected:
	
	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	void onAwake();
	void onRemove();
	void onUpdate();

private:

	float m_near;
	float m_far;

	taabb3d m_frustrumAABB;

	tcolor m_clearColor;
	float  m_clearDepth;

	tflag32 m_cullingMask;
	int m_depth;
	int m_clearFlags;

	tvec3 m_lookDir;
	tvec3 m_upDir;
	tvec3 m_rightDir;

	TMatrix4x4 m_viewMatrix;
	TMatrix4x4 m_projMatrix;
	TMatrix4x4 m_vpMatrix;
	TMatrix4x4 m_invProjMatrix;
};

#endif // SWCamera_h__