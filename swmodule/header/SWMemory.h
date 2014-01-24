//
//  SWMemory.h
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWMemory_h
#define protopang_SWMemory_h

#include <signal.h>
#include <stdio.h>

class SWMemory
{
public:
    
	static	void*	operator new( size_t size );
    static  void*   operator new( size_t size, void* memory );
	static	void	operator delete(void* memory);
    
};
/*

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
struct MemBlock
{

	MemBlock* next;
	bool isUsing;

	bool isTail()
	{
		return ( next == NULL );
	};

	unsigned int capacity()
	{
		if ( isTail() ) return 0;
		unsigned int p = (unsigned int)((char*)next - ((char*)this + sizeof(MemBlock)));
		return p;
	};

	void* memory()
	{
		return (void*)((char*)this + (sizeof(MemBlock)/sizeof(char)));
	}

	bool merge()
	{
		MemBlock* old = next;
		while( next != NULL )
		{
			if ( next->isTail() ) break;
			if ( next->isUsing ) break;
			next = next->next;
		}
		return ( old != next );
	}

	bool divide( unsigned int size )
	{
		if ( capacity() < (size + sizeof(MemBlock)) ) return false;
		
		MemBlock* sep = (MemBlock*)((char*)(memory()) + size);
		sep->isUsing = false;
		sep->next = next;
		next = sep;
		return true;
	}

	MemBlock* request( unsigned int size )
	{
		MemBlock* block = this;
		while( block != NULL )
		{
			if ( block->isTail() ) return NULL;
			if ( false == block->isUsing )
			{
				if ( size <= block->capacity() ) break;
				 block->merge();
				if ( size <= block->capacity() ) break;
			}
			block = block->next;
		}
		
		if ( block->isTail() ) return NULL;

		block->divide( size );

		return block;
	}

	static MemBlock* createBlock( unsigned int size )
	{
		void* chunk = malloc( size );
		
		MemBlock* head = (MemBlock*)chunk;
		MemBlock* tail = (MemBlock*)((char*)(chunk) + size - sizeof(MemBlock));
		
		head->next = tail;
		tail->next = NULL;

		head->isUsing = false;
		tail->isUsing = true;

		return head;
	}
	static MemBlock* digBlock( void* mem )
	{
		return (MemBlock*)((char*)mem - (sizeof(MemBlock)/sizeof(char)));
	}
};
#ifdef _MSC_VER
#pragma pack(pop)
#endif

struct MemRod
{
	unsigned int maxiumCapacity;
	MemBlock     original;
	
	static MemRod* create( unsigned int size )
	{
		if ( size < (sizeof(MemRod) + sizeof(MemBlock)*2) ) return NULL;
		void* chunk = malloc( sizeof(MemRod) + size );
		
		MemRod*   rod  = (MemRod*)chunk;
		MemBlock* head = (MemBlock*)&rod->original;
		MemBlock* tail = (MemBlock*)((char*)(head) + size - sizeof(MemBlock));
		
		head->next = tail;
		tail->next = NULL;

		head->isUsing = false;
		tail->isUsing = true;

		rod->maxiumCapacity = head->capacity();

		return rod;
	}

	MemBlock* request( unsigned int size )
	{
		MemBlock* block = &original;
		while( block != NULL )
		{
			if ( block->isTail() ) return NULL;
			if ( false == block->isUsing )
			{
				if ( size <= block->capacity() ) break;
				 block->merge();
				if ( size <= block->capacity() ) break;
			}
			block = block->next;
		}
		
		if ( block->isTail() ) return NULL;

		block->divide( size );

		return block;
	}

	void* alloc( unsigned int size )
	{
		MemBlock* block = request( size );
		if ( block == NULL ) return NULL;
		block->isUsing = true;
		return block->memory();
	}

	void free( void* mem )
	{
		MemBlock* block = (MemBlock*)((char*)mem - (sizeof(MemBlock)/sizeof(char)));
		block->isUsing = false;
		block->merge();
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
	}
};
*/
#endif
