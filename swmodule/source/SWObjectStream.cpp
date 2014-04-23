#include "SWObjectStream.h"
#include "SWGameContext.h"
#include "SWObject.h"

SWObjectWriter::SWObjectWriter( SWOutputStream* os )
	: m_os( os )
{

}

SWObjectWriter::~SWObjectWriter()
{

}

void SWObjectWriter::writeObject( SWObject* obj )
{
	tuint objID = obj->getID();
	writeUInt( objID );
	if ( m_doneTable.find( objID ) == m_doneTable.end() )
	{
		writeString( obj->queryRtti()->name );

		m_doneTable.insert( std::make_pair( objID, obj ) );
		obj->serialize( this );
	}
}

void SWObjectWriter::writeFloat( float val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeByte( tbyte val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeChar( char val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeUInt( tuint val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeInt( int val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeString( const tstring& val )
{
	writeUInt( val.size() );
	m_os()->write( (tbyte*)val.c_str(), val.size() );
}

void SWObjectWriter::writeVec2( const tvec2& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeVec3( const tvec3& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeQuat( const tquat& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeMat44( const tmat44& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeRect( const trect& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeBool( bool val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

void SWObjectWriter::writeColor( const tcolor& val )
{
	m_os()->write( (tbyte*)&val, sizeof(val) );
}

//////////////////////////////////////////////////////////////////////////

SWObjectReader::SWObjectReader( SWInputStream* is )
	: m_is( is )
{

}

SWObjectReader::~SWObjectReader()
{

}

SWObject* SWObjectReader::readObject()
{
	tuint objID = readUInt();
	SWHardRef<SWObject> object = NULL;

	ObjectTable::iterator itor = m_doneTable.find( objID );
	if ( itor == m_doneTable.end() )
	{
		tstring typeName;
		readString( typeName );
		object = SW_GC.newInstance( typeName );
		object()->deserialize( this );
		m_doneTable.insert( std::make_pair( objID, object() ) );
	}
	else
	{
		object = itor->second();
	}

	return object();
}

float SWObjectReader::readFloat()
{
	float val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

tbyte SWObjectReader::readByte()
{
	tbyte val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

char SWObjectReader::readChar()
{
	char val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

tuint SWObjectReader::readUInt()
{
	tuint val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

int SWObjectReader::readInt()
{
	int val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

bool SWObjectReader::readBool()
{
	bool val = 0;
	m_is()->read( (tbyte*)&val, sizeof( val ) );
	return val;
}

void SWObjectReader::readString( tstring& val )
{
	val.resize( readUInt() );
	m_is()->read( (tbyte*)val.c_str(), val.size() );
}

void SWObjectReader::readVec2( tvec2& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}

void SWObjectReader::readVec3( tvec3& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}

void SWObjectReader::readQuat( tquat& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}

void SWObjectReader::readMat44( tmat44& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}

void SWObjectReader::readRect( trect& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}

void SWObjectReader::readColor( tcolor& val )
{
	m_is()->read( (tbyte*)&val, sizeof( val ) );
}
