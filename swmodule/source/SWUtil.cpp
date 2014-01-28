#include "SWUtil.h"
#include "SWIOStream.h"

#include <locale>
#include <codecvt>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

#define MemBlockSize (5)
struct MemBlock
{
	MemBlock* next;
	bool isUsing;

	inline bool isTail()
	{
		return ( next == NULL );
	};

	inline unsigned int capacity()
	{
		if ( isTail() ) return 0;
		unsigned int p = (unsigned int)((char*)next - ((char*)this + MemBlockSize));
		return p;
	};

	inline void* memory()
	{
		return (void*)((char*)this + MemBlockSize);
	}

	inline void merge()
	{
		while( next != NULL )
		{
			if ( next->isTail() ) break;
			if ( next->isUsing ) break;
			next = next->next;
		}
	}

	inline void divide( unsigned int size )
	{
		if ( capacity() <= (size + MemBlockSize) ) return ;

		MemBlock* sep = (MemBlock*)((char*)(memory()) + size);
		sep->isUsing = false;
		sep->next = next;
		next = sep;
	}
};

#define MemRodSize ()
struct MemRod
{
	MemRod* next;
	unsigned int maximumCapacity;
	MemBlock*    cursor;
	MemBlock     original;
	
	static MemRod* createRod( unsigned int size )
	{
		if ( size == 0 ) return NULL;
		unsigned int baseSize = (sizeof(MemRod) + MemBlockSize);
		unsigned int totalSize = baseSize + size;
		void* chunk = malloc( totalSize );
		
		MemRod*   rod  = (MemRod*)chunk;
		MemBlock* head = (MemBlock*)&rod->original;
		MemBlock* tail = (MemBlock*)((char*)(chunk) + totalSize - MemBlockSize);
		
		rod->next = NULL;
		rod->cursor = head;

		head->next = tail;
		tail->next = NULL;

		head->isUsing = false;
		tail->isUsing = true;

		rod->maximumCapacity = head->capacity();

		return rod;
	}

	static void deleteRod( MemRod* rod )
	{
		::free( (void*)rod );
	}

	MemBlock* request( unsigned int size )
	{
		if ( size > maximumCapacity ) return NULL;
		MemBlock* anchor = cursor;
		while( cursor != NULL )
		{
			if ( false == cursor->isUsing )
			{
				cursor->merge();
				if ( size <= cursor->capacity() ) break;
			}
			cursor = cursor->next;
			if ( cursor->isTail() ) cursor = &original;
			if ( cursor <= anchor && anchor < cursor->next ) return NULL;
		}

		cursor->divide( size );

		return cursor;
	}

	void defragment()
	{
		MemBlock* block = &original;
		while( block != NULL )
		{
			if ( block->isTail() ) return ;
			block->merge();
			block = block->next;
		}
		cursor = &original;
	}
};
#ifdef _MSC_VER
#pragma pack(pop)
#endif

class MemCore
{
	enum { DEFAULT_MAX_CAPACITY = 4096 };
	MemRod* m_fuelRod;
	MemRod* m_cursor;

	MemCore()
		: m_fuelRod( MemRod::createRod( DEFAULT_MAX_CAPACITY ) )
		, m_cursor( m_fuelRod )
	{
	};
	~MemCore()
	{
	}

public:

	static MemCore& instance()
	{
		static MemCore core;
		return core;
	}

	void* alloc( unsigned int size )
	{
		MemBlock* block = NULL;
		do
		{
			block = m_cursor->request( size );
			if ( block != NULL ) break;
			if ( m_cursor->next == NULL )
			{
				unsigned int allocSize = (size > DEFAULT_MAX_CAPACITY)? size : DEFAULT_MAX_CAPACITY;
				MemRod* newRod = MemRod::createRod( allocSize );
				newRod->next = m_fuelRod;
				m_cursor = m_fuelRod = newRod;
			}
			else m_cursor = m_cursor->next;
		} while ( true );

		block->isUsing = true;

		return block->memory();
	}

	void free( void* mem )
	{
		MemBlock* block = (MemBlock*)((char*)mem - MemBlockSize);
		block->isUsing = false;
	}
	
	void defragment()
	{
		m_cursor = m_fuelRod;
		while ( m_cursor != NULL )
		{
			m_cursor->defragment();
			m_cursor = m_cursor->next;
		}
		m_cursor = m_fuelRod;
	}
};

__SWUtil::__SWUtil()
{
}

__SWUtil::~__SWUtil()
{
}

__SWUtil& __SWUtil::getInstance()
{
	static __SWUtil instance;
	return instance;
}

unsigned int __SWUtil::getMicroCount()
{
#ifdef WIN32
    return GetTickCount()*1000;
#else
    struct timeval tick;
    gettimeofday(&tick, 0);
    unsigned int sec = tick.tv_sec*1000000;
    unsigned int usec = tick.tv_usec;
    return (sec + usec);
#endif
}

void* __SWUtil::alloc( size_t size )
{
	return MemCore::instance().alloc( size );
	//return malloc( size );
}

void  __SWUtil::free( void* memory )
{
	MemCore::instance().free( memory );
	//::free( memory );
}

tnumber __SWUtil::strToNum( const tstring& str )
{
	float num = 0;
	sscanf( str.c_str(), "%f", &num );
	return num;
}

void __SWUtil::consoleXY( int x, int y )
{
#ifdef WIN32
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#endif
}

twstring __SWUtil::utf8ToUnicode( const tstring& str )
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	const char* begin = &str[0];
	const char* end   = &str[str.size()];

	//! bom check
	if ( str.size() > 3 ) do
	{
		if ( str[0] != (char)0xEF ) break;
		if ( str[1] != (char)0xBB ) break;
		if ( str[2] != (char)0xBF ) break;
		begin = &str[3];
	} while ( false );

	return conv.from_bytes( begin, end );
}

tstring __SWUtil::unicodeToUtf8( const twstring& str )
{
	if ( str.size() == 0 ) return "";
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	const wchar_t* begin = &str[0];
	const wchar_t* end   = &str[str.size()];
	
	//! bom check
	if ( str[0] == (wchar_t)0xfeff ) begin = &str[1];
	else if ( str[0] == (wchar_t)0xfffe ) begin = &str[1];

	return conv.to_bytes( begin, end );

}


void __SWUtil::copyStream( SWOutputStream* os, SWInputStream* is )
{
	if ( !os || !is ) return;

	int sz = is->available();
	if ( sz <= 0 ) return;

	tarray<tbyte> buf;
	buf.resize( sz );
	int ret = is->read( &buf[0], sz );
	if ( ret <= 0 ) return;
	os->write( &buf[0], sz );

}