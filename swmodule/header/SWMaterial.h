
#ifndef SWMaterial_h__
#define SWMaterial_h__

#include "SWObject.h"

class SWQuaternion;
class SWMatrix4x4;
class SWShader;
class SWMaterial : public SWObject
{
	SW_RTTI( SWMaterial, SWObject );
	
	typedef ttable<tstring,int> IntTable;
	typedef ttable<tstring,float> FloatTable;
	typedef ttable<tstring,SWQuaternion> Vec4Table;
	typedef ttable<tstring,SWMatrix4x4> MatTable;
	typedef ttable<tstring,tuint> TexTable;

	SWHardRef<SWShader> m_shader;

	IntTable   m_intTable;
	FloatTable m_floatTable;
	Vec4Table  m_vec4Table;
	MatTable   m_matTable;
	TexTable   m_texTable;

public:

	void setInt( const tstring& name, int val );
	void setFloat( const tstring& name, float val );
	void setVector4( const tstring& name, const SWQuaternion& val );
	void setMatrix( const tstring& name, const SWMatrix4x4& val );
	void setTexture( const tstring& name, tuint texID );
	
	bool getInt( const tstring& name, int& val );
	bool getFloat( const tstring& name, float& val );
	bool getVector4( const tstring& name, SWQuaternion& val );
	bool getMatrix( const tstring& name, SWMatrix4x4& val );
	bool getTexture( const tstring& name, tuint& texID );

	void apply();
};

#endif // SWMaterial_h__