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
	/* return : read byte count. return -1 if there is no more */
	virtual int read(tbyte* b, tuint len) = 0;
	virtual tuint available() = 0;
	virtual tuint skip( tuint len ) = 0;
	virtual void reset() = 0;

	int read() { tbyte b; read(&b,1); return (int)b; };
};

class SWOutputStreamWriter
{
	SWOutputStream* m_os;
public:

	SWOutputStreamWriter(SWOutputStream& os) : m_os(&os) {};

	template<typename T>
	void write(const T& t) { if (m_os) m_os->write((tbyte*)&t, sizeof(T)); };

	void writeString(const tstring& str)
	{
		if (m_os)
		{
			tstring::size_type sz = str.size();
			write(sz);
			if (sz) m_os->write((tbyte*)&str[0], sz);
		}
	};

};

class SWInputStreamReader
{
	SWInputStream* m_is;
public:
	SWInputStreamReader(SWInputStream& is) : m_is(&is) {};

	template<typename T>
	int read(T& t) { if (m_is) return m_is->read((tbyte*)&t, sizeof(T)); return -1; };

	void readString(tstring& str)
	{
		if (m_is)
		{
			tstring::size_type sz = 0;
			read(sz);
			if (sz)
			{
				str.resize(sz);
				m_is->read((tbyte*)&str[0], sz);
			}
		}
	};

};
#endif // SWIOStream_h__