#ifndef SWFileStream_h__
#define SWFileStream_h__

#include "SWIOStream.h"
#include "SWDefines.h"

class SWFileOutputStream : public SWOutputStream
{
public:
	SWFileOutputStream();
	SWFileOutputStream( const tstring& file, int mode = SW_File_Refresh | SW_File_Binary );
	~SWFileOutputStream();
	
	void write(tbyte* b, tuint len) ;

	bool open( const tstring& file, int mode );
	void close();
	void flush();
	tuint size();
	void setPos( tuint pos );
	tuint getPos();
	FILE* getFile();

private:	
	
	FILE* m_file;

};

class SWFileInputStream : public SWInputStream
{
public:
	SWFileInputStream();
	SWFileInputStream( FILE* file ) : m_file( file ) {};
	SWFileInputStream( const tstring& file );
	~SWFileInputStream();
	
	/* return : read byte count. return -1 if there is no more */
	int read(tbyte* b, tuint len) ;
	int skip( tuint len );
	int available();

	bool open( const tstring& file );
	void close();
	void setPos( tuint pos );
	tuint getPos();
	FILE* getFile();

private:	
	
	FILE* m_file;

};

#endif // SWFileStream_h__
