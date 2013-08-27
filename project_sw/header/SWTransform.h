//
//  SWTransform.h
//  prototype
//
//  Created by 한상운 on 13. 5. 23..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef prototype_SWTransform_h
#define prototype_SWTransform_h

#include "SWComponent.h"
#include "SWMatrix4x4.h"
#include <list>

/**
 @brief 객체가 공간 개념을 갖게 한다.
        특별한 경우를 제외하고 가장 기본이 되는 컴포넌트
 */
class SWTransform : public SWComponent
{
    SW_RTTI( SWTransform, SWComponent );
public:
	typedef std::list< SWHardRef<SWTransform> > TransformList;
private:

	SWWeakRef<SWTransform> m_parent;
	TransformList          m_children;

	SWVector3f    m_scale;    //< 비율.
	SWQuaternion  m_rotate;   //< 회전.
	SWVector3f    m_position; //< 위치.
	SWMatrix4x4   m_worldMat; //< world matrix

public:

    SWTransform();
    ~SWTransform();

	void setParent( SWTransform* parent );
	SWTransform* getParent();

	const SWMatrix4x4& getWorldMatrix();

	const SWVector3f&   getLocalScale();
	const SWQuaternion& getLocalRotate();
	const SWVector3f&   getLocalPosition();

	void setLocalScale( const SWVector3f& scale );
	void setLocalRotate( const SWQuaternion& rotate );
	void setLocalPosition( const SWVector3f& position );

	SWTransform* find( const std::string& query );
	void copyChildren( TransformList& transList );

	void update();
};

#endif
