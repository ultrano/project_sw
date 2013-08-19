//
//  SWTouchSensor.h
//  prototype
//
//  Created by 한상운 on 13. 5. 29..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWTouchSensor_h
#define prototype_SWTouchSensor_h

#include "SWComponent.h"
#include "SWRect.h"
#include "SWVector2f.h"
#include <string>

/**
 @brief 지정한 화면상의 위치가 터치되는지 감지한다.
        SWTransform 의 위치를 이용한다.
 @warning 필요 컴포넌트 : SWTransform
 */
class SWTouchSensor : public SWComponent
{
    
    SW_RTTI( SWTouchSensor, SWComponent );
    
private:
    
    //! 컬백을 위한 핸들러
    SWWeakRef<SWObject> m_target;
    SWHandler           m_handler;
   
    //! 핸들러가 공유되어 사용되는경우
    //! 태그를 이용해 구별 가능.
    std::string m_tag;
    
    //! 터치 영역의 크기
    float m_width, m_height;
    
    //! 객체의 공간 좌표에서의 터치영역 오프셋
    SWVector2f m_offset;
    
    
    //! 핸들러를 호출할 경우 무슨 이벤트로
    //! 호출했는지를 알리기 위함.
    int m_type;
    int m_touchX;
    int m_touchY;
    
    //! 터치 처리는 할지 말지를 결정.
    bool m_isDisable;
    
public:
    
    SWTouchSensor();
    SWTouchSensor( SWObject* target, const SWHandler& handler
                  , const std::string& tagName
                  , float w, float h
                  , const SWVector2f& offset = SWVector2f(0,0) );
    ~SWTouchSensor();
    
    void setHandler( SWObject* target, const SWHandler& handler );
    
    void setTag( const std::string& tag);
    const std::string& getTag() const;
    
    int  getType() const { return m_type; };
    int  getX() const { return m_touchX; };
    int  getY() const { return m_touchY; };
    
    void setWidth( float width );
    void setHeight( float height );
    void setOffset( const SWVector2f& offset );
    
    float getWidth() const;
    float getHeight() const;
    const SWVector2f& getOffset() const;
    
    void setDisable( bool check ) { m_isDisable = check; };
    bool isDisable() const { return m_isDisable; };
    
    void handleTouch( int type, int x, int y );
    
};

#endif
