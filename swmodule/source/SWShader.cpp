#include "SWShader.h"
#include "SWGameContext.h"
#include "SWFileStream.h"

SWShader::SWShader()
{

}

SWShader::~SWShader()
{
	SW_GC.releaseShader( this );
}

int SWShader::getUniformLocation( const tstring& name ) const
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
			name = itor->first;
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
	SW_GC.useShader( this );
}

SWHardRef<SWShader> SWShader::loadShader( const tstring& filePath )
{
	SWHardRef<SWFileInputStream> fis = new SWFileInputStream( SW_GC.assetPath( filePath ) );
	tuint bufSize = fis()->size();
	tstring source;
	source.resize( bufSize );
	fis()->read( (tbyte*)&source[0], bufSize );
	
	return SW_GC.compileShader( source );
}
