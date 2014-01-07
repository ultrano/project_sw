#include "SWMaterial.h"
#include "SWMatrix4x4.h"
#include "SWShader.h"
#include "SWGameContext.h"

void SWMaterial::setShader( const SWShader* shader )
{
	m_shader = shader;
}

SWShader* SWMaterial::getShader() const
{
	return m_shader();
}

void SWMaterial::setFloat( const tstring& name, float val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_floatTable[ index ] = val;
}
void SWMaterial::setVector2( const tstring& name, const SWVector2f& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec2Table[ index ] = val;
}
void SWMaterial::setVector3( const tstring& name, const SWVector3f& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec3Table[ index ] = val;
}
void SWMaterial::setVector4( const tstring& name, const SWQuaternion& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec4Table[ index ] = val;
}
void SWMaterial::setMatrix4x4( const tstring& name, const SWMatrix4x4& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_matTable[ index ] = val;
}
void SWMaterial::setTexture( const tstring& name, tuint texID )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_texTable[ index ] = texID;
}

bool SWMaterial::getFloat( const tstring& name, float& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	val = m_floatTable.at( index );
	return true;
}
bool SWMaterial::getVector2( const tstring& name, SWVector2f& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	val = m_vec2Table.at( index );
	return true;
}
bool SWMaterial::getVector3( const tstring& name, SWVector3f& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	val = m_vec3Table.at( index );
	return true;
}
bool SWMaterial::getVector4( const tstring& name, SWQuaternion& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	val = m_vec4Table.at( index );
	return true;
}
bool SWMaterial::getMatrix4x4( const tstring& name, SWMatrix4x4& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	val = m_matTable.at( index );
	return true;
}
bool SWMaterial::getTexture( const tstring& name, tuint& texID ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	texID = m_texTable.at( index );
	return true;
}

void SWMaterial::apply()
{
	m_shader()->use();
	
	{
		FloatTable::iterator itor = m_floatTable.begin();
		for ( ; itor != m_floatTable.end() ; ++itor ) SW_GC.setShaderFloat( itor->first, itor->second );
	}
	{
		Vec2Table::iterator itor = m_vec2Table.begin();
		for ( ; itor != m_vec2Table.end() ; ++itor )
		{
			SWVector2f& v = itor->second;
			SW_GC.setShaderVector2( itor->first, v.x, v.y );
		}
	}
	{
		Vec3Table::iterator itor = m_vec3Table.begin();
		for ( ; itor != m_vec3Table.end() ; ++itor )
		{
			SWVector3f& v = itor->second;
			SW_GC.setShaderVector3( itor->first, v.x, v.y, v.z );
		}
	}
	{
		Vec4Table::iterator itor = m_vec4Table.begin();
		for ( ; itor != m_vec4Table.end() ; ++itor )
		{
			SWQuaternion& v = itor->second;
			SW_GC.setShaderVector4( itor->first, v.x, v.y, v.z, v.w );
		}
	}
	{
		MatTable::iterator itor = m_matTable.begin();
		for ( ; itor != m_matTable.end() ; ++itor ) SW_GC.setShaderMatrix4x4( itor->first, (float*)&itor->second );
	}
	{
		TexTable::iterator itor = m_texTable.begin();
		for ( ; itor != m_texTable.end() ; ++itor ) SW_GC.setShaderTexture( itor->first, itor->second );
	}
}
