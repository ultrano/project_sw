
#ifndef SWMaterial_h__
#define SWMaterial_h__

#include "SWObject.h"

class TVector2f;
class TVector3f;
class TQuaternion;
class TMatrix4x4;
class SWShader;
class SWTexture;

class SWMaterial : public SWObject
{
	SW_RTTI( SWMaterial, SWObject );

public:

	static void setDefaultShader( SWShader* shader );

public:

	SWMaterial();
	SWMaterial( const SWShader* shader );
	SWMaterial( factory_constructor );
	~SWMaterial();

	void setShader( const SWShader* shader );;
	SWShader* getShader() const;;

	void setFloat( const tstring& name, float val );
	void setVector2( const tstring& name, const TVector2f& val );
	void setVector3( const tstring& name, const TVector3f& val );
	void setVector4( const tstring& name, const TQuaternion& val );
	void setMatrix4x4( const tstring& name, const TMatrix4x4& val );
	void setTexture( const tstring& name, SWTexture* texture );
	
	bool getFloat( const tstring& name, float& val ) const;
	bool getVector2( const tstring& name, TVector2f& val ) const;
	bool getVector3( const tstring& name, TVector3f& val ) const;
	bool getVector4( const tstring& name, TQuaternion& val ) const;
	bool getMatrix4x4( const tstring& name, TMatrix4x4& val ) const;
	SWTexture* getTexture( const tstring& name ) const;

	void apply();

protected:

	void serialize( SWObjectWriter* writer );
	void deserialize( SWObjectReader* reader );

private:

	typedef ttable<int,float> FloatTable;
	typedef ttable<int,TVector2f> Vec2Table;
	typedef ttable<int,TVector3f> Vec3Table;
	typedef ttable<int,TQuaternion> Vec4Table;
	typedef ttable<int,TMatrix4x4> MatTable;
	typedef ttable< int, SWHardRef<SWTexture> > TexTable;

	static SWHardRef<SWShader> m_defaultShader;
	SWHardRef<SWShader> m_shader;

	FloatTable m_floatTable;
	Vec2Table  m_vec2Table;
	Vec3Table  m_vec3Table;
	Vec4Table  m_vec4Table;
	MatTable   m_matTable;
	TexTable   m_texTable;
};

#endif // SWMaterial_h__