//
//  SWTimer.h
//  protopang
//
//  Created by 한상운 on 13. 6. 12..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef protopang_SWTimer_h
#define protopang_SWTimer_h
/*
#include "SWObject.h"
#include <string>

class SWTimer : public SWObject
{
    SW_RTTI( SWTimer, SWObject );
    
    SWWeakRef<SWObject> m_target;
    SWHandler m_handler;
    
    float m_interval;
    tstring m_tag;
    
    float m_accumulation;
    bool  m_isRunning;
    
public:
    
    //! 타이머는 컬백 핸들러를 반드시 필요로 한다.
    SWTimer( SWObject* target, SWHandler handler);
    
    //! 시간 간격
    void  setInterval( float interval ) { m_interval = interval; };
    float getInterval() const { return m_interval; };
    
    //! 같은 핸들러를 공유할때 타이머 구분을 위한 태그
    void  setTag( const tstring& tag ) { m_tag = tag; };
    const tstring& getTag() const { return m_tag; };
    
    //! stop & start
    void reStart();
    
    //! pause 상태일 경우 축적된 시간값을 유지한채 다시 시작
    //! 축적된 시간값 초기화와 함께 시작 하고 싶다면 reStart 를 호출
    void start();
    
    //! 축척된 시간값을 0으로 놓고 중지.
    void stop();
    
    //! 축척된 시간값을 유지한채 중지.
    void pause();
    
    //! 시간을 축척하기 위한 업데이트
    void update( float elapsed );
};
*/
#endif
