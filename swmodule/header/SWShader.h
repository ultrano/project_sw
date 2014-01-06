#ifndef SWShader_h__
#define SWShader_h__

#include "SWObject.h"

class SWShader : public SWObject
{
	SW_RTTI( SWShader, SWObject );

	typedef ttable<tstring,int> UniformTable;

	tuint m_shaderID;
	UniformTable m_uniformTable;

public:

	void setUniformLocation( const tstring& name, int index );
	int  getUniformLocation( const tstring& name ) const;

	void  setShaderID( tuint shaderID );;
	tuint getShaderID() const;
};

#endif // SWShader_h__