//
//  SWRtti.h
//  prototype
//
//  Created by SangWoon-Han on 13. 5. 24..
//  Copyright (c) 2013 SangWoon-Han. All rights reserved.
//

#ifndef prototype_SWRtti_h
#define prototype_SWRtti_h

/** @biref save the RTTI(Real Time Type Info).
 */
class SWRtti
{
public:
    
	const char* name;  //< name of type
	const SWRtti* super; //< info of super type
    
	SWRtti(const char* n) : name(n), super(NULL) {}
	SWRtti(const char* n, const SWRtti* s) : name(n), super(s) {}
};

/* @brief declare RTTI start point */
#define SW_RTTI_ROOT(typeName) public:\
virtual const SWRtti* queryRtti() const { return getRtti(); }; \
static const SWRtti* getRtti() { static SWRtti rtti(#typeName); return &rtti;} \
typedef typeName __this;\
private:

/* @brief write type name and super type name */

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
 @brief using like (static_cast, reinterpret_cast) 
 static_cast<TypeA>( a );
 reinterpret_cast<TypeB>( b );
 swrtti_cast<TypeC>( c ); << using like this.
 %%% SWRtti will fail to compile if there isn't. %%%
 */
template<typename T1, typename T2>
inline T1* swrtti_cast(T2* cls)
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

