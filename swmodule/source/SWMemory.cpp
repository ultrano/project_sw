//
//  SWMemory.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWMemory.h"
#include <stdlib.h>
#include "SWUtil.h"
#include "SWLog.h"

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

	inline MemRod* addMemRod( unsigned int size )
	{
		unsigned int allocSize = (size > DEFAULT_MAX_CAPACITY)? size : DEFAULT_MAX_CAPACITY;
		MemRod* newRod = MemRod::createRod( allocSize );
		newRod->next = m_fuelRod;
		return (m_fuelRod = newRod);
	}

	inline void* alloc( unsigned int size )
	{
		MemBlock* block = NULL;
		do
		{
			block = m_cursor->request( size );
			if ( block != NULL ) break;
			if ( m_cursor->next == NULL )
			{
				m_cursor = addMemRod( size );
			}
			else
			{
				m_cursor = m_cursor->next;
			}
		} while ( true );

		block->isUsing = true;

		return block->memory();
	}

	inline void dealloc( void* mem )
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

int& getMemCount()
{
    static int count = 0;
    return count;
}

void* SWMemory::operator new( size_t size )
{
    ++getMemCount();
    SWLog( "allocated, maintain: %d", getMemCount() );
    return SWAlloc( size );
}

void*   SWMemory::operator new( size_t size, void* memory )
{
    return memory;
}

void SWMemory::operator delete( void* memory )
{
	SWFree( memory );
    --getMemCount();
    SWLog( "freed, remains: %d", getMemCount() );
}

void* SWAlloc( size_t size )
{
	//return malloc( size );
	return MemCore::instance().alloc( size );
}

void  SWFree( void* mem )
{
	//free( mem );
	MemCore::instance().dealloc( mem );
}