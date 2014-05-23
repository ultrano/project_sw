//
//  SWCriticalSection.h
//  protopang
//
//  Created by SangWoon-Han on 13. 6. 4..
//  Copyright (c) 2013 SangWoon-Han. All rights reserved.
//

#ifndef protopang_SWCriticalSection_h
#define protopang_SWCriticalSection_h

//! 간단한 구조의 임계영역 클레스
//! 기존에 써왔던 CRITICAL_SECTION 이 window 에서만 사용 가능한거 같아서 일단
//! 만들어서 씀. 발견하면 내부 구조만 바꾸면 될듯.
//! 그리고 세마포어나 뮤텍스는 커널모드 임계구역 이라서 속도 고려상 제외.
//! CPU의 한 사이클 안에 수정이 가능한(이라고 알고 있다;) bool값을 signal 값으로 사용.
class SWCriticalSection
{
    //! 신호 (true:진입가능, false:진입불가)
    bool m_signal;
    
public:
    
    SWCriticalSection();
    
    //! 신호를 false 로 바꾼다.
    void enter();
    
    //! 신호를 true 로 바꾼다.
    void leave();
    
    //! 신호가 true가 될때 까지 대기 한다.
    int  wait();
    
    
};

//! 임계 구역 편의 클레스
//! 생성자에서 SWCriticalSection::enter() 호출
//! 소멸자에서 SWCriticalSection::leave() 호출
class SWSectionGuard
{
    SWCriticalSection& m_cs;
public:
    SWSectionGuard( SWCriticalSection& cs );
    ~SWSectionGuard();
};

#endif
