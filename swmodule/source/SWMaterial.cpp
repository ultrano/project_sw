#include "SWMaterial.h"
#include "SWMatrix4x4.h"

void SWMaterial::setInt( const tstring& name, int val )
{
	m_intTable.insert( std::make_pair( name, val ) );
}
void SWMaterial::setFloat( const tstring& name, float val )
{
	m_floatTable.insert( std::make_pair( name, val ) );
}
void SWMaterial::setVector4( const tstring& name, const SWQuaternion& val )
{
	m_vec4Table.insert( std::make_pair( name, val ) );
}
void SWMaterial::setMatrix( const tstring& name, const SWMatrix4x4& val )
{
	m_matTable.insert( std::make_pair( name, val ) );
}
void SWMaterial::setTexture( const tstring& name, tuint texID )
{
	m_texTable.insert( std::make_pair( name, texID ) );
}

bool SWMaterial::getInt( const tstring& name, int& val )
{
	IntTable::iterator itor = m_intTable.find( name );
	if ( itor == m_intTable.end() ) return false;
	val = itor->second;
	return true;
}
bool SWMaterial::getFloat( const tstring& name, float& val )
{
	FloatTable::iterator itor = m_floatTable.find( name );
	if ( itor == m_floatTable.end() ) return false;
	val = itor->second;
	return true;
}
bool SWMaterial::getVector4( const tstring& name, SWQuaternion& val )
{
	Vec4Table::iterator itor = m_vec4Table.find( name );
	if ( itor == m_vec4Table.end() ) return false;
	val = itor->second;
	return true;
}
bool SWMaterial::getMatrix( const tstring& name, SWMatrix4x4& val )
{
	MatTable::iterator itor = m_matTable.find( name );
	if ( itor == m_matTable.end() ) return false;
	val = itor->second;
	return true;
}
bool SWMaterial::getTexture( const tstring& name, tuint& texID )
{
	TexTable::iterator itor = m_texTable.find( name );
	if ( itor == m_texTable.end() ) return false;
	texID = itor->second;
	return true;
}