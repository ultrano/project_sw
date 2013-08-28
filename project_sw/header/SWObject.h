//
//  SWObject.h
//  protopang
//
//  Created by hahasasa on 13. 6. 9..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWObject_h
#define protopang_SWObject_h


#include "SWRefCounter.h"
#include "SWRtti.h"

//! 상호 작용의 기본 단위이자 객체성을 띄는 클레스의 뿌리.
//! 객체간 상호작용은 SWHandler 라고 지정한 포맷의
//! 메소드 포인터를 이용.
class SWObject : public SWRefCountable
{
	SW_RTTI_ROOT( SWObject );
	unsigned     m_id;        //< 객체의 유일성을 위한 ID
public:
	SWObject();
	~SWObject();
	unsigned getID() const { return m_id; };
};

//! SWHandler는 호출자와 피호출자간, 대화의 창구가 된다.
typedef void (SWObject::*SWHandler)( SWObject* );
#define DELEGATE( method ) (SWDelegate( this, __this::method ))

class SWDelegate : public SWMemory
{
public:
	SWWeakRef<SWObject> m_object;
	SWHandler m_handler;

	SWDelegate( SWObject* object, const SWHandler& handler );
	SWDelegate( const SWDelegate& copy );
	void call(SWObject* object);
	bool isValid();

};

#endif
