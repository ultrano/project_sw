//
//  SWLog.cpp
//  prototype
//
//  Created by 한상운 on 13. 5. 25..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#include "SWLog.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void SWLog( const char* tag, const char* format, ... )
{
    static char buf1[128];
    static char buf2[128];
    
    va_list args;
    va_start( args, format );
    sprintf( &buf1[0], "[%s] : %s\n", tag, format );
    vsprintf( &buf2[0], &buf1[0], args );
    va_end( args );
    printf( &buf2[0] );
    
}
