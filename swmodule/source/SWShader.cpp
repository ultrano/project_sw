#include "SWShader.h"
#include "SWFileStream.h"
#include "SWOpenGL.h"
#include "SWDefines.h"
#include "SWLog.h"

SWShader::SWShader()
{

}

SWShader::~SWShader()
{
	glDeleteProgram( m_shaderID );
}

int SWShader::getUniformLocation( const thashstr& name ) const
{
	UniformTable::const_iterator itor = m_uniformTable.find( name );
	if ( itor != m_uniformTable.end() ) return itor->second;
	return -1;
}

bool SWShader::getUniformName( int index, tstring& name )
{
	UniformTable::const_iterator itor = m_uniformTable.begin();
	for ( ; itor != m_uniformTable.end() ; ++itor )
	{
		if ( itor->second == index )
		{
			const thashstr& hashName = itor->first;
			name = hashName.str();
			return true;
		}
	}
	return false;
}

tuint SWShader::getShaderID() const
{
	return m_shaderID;
}

tuint SWShader::getUniformCount() const
{
	return m_uniformTable.size();
}

void SWShader::use()
{
	glUseProgram( m_shaderID );

	glDisableVertexAttribArray( SW_Attribute_Position );
	glDisableVertexAttribArray( SW_Attribute_Texture );

	for ( tuint i = 0 ; i < m_attributes.size() ; ++i )
	{
		glEnableVertexAttribArray( m_attributes[i] );
	}
}

SWHardRef<SWShader> SWShader::compileShader( const tstring& source )
{
	tuint shaderID = glLoadProgram( source.c_str() );
	if ( shaderID == 0 )
	{
		SWLog( "compile shader failed" );
		return NULL;
	}
	int bufSize = 0;
	int count = 0;
	tstring name;

	SWHardRef<SWShader> shader = new SWShader();
	shader()->m_shaderID = shaderID;

	//! check uniform
	{
		glGetProgramiv( shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize );
		glGetProgramiv( shaderID, GL_ACTIVE_UNIFORMS, &count );
		name.resize( bufSize );

		for ( int i = 0 ; i < count ; ++i )
		{
			GLint sz = 0;
			GLenum type = GL_NONE;
			GLint len = 0;
			glGetActiveUniform( shaderID, i, bufSize, &len, &sz, &type, &name[0] );
			int index = glGetUniformLocation( shaderID, name.c_str() );
			thashstr hashName = name.substr(0,len);
			shader()->m_uniformTable.insert( std::make_pair( hashName, index ) );
		}
	}

	//! check attribute
	{
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize );
		glGetProgramiv( shaderID, GL_ACTIVE_ATTRIBUTES, &count );
		name.resize( bufSize );

		tarray<tstring> attribs;
		for ( int i = 0 ; i < count ; ++i )
		{
			GLint sz = 0;
			GLenum type = GL_NONE;
			GLint len = 0;
			glGetActiveAttrib( shaderID, i, bufSize, &len, &sz, &type, &name[0] );
			attribs.push_back( name.substr(0,len) );
		}

		for ( int i = 0 ; i < count ; ++i )
		{
			int index = -1;
			const tstring attribName = attribs[i];
			if ( attribName == "a_pos" ) index = SW_Attribute_Position;
			else if ( attribName == "a_tex" ) index = SW_Attribute_Texture;
			else continue;

			if ( index < 0 ) continue;
			glBindAttribLocation( shaderID, index, attribName.c_str() );
			shader()->m_attributes.push_back( (tuint)index );
		}
		glLinkProgram( shaderID );
	}

	return shader();
}
