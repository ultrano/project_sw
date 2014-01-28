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
#endif
