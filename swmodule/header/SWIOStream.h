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
	void write(tbyte b) { write(&b,1); };
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
	int read() { tbyte b; return ( read(&b,1) > 0 )? (int)b : -1; }
};

class SWOutputStreamWriter : public SWMemory
{
	SWWeakRef<SWOutputStream> m_os;
public:

	SWOutputStreamWriter(SWOutputStream* os) : m_os( os ) {};

	template<typename T>
	void write(const T& t)
	{
		if ( m_os.isValid()) m_os()->write((tbyte*)&t, sizeof(T));
	};

	void writeString(const tstring& str)
	{
		if (m_os.isValid())
		{
			tstring::size_type sz = str.size();
			write(sz);
			if (sz) m_os()->write((tbyte*)&str[0], sz);
		}
	};

};

class SWInputStreamReader : public SWMemory
{
	SWWeakRef<SWInputStream> m_is;
public:
	SWInputStreamReader(SWInputStream* is) : m_is(is) {};

	template<typename T>
	int read(T& t)
	{
		if (!m_is.isValid()) return -1;
		return m_is()->read((tbyte*)&t, sizeof(T));
	};

	void readString(tstring& str)
	{
		if (m_is.isValid())
		{
			tstring::size_type sz = 0;
			read(sz);
			if (sz)
			{
				str.resize(sz);
				m_is()->read((tbyte*)&str[0], sz);
			}
		}
	};

	bool readLine( tstring& str )
	{
		str.clear();
		char c;
		while( true )
		{
			c = m_is()->read();
			if ( c == '\r' ) continue;
			if ( c <= 0 || c == '\n' ) break;
			str += c;
		}
		return str.size() > 0;
	}

};
#endif // SWIOStream_h__