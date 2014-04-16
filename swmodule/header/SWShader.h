#ifndef SWShader_h__
#define SWShader_h__

#include "SWObject.h"

class SWShader : public SWObject
{
	SW_RTTI( SWShader, SWObject );

public:

	static SWHardRef<SWShader> compileShader( const tstring& source );

public:

	int  getUniformLocation( const tstring& name ) const;
	bool getUniformName( int index, tstring& name );
	tuint getUniformCount() const;

	tuint getShaderID() const;

	void use();

private:

	SWShader();
	~SWShader();

private:
	friend class SWGameContext;

	typedef ttable<tstring,int> UniformTable;
	typedef tarray<tuint> AttribArray;

	tuint m_shaderID;
	UniformTable m_uniformTable;
	AttribArray m_attributes;

};

#endif // SWShader_h__