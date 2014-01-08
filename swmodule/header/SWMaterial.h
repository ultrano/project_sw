
#ifndef SWMaterial_h__
#define SWMaterial_h__

#include "SWObject.h"

class SWVector2f;
class SWVector3f;
class SWQuaternion;
class SWMatrix4x4;
class SWShader;
class SWMaterial : public SWObject
{
	SW_RTTI( SWMaterial, SWObject );
	
	typedef ttable<int,float> FloatTable;
	typedef ttable<int,SWVector2f> Vec2Table;
	typedef ttable<int,SWVector3f> Vec3Table;
	typedef ttable<int,SWQuaternion> Vec4Table;
	typedef ttable<int,SWMatrix4x4> MatTable;
	typedef ttable<int,tuint> TexTable;

	SWHardRef<SWShader> m_shader;

	FloatTable m_floatTable;
	Vec2Table  m_vec2Table;
	Vec3Table  m_vec3Table;
	Vec4Table  m_vec4Table;
	MatTable   m_matTable;
	TexTable   m_texTable;

public:

	SWMaterial();
	SWMaterial( const SWShader* shader );
	~SWMaterial();

	void setShader( const SWShader* shader );;
	SWShader* getShader() const;;

	void setFloat( const tstring& name, float val );
	void setVector2( const tstring& name, const SWVector2f& val );
	void setVector3( const tstring& name, const SWVector3f& val );
	void setVector4( const tstring& name, const SWQuaternion& val );
	void setMatrix4x4( const tstring& name, const SWMatrix4x4& val );
	void setTexture( const tstring& name, tuint texID );
	
	bool getFloat( const tstring& name, float& val ) const;
	bool getVector2( const tstring& name, SWVector2f& val ) const;
	bool getVector3( const tstring& name, SWVector3f& val ) const;
	bool getVector4( const tstring& name, SWQuaternion& val ) const;
	bool getMatrix4x4( const tstring& name, SWMatrix4x4& val ) const;
	bool getTexture( const tstring& name, tuint& texID ) const;

	void apply();
};

#endif // SWMaterial_h__