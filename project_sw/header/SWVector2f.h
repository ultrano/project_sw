﻿//
//  SWVector2f.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWVector2f_h
#define prototype_SWVector2f_h

#include "SWMemory.h"

// 2D 공간 좌표 Class
class SWVector2f : public SWMemory
{
public:
    
    //! 2D 공간 좌표
    float x, y;
    
    //! 생성자
    SWVector2f();
    SWVector2f( float kx, float ky );
    
    //! 벡터 길이
    float length() const;
    
    //! 다른 지점과의 거리
    float distWith( float kx, float ky ) const;
    float distWith( const SWVector2f& pt ) const;
    
    //! 내적 연산
    float dotWith( float kx, float ky ) const;
    float dotWith( const SWVector2f& pt ) const;
    
    //! 단위화 값 (길이가 0인 벡터에 대한 단위화는 보장하지 않는다. (사전에 체크))
    SWVector2f    normalize() const;
    
    //! 사칙 & 논리 연산 오버로드.
	SWVector2f    operator +( const SWVector2f& pt ) const;
	SWVector2f    operator -( const SWVector2f& pt ) const;
	SWVector2f    operator -() const;
	SWVector2f    operator *( float scalar ) const;
	SWVector2f    operator /( float scalar ) const;
    
	SWVector2f&   operator +=( const SWVector2f& pt ) ;
	SWVector2f&   operator -=( const SWVector2f& pt ) ;
	bool        operator ==( const SWVector2f& pt ) const;
	bool        operator !=( const SWVector2f& pt ) const;
};

#endif
