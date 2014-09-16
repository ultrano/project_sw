#ifndef SWIOStream_h__
#define SWIOStream_h__

#include "SWRefCounter.h"
#include "SWType.h"
#include "SWRtti.h"

class SWOutputStream : public SWRefCountable
{
	SW_RTTI_ROOT( SWOutputStream );
public:
	virtual void write(tbyte* b, tuint len) = 0;
	void writeByte(tbyte b) { write(&b,1); };
};

class SWInputStream : public SWRefCountable
{
	SW_RTTI_ROOT( SWInputStream );
public:
	/* 
	   return    : read byte count. 
	   return  0 : if there is no more to read.
	   return -1 : invalid stream.
	*/
	virtual int skip( tuint len ) = 0;
	virtual int available() = 0;
	virtual int read(tbyte* b, tuint len) = 0;
	int readByte() { tbyte b; return ( read(&b,1) > 0 )? (int)b : -1; }
};

class SWOutputStreamWriter : public SWMemory
{
public:

	SWOutputStreamWriter(SWOutputStream* os);;

	void writeBytes(tbyte* buffer, tuint size);
	void writeString(const tstring& str);;
	void wirteLine(const tstring& str);

	template<typename T> void write(const T& t)
	{
		if ( m_os.isValid()) m_os()->write((tbyte*)&t, sizeof(T));
	};

private:
	
	SWWeakRef<SWOutputStream> m_os;

};

class SWInputStreamReader : public SWMemory
{
public:

	SWInputStreamReader(SWInputStream* is);;

	int readBytes( tbyte* buffer, tuint size );
	int readString(tstring& str);
	int readLine( tstring& str );

	template<typename T> int read(T& t)
	{
		if (!m_is.isValid()) return -1;
		return m_is()->read((tbyte*)&t, sizeof(T));
	};

private:
	
	SWWeakRef<SWInputStream> m_is;

};
#endif // SWIOStream_h__