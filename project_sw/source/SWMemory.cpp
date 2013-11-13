//
//  SWMemory.cpp
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWMemory.h"
#include "SWGameContext.h"
#include <memory>
#include "SWLog.h"


int& getMemCount()
{
    static int count = 0;
    return count;
}

void* SWMemory::operator new( size_t size )
{
    ++getMemCount();
    //SWLog( "allocated, maintain: %d", getMemCount() );
    return SW_GC.alloc( size );
}

void*   SWMemory::operator new( size_t size, void* memory )
{
    return memory;
}

void SWMemory::operator delete( void* memory )
{
	SW_GC.free( memory );
    --getMemCount();
    //SWLog( "freed, remains: %d", getMemCount() );
}