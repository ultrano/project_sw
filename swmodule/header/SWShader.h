#ifndef SWShader_h__
#define SWShader_h__

#include "SWObject.h"

class SWShader : public SWObject
{
	SW_RTTI( SWShader, SWObject );
	
	friend class SWGameContext;

	typedef ttable<tstring,int> UniformTable;
	typedef tarray<tuint> AttribArray;

	tuint m_shaderID;
	UniformTable m_uniformTable;
	AttribArray m_attributes;

public:

	SWShader();
	~SWShader();

	int  getUniformLocation( const tstring& name ) const;
	bool getUniformName( int index, tstring& name );
	tuint getUniformCount() const;

	tuint getShaderID() const;

	void use();

	static SWHardRef<SWShader> loadShader( const tstring& filePath );
};

#endif // SWShader_h__