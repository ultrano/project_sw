#include "SWMaterial.h"
#include "TMatrix4x4.h"
#include "SWShader.h"
#include "SWTexture.h"
#include "SWOpenGL.h"

SWHardRef<SWShader> SWMaterial::m_defaultShader = NULL;

SWMaterial::SWMaterial()
{
	setShader( m_defaultShader() );
}
SWMaterial::SWMaterial( const SWShader* shader )
{
	setShader( shader );
}
SWMaterial::~SWMaterial()
{
}

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
void SWMaterial::setVector2( const tstring& name, const TVector2f& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec2Table[ index ] = val;
}
void SWMaterial::setVector3( const tstring& name, const TVector3f& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec3Table[ index ] = val;
}
void SWMaterial::setVector4( const tstring& name, const TQuaternion& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_vec4Table[ index ] = val;
}
void SWMaterial::setMatrix4x4( const tstring& name, const TMatrix4x4& val )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_matTable[ index ] = val;
}

void SWMaterial::setTexture( const tstring& name, SWTexture* texture )
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return;
	m_texTable[ index ] = texture;
}

bool SWMaterial::getFloat( const tstring& name, float& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;
	
	FloatTable::const_iterator itor = m_floatTable.find( index );
	val = itor->second;
	return true;
}
bool SWMaterial::getVector2( const tstring& name, TVector2f& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;

	Vec2Table::const_iterator itor = m_vec2Table.find( index );
	val = itor->second;
	return true;
}
bool SWMaterial::getVector3( const tstring& name, TVector3f& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;

	Vec3Table::const_iterator itor = m_vec3Table.find( index );
	val = itor->second;
	return true;
}
bool SWMaterial::getVector4( const tstring& name, TQuaternion& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;

	Vec4Table::const_iterator itor = m_vec4Table.find( index );
	val = itor->second;
	return true;
}
bool SWMaterial::getMatrix4x4( const tstring& name, TMatrix4x4& val ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return false;

	MatTable::const_iterator itor = m_matTable.find( index );
	val = itor->second;
	return true;
}

SWTexture* SWMaterial::getTexture( const tstring& name ) const
{
	int index = m_shader()->getUniformLocation( name );
	if ( index < 0 ) return NULL;

	TexTable::const_iterator itor = m_texTable.find( index );
	return itor->second();
}

void SWMaterial::apply()
{
	m_shader()->use();
	
	//! float
	{
		FloatTable::iterator itor = m_floatTable.begin();
		for ( ; itor != m_floatTable.end() ; ++itor )
		{
			glUniform1f( itor->first, itor->second );
		}
	}

	//! vector2
	{
		Vec2Table::iterator itor = m_vec2Table.begin();
		for ( ; itor != m_vec2Table.end() ; ++itor )
		{
			TVector2f& v = itor->second;
			glUniform2f( itor->first, v.x, v.y );
		}
	}

	//! vector3
	{
		Vec3Table::iterator itor = m_vec3Table.begin();
		for ( ; itor != m_vec3Table.end() ; ++itor )
		{
			TVector3f& v = itor->second;
			glUniform3f( itor->first, v.x, v.y, v.z );
		}
	}

	//! vector4
	{
		Vec4Table::iterator itor = m_vec4Table.begin();
		for ( ; itor != m_vec4Table.end() ; ++itor )
		{
			TQuaternion& v = itor->second;
			glUniform4f( itor->first, v.x, v.y, v.z, v.w );
		}
	}

	//! matrix4x4
	{
		MatTable::iterator itor = m_matTable.begin();
		for ( ; itor != m_matTable.end() ; ++itor )
		{
			glUniformMatrix4fv( itor->first, 1, GL_FALSE, (float*)&itor->second );
		}
	}

	//! texture
	{
		TexTable::iterator itor = m_texTable.begin();
		for ( ; itor != m_texTable.end() ; ++itor )
		{
			SWTexture* texture = itor->second();
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, texture->getTextureID() );
			glUniform1i( itor->first, 0 );
		}
	}
}

void SWMaterial::setDefaultShader( SWShader* shader )
{
	m_defaultShader = shader;
}
