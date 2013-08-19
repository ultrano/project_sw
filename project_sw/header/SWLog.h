//
//  SWLog.h
//  prototype
//
//  Created by 한상운 on 13. 5. 25..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef sw_SWLog_h
#define sw_SWLog_h

/**
 @brief CS_knlPrintk 튜닝 로그 함수
 자동 줄넘긴 추가
 로그의 분류를 위한 tag 인자 추가.
 @param tag 로그 분류를 위한 태그
 @param format 로그 내용
 */
void SW_OutputLog( const char* tag, const char* format, ... );

#endif
