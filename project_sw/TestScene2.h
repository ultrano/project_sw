#ifndef TestScene2_h__
#define TestScene2_h__

#include "SWGameScene.h"
#include "SWGameContext.h"
#include "SWGameObject.h"
#include "SWSocket.h"

#include "SWUtil.h"

class TestScene2 : public SWGameScene
{
	SW_RTTI( TestScene2, SWGameScene );

	float m_time;
	float m_remain;
	SWHardRef<SWSocket> m_sock;
	SWHardRef<SWOutputStream> m_os;
	SWHardRef<SWInputStream> m_is;

public:
	TestScene2() {};
	TestScene2(factory_constructor){};
	void onAwake()
	{
		m_sock = SWSocket::connect( "127.0.0.1", 4000 );
		
		m_os = m_sock()->getOutputStream();
		m_is = m_sock()->getInputStream();
		
		tstring msg = "hello there";
		m_os()->write( (tbyte*)&msg[0], msg.size() );
	}

	void onUpdate()
	{
		char buf[256] = {0};
		if ( m_is()->read( (tbyte*)&buf[0], 256 ) > 0 )
		{
			SWLog( buf );
			tstring msg = "hello there";
			m_os()->write( (tbyte*)&msg[0], msg.size() );
		}
		else
		{
			SWLog( "wait for message" );
		}
	}
};

#endif // TestScene2_h__