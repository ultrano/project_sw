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
#include "SWProfiler.h"
#include "SWUtil.h"
/*
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
*/
SWLogCenter::SWLogCenter()
{
}

SWLogCenter::~SWLogCenter()
{
}

SWLogCenter& SWLogCenter::getInstance()
{
	static SWLogCenter instance;
	return instance;
}

void SWLogCenter::write( const char* file, unsigned int line, const char* format, ... )
{
    static char buf[128];
    va_list args;
    va_start( args, format );
    vsprintf( &buf[0],format, args );
    va_end( args );

#ifdef ANDROID
    __android_log_print( ANDROID_LOG_INFO, "game log", &buf[0] );
#else
    printf( "%s\n", &buf[0] );
#endif
}

void SWLogCenter::present()
{
}