//
//  SWRtti.h
//  prototype
//
//  Created by 한상운 on 13. 5. 24..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWRtti_h
#define prototype_SWRtti_h

/** @biref RTTI(Real Time Type Info) 를 저장.
 */
class SWRtti
{
public:
    
	const char* name;  //< 타임 이름
	const SWRtti* super; //< 상위 타입 정보
    
	SWRtti(const char* n) : name(n), super(NULL) {}
	SWRtti(const char* n, const SWRtti* s) : name(n), super(s) {}
};

/* @brief RTTI의 시작점인 클레스에 선언 */
#define SW_RTTI_ROOT(typeName) public:\
virtual const SWRtti* queryRtti() const { return getRtti(); }; \
static const SWRtti* getRtti() { static SWRtti rtti(#typeName); return &rtti;} \
typedef typeName __this;\
private:

/* @brief 타입의 이름과 상위 타입 이름을 기제 */

#ifdef _MSC_VER
#define SW_RTTI(typeName,superType) public:\
	virtual const SWRtti* queryRtti() const { return getRtti(); }; \
	static const SWRtti* getRtti() { static SWRtti rtti(#typeName,superType::getRtti()); return &rtti;} \
	typedef typeName __this;\
private:
#else
#define SW_RTTI(typeName,superType) public:\
	virtual const SWRtti* queryRtti() const { return getRtti(); }; \
	static const SWRtti* getRtti() { static SWRtti rtti(#typeName,superType::getRtti()); return &rtti;} \
	typedef typeName __this;\
	typedef superType __super;\
private:
#endif
/**
 @brief static_cast, reinterpret_cast 와 같은 형태로 사용
 static_cast<TypeA>( a );
 reinterpret_cast<TypeB>( b );
 swrtti_cast<TypeC>( c ); << 이런 형태로 사용.
 %%% SWRtti 가 선언 되지 않은 타입은 컴파일에러 혹은 오동작 할수 있음. %%%
 */
template<typename T1, typename T2>
T1* swrtti_cast(T2* cls)
{
	if (!cls) return NULL;
	const SWRtti* rtti = cls->queryRtti();
	while (rtti)
	{
		if (rtti == T1::getRtti()) return (T1*)cls;
		rtti = rtti->super;
	}
	return NULL;
}

#endif

