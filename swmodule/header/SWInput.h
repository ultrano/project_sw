#ifndef SWInput_h__
#define SWInput_h__

#include "SWMemory.h"
#include "SWList.h"

#define SWInput (__SWInput::getInstance())

class __SWInput : public SWMemory
{
	friend class SWGameContext;
private:

	int m_touchState;
	int m_deltaX;
	int m_deltaY;
	tvec2 m_touchXY;

	enum { eKeyCount = 256 };
	bool m_keyFlags[eKeyCount];
	tuint m_lastKey;
	int m_keyDownCount;

	SWList::Type m_listeners;

	__SWInput();
	~__SWInput();

public:

	static __SWInput& getInstance();

	int getTouchState() const;
	const tvec2& getTouchXY() const;

	int getDeltaX() const;
	int getDeltaY() const;

	bool getKey( tuint code ) const;
	tuint getLastKey() const;
	//bool anyKeyDown() const;

	void addInputDelegate( SWDelegator* del );
	void removeInputDelegate( SWDelegator* del );
};

#endif // SWInput_h__