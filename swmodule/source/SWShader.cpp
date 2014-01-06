#include "SWShader.h"

void SWShader::setUniformLocation( const tstring& name, int index )
{
	m_uniformTable.insert( std::make_pair( name, index ) );
}

int SWShader::getUniformLocation( const tstring& name ) const
{
	UniformTable::const_iterator itor = m_uniformTable.find( name );
	if ( itor != m_uniformTable.end() ) return itor->second;
	return -1;
}

void SWShader::setShaderID( tuint shaderID )
{
	m_shaderID = shaderID;
}

tuint SWShader::getShaderID() const
{
	return m_shaderID;
}
