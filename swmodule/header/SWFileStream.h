#ifndef SWFileStream_h__
#define SWFileStream_h__

#include "SWIOStream.h"

class SWFileOutputStream : public SWOutputStream
{
	FILE* m_file;
public:
	SWFileOutputStream();
	SWFileOutputStream( const tstring& file );
	~SWFileOutputStream();
	void write(tbyte* b, tuint len) ;

	bool open( const tstring& file );
	void close();
	tuint size();
	void setPos( tuint pos );
	tuint getPos();
	FILE* getFile();
};

class SWFileInputStream : public SWInputStream
{
	FILE* m_file;
public:
	SWFileInputStream();
	SWFileInputStream( const tstring& file );
	~SWFileInputStream();
	int read(tbyte* b, tuint len) ;

	bool open( const tstring& file );
	void close();
	tuint size();
	void setPos( tuint pos );
	tuint getPos();
	FILE* getFile();
};

#endif // SWFileStream_h__