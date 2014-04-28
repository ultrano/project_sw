//
//  SWMemory.h
//  protopang
//
//  Created by 한상운 on 13. 6. 10..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWMemory_h
#define protopang_SWMemory_h

#include <signal.h>
#include <stdio.h>
#include <limits>
#include <string>

void* SWAlloc( size_t size );
void  SWFree( void* mem );

class SWMemory
{
public:
    
	static	void*	operator new( size_t size );
    static  void*   operator new( size_t size, void* memory );
	static	void	operator delete(void* memory);
    
};

template <class T> 
struct SWAllocator {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> struct rebind { typedef SWAllocator<U> other; };
    SWAllocator() throw() {}
    SWAllocator(const SWAllocator&) throw() {}

    template <class U> SWAllocator(const SWAllocator<U>&) throw(){}

    ~SWAllocator() throw() {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type s, void const * = 0) {
        if (0 == s)
            return NULL;
        pointer temp = (pointer)SWAlloc(s * sizeof(T)); 
        if (temp == NULL)
            throw std::bad_alloc();
        return temp;
    }

    void deallocate(pointer p, size_type) {
        SWFree(p);
    }

    size_type max_size() const throw() { 
        return std::numeric_limits<size_t>::max() / sizeof(T); 
    }

    void construct(pointer p, const T& val) {
        new((void *)p) T(val);
    }

    void destroy(pointer p) {
        p->~T();
    }
};
template<class _Ty, class _Other> 
inline bool operator==(const SWAllocator<_Ty>&, const SWAllocator<_Other>&)
{	// test for allocator equality
	return (true);
}
template<class _Ty, class _Other> 
inline bool operator==(const SWAllocator<_Ty>&, const _Other&)
{	// test for allocator equality
	return (false);
}
#endif
