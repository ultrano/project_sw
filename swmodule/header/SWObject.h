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
#include "SWType.h"
#include "SWObjectFactory.h"

//! 상호 작용의 기본 단위이자 객체성을 띄는 클레스의 뿌리.
//! 객체간 상호작용은 SWHandler 라고 지정한 포맷의
//! 메소드 포인터를 이용.
class SWObject;
class SWDelegator;
class SWHandler;
class SWObjectWriter;
class SWObjectReader;

class SWObject : public SWRefCountable
{
	SW_RTTI_ROOT( SWObject );
	SW_REFERENCE( SWObject );
public:

	typedef tlist< Ref, SWAllocator< Ref > > List;
	typedef tarray< Ref, SWAllocator< Ref > > Array;
	typedef tset< Ref, SWAllocator< Ref > > Set;

	typedef tlist< WRef, SWAllocator< WRef > > WList;
	typedef tarray< WRef, SWAllocator< WRef > > WArray;
	typedef tset< Ref, SWAllocator< WRef > > WSet;

public:

	SWObject();
	~SWObject();
	tuint getID() const { return m_id.getID(); };
	SWDelegator* getDelegator( const SWHandler& handler );
	virtual tstring toString() const;
	virtual void destroy();
	virtual SWHardRef<SWObject> clone();

	template<typename T>
	SWHardRef<T> clone()
	{
		SWHardRef<SWObject> obj = clone();
		return swrtti_cast<T>( obj() );
	};

protected:

	friend class SWObjectWriter;
	friend class SWObjectReader;
	virtual void serialize( SWObjectWriter* writer ) {};
	virtual void deserialize( SWObjectReader* reader ) {};

private:
	tobjectid m_id;        //< 객체의 유일성을 위한 ID
	SWObject::List m_delegates;
};

#define GetDelegator( method ) ( getDelegator( SWHandler::delimit(&__this::method) ) )
#define FindDelegator( obj, type_name, method ) ( swrtti_cast<type_name>( obj )? obj->getDelegator( SWHandler::delimit(&type_name::method) ) : NULL )

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

class SWDelegator : public SWObject
{
	SW_RTTI( SWDelegator, SWObject );
	SWObject::WRef m_object;
	SWHandler m_handler;
public:

	SWDelegator( SWObject* object, const SWHandler& handler );
	SWDelegator( const SWDelegator& copy );
	
	void call() const;
	void call(SWObject* param) const;

	bool isValid() const;
	bool isEqual( const SWDelegator* dg ) const;
	bool isEqual( SWObject* object, const SWHandler& handler ) const;
	SWObject* getObject() const { return m_object(); };
	const SWHandler& getHandler() const { return m_handler; };
};

#endif
