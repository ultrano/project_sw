//
//  SWRefCounter.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWRefCounter_h
#define prototype_SWRefCounter_h

#include "SWMemory.h"
#include <stdio.h>

/**
@brief 참조 카운트에 필요한 상위 클레스
@warning % "SWRefCountable"를 상속한 클레스의 주의사항 %
강제 삭제시, "delete this"같은 방법을 사용하지말고 
"_make_zero_ref"를 호출해 줘야 한다.
그래야 SWRefCountable를 참조하는 참조자가
객체의 생사 여부를 알수 있다.
*/
class SWRefCountable : public SWMemory
{
	friend class SWRefObject;
	template<typename T> friend  class WeakRef;
	template<typename T> friend  class HardRef;

protected:

	SWRefObject* m_ref;
	SWRefCountable();
	virtual ~SWRefCountable();

	/**
	@brief 참조가 없으면 참조자에 의해 호출된다.
	기본 동작은 "delete this;"지만,
	기본 메모리 관리자가 있을경우 오버라이딩해서 변경할수 있다.
	*/
	virtual void _on_zero_ref();

public:

	/**
	@brief 참조자들과의 연결을 끊고 "_on_zero_ref()"를 호출한다.
	*/
	void	_make_zero_ref();

	SWRefObject* getRefObject() const { return m_ref; };
};

#define SW_REFERENCE(name) public: typedef SWHardRef<name> Ref; typedef SWWeakRef<name> WRef; private:


/**
@brief 객체의 참조 카운트를 보관.
*/
class SWRefObject : public SWMemory
{
	friend class SWRefCountable;
	SWRefCountable* m_obj;
	int m_hard;
	int m_weak;
	SWRefObject( SWRefCountable* obj ) : m_obj(obj), m_hard(0), m_weak(0) {}
	~SWRefObject() {}
public:
	inline void incWeak() { ++m_weak; }
	inline void decWeak() { if (--m_weak == 0) delete this; }
	inline SWRefCountable* getObject() const { return m_obj; }
	inline int getHard() const { return m_hard; }
	inline int getWeak() const { return m_weak; }
	inline void incHard() { if ( !m_obj ) return; ++m_hard; }
	inline void decHard()
	{
		if ( !m_obj ) return;
		--m_hard;
		if (m_hard == 0) 
		{
			if ( m_obj )
			{
				m_obj->_make_zero_ref();
				m_obj = NULL;
			}
		}
		else if ( m_hard < 0 ) m_hard = 0;
	}

	bool isUndead() const { return ( getHard() <= 0 && getObject() ); }
	/*
	there is two possibility.
	first. this was never assigned to "reference counter" like SWHardRef or SWWeakRef after created.
	second. this is dying. 
	*/
};

/**
@brief 약참조를 하는 참조자
객체의 생명에는 관여 하지 않는다.
"operator ()"를 통에 객체를 참조 가능.
*/
template<typename T>
class SWWeakRef
{
	SWRefObject* m_ref;
public:
	inline SWWeakRef() : m_ref(NULL) {};
	inline SWWeakRef(const T* obj) : m_ref(NULL) { *this = obj; };
	inline SWWeakRef(const SWWeakRef& ref) : m_ref(NULL) { *this = ref; };
	inline ~SWWeakRef() { *this = NULL; }
	inline SWWeakRef& operator = (const SWWeakRef& ref)
	{
		if ( m_ref != ref.m_ref )
		{
			if ( m_ref )
			{
				m_ref->decWeak();
				m_ref = NULL;
			}
			if ( ref.isValid() )
			{
				m_ref = ref.m_ref;
				m_ref->incWeak();
			}
		}
		return *this;
	}
	inline SWWeakRef& operator = (const T* obj)
	{
		if ( m_ref != (obj?obj->getRefObject():NULL) )
		{
			if ( m_ref )
			{
				m_ref->decWeak();
				m_ref = NULL;
			}
			if ( obj && obj->getRefObject() )
			{
				m_ref = obj->getRefObject();
				m_ref->incWeak();
			}
		}
		return *this;
	}

	inline T* operator ()() const { return (T*)(isValid()? m_ref->getObject():NULL); };
	inline T* getRaw() const { return (T*)(m_ref? m_ref->getObject():NULL); };

	inline bool operator < (const T* obj) const { return ((*this)() < obj); };
	inline bool operator > (const T* obj) const { return ((*this)() > obj); };
	inline bool operator == (const T* obj) const { return ((*this)() == obj); };
	inline bool operator != (const T* obj) const { return ((*this)() != obj); };
	inline bool operator < (const SWWeakRef& ref) const { return ((*this)() < ref()); };
	inline bool operator > (const SWWeakRef& ref) const { return ((*this)() > ref()); };
	inline bool operator == (const SWWeakRef& ref) const { return ((*this)() == ref()); };
	inline bool operator != (const SWWeakRef& ref) const { return ((*this)() != ref()); };
	inline bool isValid() const { return (m_ref && (m_ref->getHard() > 0)); };
	inline bool isUndead() const { return (m_ref && m_ref->isUndead() ); };
};


/**
@brief 강참조를 하는 참조자
객체의 생명에 관여 한다.
객체의 강참조가 없으면 "SWRefCountable::_make_zero_ref()" 를 호출한다.
"operator ()"를 통에 객체를 참조 가능.
*/
template<typename T>
class SWHardRef
{
	SWRefObject* m_ref;
public:
	inline SWHardRef() : m_ref(NULL) {};
	inline SWHardRef(const T* obj) : m_ref(NULL) { *this = obj; };
	inline SWHardRef(const SWHardRef& ref) : m_ref(NULL) { *this = ref; };
	inline ~SWHardRef() { *this = NULL; }
	inline SWHardRef& operator = (const SWHardRef& ref)
	{
		if ( m_ref != ref.m_ref )
		{
			if ( m_ref )
			{
				m_ref->decHard();
				m_ref->decWeak();
				m_ref = NULL;
			}
			if ( ref.isValid() )
			{
				m_ref = ref.m_ref;
				m_ref->incHard();
				m_ref->incWeak();
			}
		}
		return *this;
	}
	inline SWHardRef& operator = (const T* obj)
	{
		if ( m_ref != (obj?obj->getRefObject():NULL) )
		{
			if ( m_ref )
			{
				m_ref->decHard();
				m_ref->decWeak();
				m_ref = NULL;
			}
			if ( obj && obj->getRefObject() )
			{
				m_ref = obj->getRefObject();
				m_ref->incHard();
				m_ref->incWeak();
			}
		}
		return *this;
	}

	inline T* operator ()() const { return (T*)(isValid()? m_ref->getObject():NULL); };
	inline T* getRaw() const { return (T*)(m_ref? m_ref->getObject():NULL); };

	inline bool operator < (const T* obj) const { return ((*this)() < obj); };
	inline bool operator > (const T* obj) const { return ((*this)() > obj); };
	inline bool operator == (const T* obj) const { return ((*this)() == obj); };
	inline bool operator != (const T* obj) const { return ((*this)() != obj); };
	inline bool operator < (const SWHardRef& ref) const { return ((*this)() < ref()); };
	inline bool operator > (const SWHardRef& ref) const { return ((*this)() > ref()); };
	inline bool operator == (const SWHardRef& ref) const { return ((*this)() == ref()); };
	inline bool operator != (const SWHardRef& ref) const { return ((*this)() != ref()); };
	inline bool isValid() const { return (m_ref && (m_ref->getHard() > 0)); };
	inline bool isUndead() const { return (m_ref && m_ref->isUndead() ); };
};

#endif
