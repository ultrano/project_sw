#ifndef SWObjectStream_h__
#define SWObjectStream_h__

#include "SWIOStream.h"

class SWObject;

class SWObjectWriter : public SWRefCountable
{
public:
	
	SWObjectWriter( SWOutputStream* os );
	~SWObjectWriter();
	
	void writeObject( SWObject* obj );

	void writeFloat( float val );
	void writeByte( tbyte val );
	void writeChar( char val );
	void writeUInt( tuint val );
	void writeInt( int val );
	void writeBool( bool val );

	void writeString( const tstring& val );
	void writeVec2( const tvec2& val );
	void writeVec3( const tvec3& val );
	void writeQuat( const tquat& val );
	void writeMat44( const tmat44& val );
	void writeRect( const trect& val );
	void writeColor( const tcolor& val );

private:
	typedef ttable< tuint, SWHardRef<SWObject> > ObjectTable;

	SWHardRef<SWOutputStream> m_os;
	ObjectTable m_doneTable;
};

class SWObjectReader : public SWRefCountable
{
public:

	SWObjectReader( SWInputStream* is );
	~SWObjectReader();

	SWObject* readObject();

	float readFloat();
	tbyte readByte();
	char  readChar();
	tuint readUInt();
	int   readInt();
	bool  readBool();

	void  readString( tstring& val );
	void  readVec2( tvec2& val );
	void  readVec3( tvec3& val );
	void  readQuat( tquat& val );
	void  readMat44( tmat44& val );
	void  readRect( trect& val );
	void  readColor( tcolor& val );

private:
	typedef ttable< tuint, SWHardRef<SWObject> > ObjectTable;

	SWHardRef<SWInputStream> m_is;
	ObjectTable m_doneTable;
};

#endif // SWObjectStream_h__