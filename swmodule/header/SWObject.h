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
#include <string>
#include <list>

//! 상호 작용의 기본 단위이자 객체성을 띄는 클레스의 뿌리.
//! 객체간 상호작용은 SWHandler 라고 지정한 포맷의
//! 메소드 포인터를 이용.
class SWObject;
class SWDelegate;
class SWHandler;
typedef std::list< SWHardRef<SWObject> > SWObjectList;
typedef std::string tstring;
typedef double      tnumber;

#define tlist std::list
#define ttable std::map


class SWObject : public SWRefCountable
{
	SW_RTTI_ROOT( SWObject );
	unsigned     m_id;        //< 객체의 유일성을 위한 ID
	SWObjectList m_delegates;
public:

	typedef SWHardRef<SWObject> Ref;
	typedef SWWeakRef<SWObject> WRef;

	SWObject();
	~SWObject();
	unsigned getID() const { return m_id; };
	SWDelegate* getDelegate( const SWHandler& handler );
	virtual std::string toString() const;
	virtual void destroy();
};

#define GetDelegate( method ) ( getDelegate( SWHandler::delimit(&__this::method) ) )

//! SWHandler는 호출자와 피호출자간, 대화의 창구가 된다.
class SWHandler
{
public:
	typedef void (SWObject::*MethodParam)(SWObject*);
	typedef void (SWObject::*MethodVoid)(void);
	MethodParam m_method;
	bool  m_needParam;
	SWHandler( MethodParam method, bool needParam )
		: m_method( method )
		, m_needParam( needParam )
	{
	}

	template<typename T1, typename T2>
	static SWHandler delimit( void (T1::*method)(T2*) )
	{
		return SWHandler( (SWHandler::MethodParam)method, true );
	}

	template<typename T1>
	static SWHandler delimit( void (T1::*method)(void) )
	{
		return SWHandler( (SWHandler::MethodParam)method, false );
	}
};

class SWDelegate : public SWObject
{
	SW_RTTI( SWDelegate, SWObject );
	SWWeakRef<SWObject> m_object;
	SWHandler m_handler;
public:

	SWDelegate( SWObject* object, const SWHandler& handler );
	SWDelegate( const SWDelegate& copy );
	
	void call();
	void call(SWObject* param);

	bool isValid();
	bool isEqual( const SWDelegate* dg );
	bool isEqual( SWObject* object, const SWHandler& handler );
	SWObject* getObject() const { return m_object(); };
	const SWHandler& getHandler() const { return m_handler; };
};
#endif
