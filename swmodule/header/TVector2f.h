//
//  TVector2f.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_TVector2f_h
#define prototype_TVector2f_h

#include "SWMemory.h"

// 2D 공간 좌표 Class
class TVector2f : public SWMemory
{
public:
	static const TVector2f axisX;
	static const TVector2f axisY;
	static const TVector2f zero;
	static const TVector2f one;
public:
    
    //! 2D 공간 좌표
    float x, y;
    
    //! 생성자
    TVector2f();
    TVector2f( float kx, float ky );
    
    //! length from origin
    float length() const;
    
    //! distance with other
    float dist( float kx, float ky ) const;
    float dist( const TVector2f& pt ) const;
    
    //! dot product
    float dot( float kx, float ky ) const;
    float dot( const TVector2f& pt ) const;

	//! cross product
    float cross( float kx, float ky ) const;
    float cross( const TVector2f& pt ) const;

    //! 단위화 값 (길이가 0인 벡터에 대한 단위화는 보장하지 않는다. (사전에 체크))
    TVector2f normal() const;

	//! multiplies two vectors component-wise
	TVector2f scale( float kx, float ky ) const;
	TVector2f scale( const TVector2f& pt ) const;
    
	//! z-axis rotate
	void rotateZ( float radian );
    
    //! 사칙 & 논리 연산 오버로드.
	TVector2f  operator -() const { return TVector2f( -x, -y ); }
	TVector2f  operator +( const TVector2f& pt ) const { return TVector2f( x + pt.x, y + pt.y ); }
	TVector2f  operator -( const TVector2f& pt ) const {return TVector2f( x - pt.x, y - pt.y ); }
	TVector2f  operator *( float scalar ) const { return TVector2f( x * scalar, y * scalar ); }
	TVector2f  operator /( float scalar ) const { return TVector2f( x / scalar, y / scalar ); }
    
	TVector2f& operator +=( const TVector2f& pt ) { x += pt.x; y += pt.y; return *this; }
	TVector2f& operator -=( const TVector2f& pt ) { x -= pt.x; y -= pt.y; return *this; }
	TVector2f& operator *= ( float f ) { x *= f; y *= f; return *this; }
	TVector2f& operator /= ( float f ) { x /= f; y /= f; return *this; }

	bool        operator ==( const TVector2f& pt ) const { return ( ( x == pt.x ) && ( y == pt.y ) ); }
	bool        operator !=( const TVector2f& pt ) const { return ( ( x != pt.x ) && ( y != pt.y ) ); }

	TVector2f& operator = ( const TVector2f& a ) { x = a.x; y = a.y; return *this; }
	
};

inline TVector2f operator * ( float f, const TVector2f& a ) { return TVector2f( a.x * f, a.y * f ); }

#endif
